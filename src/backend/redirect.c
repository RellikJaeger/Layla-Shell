/* 
 *    Programmed By: Mohammed Isam Mohammed [mohammed_isam1984@yahoo.com]
 *    Copyright 2016, 2017, 2018, 2019 (c)
 * 
 *    file: redirect.c
 *    This file is part of the Layla Shell project.
 *
 *    Layla Shell is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Layla Shell is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Layla Shell.  If not, see <http://www.gnu.org/licenses/>.
 */    

#define _XOPEN_SOURCE   500     /* fdopen() and mkdtemp() */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "../cmd.h"
#include "backend.h"
#include "../parser/parser.h"
#include "../error/error.h"
#include "../debug.h"


/**********************************************
 * Functions used by the back-end executor to
 * perform I/O redirections of a command.
 **********************************************/

/* special value to represent an invalid variable */
#define INVALID_VAR     ((char *)-1)

/*
 * when a builtin or function redirects one of the shell's standard streams,
 * we store the original stream so that we can restore it after the builtin
 * or function finishes execution.
 */
int saved_fd[3] = { -1, -1, -1 };

/*
 * if we are executing a builtin utility or a shell function, we need to save the
 * state of the standard streams so that we can restore them after the utility or
 * function finishes execution.
 */
void save_std(int fd)
{
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
    saved_fd[fd] = dup(fd);
}


/*
 * after a builtin utility or a shell function finishes execution, restore the
 * standard streams if there were any I/O redirections.
 */
void restore_stds(void)
{
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
    int i = 0;
    for( ; i < 3; i++)
    {
        if(saved_fd[i] >= 0)
        {
            dup2(saved_fd[i], i);
            close(saved_fd[i]);
            saved_fd[i] = -1;
        }
    }
}


/*
 * perform process substitution. the op parameter specifies the redirection operator to
 * apply to the process substitution, which can be '<' or '>'. the cmdline parameter
 * contains the command(s) to execute in the process. the tmpname parameter contains
 * the pathname of the file we'll use for the redirection.
 */
void redirect_proc_do(char *cmdline, char op, char *tmpname)
{
    if(fork_child() == 0)
    {
        int i = strlen(cmdline)-1;
        int j = i+strlen(tmpname)+12;
        char *buf = malloc(j);
        if(!buf)
        {
            return;
        }

        char c = cmdline[i];
        cmdline[i] = ' ';
        sprintf(buf, "{ %s} %c%s", cmdline+1, (op == '>') ? '<' : '>', tmpname);
        cmdline[i] = c;

        struct source_s src;
        src.buffer   = buf;
        src.bufsize  = j;
        src.curpos   = INIT_SRC_POS;
        src.srctype = SOURCE_FIFO;
        src.srcname = NULL;

        parse_and_execute(&src);
        unlink(tmpname);
        exit(exit_status);
    }
}


/*
 * prepare for process substitution by opening a FIFO under /tmp/lsh, or if the
 * system doesn't support named FIFO's, create a regular pipe and use its file
 * descriptors in place of the FIFO. in the latter case, the pipe will be passed
 * to the process as a file named /dev/fdN, where N is the file descriptor number.
 * returns the pathname of the FIFO/pipe, so that we can pass it to the other end,
 * i.e. the command which will read from or write to the process we'll fork.
 */
char *redirect_proc(char op, char *cmdline)
{
    char *tmpdir = get_shell_varp("TMPDIR", "/tmp");
    int len = strlen(tmpdir);
    char tmpname[len+12];
    int tries = 100;
    while(tries--)
    {
        sprintf(tmpname, "%s%clsh/fifo%d", tmpdir, '/', tries);
        /* try to perform process substitution via using a named pipe/fifo */
        if(mkfifo(tmpname, 0600) != 0)
        {
            if(errno == EEXIST)
            {
                continue;
            }
            /*
             * if the system doesn't support named pipes, or another error occurred, such as
             * insufficient disk space, try performing this via a regular pipe, whose path we'll
             * pass as /dev/fd/filedes. if the system doesn't support this scheme, we won't be able
             * to perform process substitution.
             */
            int filedes[2];
            if(pipe(filedes) == 0)
            {
                char buf[16];
                sprintf(buf, "/dev/fd/%d", filedes[(op == '<') ? 0 : 1]);
                if(file_exists(buf))
                {
                    redirect_proc_do(cmdline, op, buf);
                    if(op == '<')
                    {
                        close(filedes[1]);
                        return get_malloced_str(buf);
                    }
                    else
                    {
                        close(filedes[0]);
                        return get_malloced_str(buf);
                    }
                    return NULL;
                }
                else
                {
                    fprintf(stderr, "%s: error creating fifo: %s\n", SHELL_NAME, "system doesn't support `/dev/fd` file names");
                    close(filedes[0]);
                    close(filedes[1]);
                    return NULL;
                }
            }
            else
            {
                fprintf(stderr, "%s: error creating fifo: %s\n", SHELL_NAME, strerror(errno));
                return NULL;
            }
        }
        redirect_proc_do(cmdline, op, tmpname);
        return get_malloced_str(tmpname);
    }
    return NULL;
}


/*
 * get the slot belonging to this fileno, or else the first empty
 * slot in the redirection table. returns -1 if no slot is available.
 */
int get_slot(int fileno, struct io_file_s *io_files)
{
    int i;
    for(i = 0; i < FOPEN_MAX; i++)
    {
        if(io_files[i].fileno == fileno || io_files[i].fileno == -1)
        {
            return i;
        }
    }
    return -1;
}


/*
 * prepare a redirection file from the given redirection node.
 */
int redirect_prep_node(struct node_s *child, struct io_file_s *io_files)
{
    int fileno = -1, i;
    char *s;
    char buf[32];
    switch(child->val_type)
    {
        case VAL_SINT:
            fileno = child->val.sint;
            break;
            
        case VAL_STR:
            s = child->val.str;
            i = strlen(s);
            /*
             * check for the non-POSIX bash and zsh redirection extensions of {var}<&N
             * and {var}>&N. the {var} part would have been added as the previous
             * node.
             */
            if(s[0] == '{' && s[i-1] == '}')
            {
                s[i-1] = '\0';
                struct node_s *child2 = child->first_child;
                if(child2)
                {
                    child2 = child2->first_child;
                }
                if(child2->val_type != VAL_STR)
                {
                    child2 = NULL;
                }

                /*
                 * if the path is '-', it means we need to close the fd, so we'll get the fd number
                 * from the shell variable where it was saved before.
                 */
                if(child2 && child2->val.str && strcmp(child2->val.str, "-") == 0)
                {
                    char *s2 = get_shell_varp(s+1, NULL);
                    if(!s2)
                    {
                        s[i-1] = '}';
                        break;
                    }
                    char *strend;
                    fileno = strtol(s2, &strend, 10);
                    s[i-1] = '}';
                    if(strend == s2)
                    {
                        fileno = -1;
                    }
                    break;
                }

                /* search for an available slot for the new file descriptor, starting with #10 */
                for(fileno = 10; fileno < FOPEN_MAX; fileno++)
                {
                    /* get a vacant file descriptor */
                    errno = 0;
                    if(fcntl(fileno, F_GETFD) == -1 && errno == EBADF)
                    {
                        /* we need to save the file number for later reference */
                        struct symtab_entry_s *entry = add_to_symtab(s+1);
                        if(entry)
                        {
                            sprintf(buf, "%d", fileno);
                            symtab_entry_setval(entry, buf);
                        }
                        break;
                    }
                }           
                s[i-1] = '}';
            }
            break;
            
        default:
            break;
    }
        
    if(fileno < 0 || fileno >= FOPEN_MAX)
    {
        sprintf(buf, "%d", fileno);
        BACKEND_RAISE_ERROR(INVALID_REDIRECT_FILENO, buf, NULL);
        //fprintf(stderr, "%s: invalid redirection file number: %d\n", SHELL_NAME, fileno);
        return 0;
    }
    
    if((i = get_slot(fileno, io_files)) == -1)
    {
        fprintf(stderr, "%s: too many open files\n", SHELL_NAME);
        return 0;
    }

    struct node_s *io = child->first_child;
    int res = (io->type == NODE_IO_FILE) ?
               file_redirect_prep(io, &io_files[i]) :
               heredoc_redirect_prep(io, &io_files[i]);
    if(!res)
    {
        return 0;
    }
    else
    {
        io_files[i].fileno = fileno;
        /*
         * in the case of combined stdout/stderr redirection, redirected word must
         * not be a number or '-'. this form of redirection is written as >&word or &>word.
         */
        child = child->first_child;
        if(fileno == 1 && child && child->type == NODE_IO_FILE)
        {
            if(io_files[i].duplicates == -1 && io_files[i].path[0] != '-' &&
               (child->val.chr == IO_FILE_AND_GREAT_GREAT || child->val.chr == IO_FILE_GREATAND))
            {
                struct node_s *node2 = io_file_node(2, IO_FILE_GREATAND, "1", child->lineno);
                if(node2)
                {
                    if((i = get_slot(2, io_files)) != -1)
                    {
                        struct node_s *io = node2->first_child;
                        int res = (io->type == NODE_IO_FILE) ? file_redirect_prep(io, &io_files[i]) :
                                                               heredoc_redirect_prep(io, &io_files[i]);
                        if(!res)
                        {
                            return 0;
                        }
                        io_files[i].fileno = 2;
                    }
                    free_node_tree(node2);
                }
                else
                {
                    fprintf(stderr, "%s: failed to duplicate stdout on stderr\n", SHELL_NAME);
                    return 0;
                }
            }
        }
    }
    return 1;
}


/*
 * initialize the redirection table for the command to be executed.
 */
int init_redirect_list(struct node_s *node, struct io_file_s *io_files)
{
    if(!io_files)
    {
        return 0;
    }
    int  i = 0;
    int  total_redirects = 0;
    for( ; i < FOPEN_MAX; i++)
    {
        io_files[i].path        = NULL;
        io_files[i].fileno      = -1;
        io_files[i].duplicates  = -1;
        io_files[i].open_mode   =  0;
        io_files[i].extra_flags =  0;
    }
    if(!node)
    {
        return 0;
    }

    struct node_s *child = node->first_child;
    /* prepare the redirections */
    while(child)
    {
        if(child->type == NODE_IO_REDIRECT)
        {
            if(!redirect_prep_node(child, io_files))
            {
                total_redirects = -1;
            }
            else
            {
                total_redirects++;
            }
        }
        child = child->next_sibling;
    }
    return total_redirects;
}


#define OPEN_SPECIAL_ERROR()                            \
do {                                                    \
    fprintf(stderr,                                     \
            "%s: error opening %s: "                    \
            "use of invalid redirection operator\n",  \
            SHELL_NAME, path);                          \
    errno = EINVAL;                                     \
    return -1;                                          \
} while(0)

/*
 * open a special file, such as a remote tcp or udp connection, or a filename
 * such as /dev/stdin.
 * returns the file descriptor on which the file is opened, -1 otherwise.
 */
int open_special(char *path, int mode)
{
    int fd = -1, i, remote = 0;
    if(strstr(path, "/dev/fd/") == path)
    {
        i = atoi(path+8);
        if(i < 0)
        {
            return -1;
        }
        fd = dup(i);
    }
    else if(strcmp(path, "/dev/stdin") == 0)
    {
        if(!flag_set(mode, O_RDONLY) && !flag_set(mode, O_RDWR))
        {
            OPEN_SPECIAL_ERROR();
        }
        fd = dup(0);
    }
    else if(strcmp(path, "/dev/stdout") == 0)
    {
        if(!flag_set(mode, O_RDWR))
        {
            OPEN_SPECIAL_ERROR();
        }
        fd = dup(1);
    }
    else if(strcmp(path, "/dev/stderr") == 0)
    {
        if(!flag_set(mode, O_RDWR))
        {
            OPEN_SPECIAL_ERROR();
        }
        fd = dup(2);
    }
    else if(strstr(path, "/dev/tcp/") == path)
    {
        remote = 1;
    }
    else if(strstr(path, "/dev/udp/") == path)
    {
        remote = 2;
    }

    if(remote)
    {
        /* get the hostname and port parts */
        char *s1 = path+9;
        char *s2 = strchr(s1, '/');
        if(!s2)
        {
            fprintf(stderr, "%s: error opening socket: missing port number\n", SHELL_NAME);
            errno = EINVAL;
            return -1;
        }
        if(!*++s2)
        {
            fprintf(stderr, "%s: error opening socket: missing port number\n", SHELL_NAME);
            errno = EINVAL;
            return -1;
        }
        
        int port;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        port = atoi(s2);
        fd = socket(AF_INET, (remote == 1) ? SOCK_STREAM : SOCK_DGRAM, 0);
        if(fd < 0) 
        {
            //fprintf(stderr, "%s: error opening socket: %s\n", SHELL_NAME, strerror(errno));
            return -1;
        }
        s2[-1] = '\0';
        server = gethostbyname(s1);
        if(server == NULL)
        {
            fprintf(stderr, "%s: no such host: %s\n", SHELL_NAME, s1);
            s2[-1] = '/';
            return -1;
        }
        s2[-1] = '/';
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        memcpy(server->h_addr_list[0], &serv_addr.sin_addr.s_addr, server->h_length);
        serv_addr.sin_port = htons(port);
        if(connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        {
            //fprintf(stderr, "%s: error connecting to socket: %s\n", SHELL_NAME, strerror(errno));
            return -1;
        }
    }
    return fd;
}


/*
 * perform the redirections in the *io_files redirection list. this function should be
 * called after the shell has forked a child process to handle execution of a command.
 * if called from the shell itself, the redirections will affect the file descriptors
 * of the shell process.
 */
int redirect_do(struct io_file_s *io_files, int do_savestd)
{
    int i, j;
    /* perform the redirections */
    for(i = 0; i < FOPEN_MAX; i++)
    {
        j = io_files[i].fileno;
        char *path;
        if((path = io_files[i].path))
        {
            //if(i <= 2) save_std(i);
            if(path[0] == '-' && path[1] == '\0')
            {
                if(j <= 2 && do_savestd)
                {
                    save_std(j);
                }
                close(j);

                /* POSIX says we can open an "unspecified file" in this case */
                if(j == 0)
                {
                    open("/dev/null", O_RDONLY);
                }
                else if(j == 1 || j == 2)
                {
                    open("/dev/null", O_WRONLY);
                }
            }
            else if(path[0] != '\0')
            {
                path = word_expand_to_str(path);
                if(!path)
                {
                    fprintf(stderr, "%s: failed to expand path: %s\n", SHELL_NAME, io_files[i].path);
                    return 0;
                }

                /* check the noclobber situation */
                if(io_files[i].open_mode == MODE_WRITE && option_set('C'))
                {
                    if(!flag_set(io_files[i].extra_flags, NOCLOBBER_FLAG))
                    {
                        struct stat st;
                        if(stat(path, &st) == 0)
                        {
                            if(S_ISREG(st.st_mode))
                            {
                                fprintf(stderr, "%s: redirection fail: file already exists: %s\n", SHELL_NAME, path);
                                free(path);
                                return 0;
                            }
                        }
                    }
                    io_files[i].open_mode |= O_EXCL;
                }
                
                /*
                 * >#((expr)) and <#((expr)) are non-POSIX extensions to move
                 * I/O file pointers to the offset specified by expr.
                 * 
                 * TODO: implement ksh-like redirection operators
                 *       '<#pattern' and '<##pattern'.
                 * 
                 *       See the Input and Output section on
                 *       https://docs.oracle.com/cd/E36784_01/html/E36870/ksh-1.html
                 */
                if(match_filename("#((*))", path, 0, 1))
                {
                    long len = strlen(path)-5;
                    char expr[len+1];
                    strncpy(expr, path+3, len);
                    expr[len] = '\0';
                    char *p2 = arithm_expand(expr);
                    if(p2)
                    {
                        char *pend;
                        len = strtol(p2, &pend, 10);
                        if(pend == p2)
                        {
                            free(p2);
                            BACKEND_RAISE_ERROR(FAILED_REDIRECT, "invalid file offset", expr);
                            EXIT_IF_NONINTERACTIVE();
                            return 0;
                        }
                        free(p2);
                        /* now seek to the given offset */
                        if(lseek(i, len, SEEK_SET) < 0)
                        {
                            BACKEND_RAISE_ERROR(FAILED_REDIRECT, "failed to lseek file", strerror(errno));
                            EXIT_IF_NONINTERACTIVE();
                            return 0;
                        }
                    }
                    else
                    {
                            BACKEND_RAISE_ERROR(FAILED_REDIRECT, "invalid file offset", expr);
                            EXIT_IF_NONINTERACTIVE();
                            return 0;
                    }
                }
                
                /*
                 * 'normal' file redirection.
                 */
                int fd = open(path, io_files[i].open_mode, FILE_MASK);
                if(fd < 0)
                {
                    //errno = 0;
                    if((fd = open_special(path, io_files[i].open_mode)) < 0)
                    {
                        BACKEND_RAISE_ERROR(FAILED_TO_OPEN_FILE, io_files[i].path, strerror(errno));
                        EXIT_IF_NONINTERACTIVE();
                        free(path);
                        return 0;
                    }
                }

                if(j <= 2 && do_savestd)
                {
                    save_std(j);
                }
                if(fd != j)
                {
                    //close(j);
                    dup2(fd, j);
                    close(fd);
                }
                free(path);
            }
        }
        else if(io_files[i].duplicates >= 0)
        {
            struct io_file_s *f = &io_files[i];
            int flags2 = fcntl(f->duplicates, F_GETFL);

            int err = 0;
            switch(f->open_mode)
            {
                case MODE_WRITE:
                case MODE_APPEND:
                    if(!flag_set(flags2, O_WRONLY) && !flag_set(flags2, O_RDWR))
                    {
                        err = 1;
                    }
                    break;

                case MODE_READ:
                    if(!flag_set(flags2, O_RDONLY) && !flag_set(flags2, O_RDWR))
                    {
                        err = 1;
                    }
                    break;
            }
            /* if error, bail out on all redirections */
            if(err)
            {
                BACKEND_RAISE_ERROR(FAILED_REDIRECT, NULL, NULL);
                EXIT_IF_NONINTERACTIVE();
                return 0;
            }

            if(j <= 2 && do_savestd)
            {
                save_std(j);
            }
            dup2(f->duplicates, j);
            if(flag_set(f->extra_flags, CLOOPEN_FLAG))
            {
                close(f->duplicates);
            }
        }
    } /* end for */
    return 1;
}


/*
 * prepare a redirection list and then execute the redirections.
 */
int redirect_prep_and_do(struct node_s *redirect_list)
{
    if(!redirect_list)
    {
        return 1;
    }
    struct io_file_s io_files[FOPEN_MAX];
    if(init_redirect_list(redirect_list, io_files) == -1)
    {
        return 0;
    }
    if(!redirect_do(io_files, 1))
    {
        return 0;
    }
    return 1;
}


/*
 * prepare an I/O redirection for a file.
 */
int file_redirect_prep(struct node_s *node, struct io_file_s *io_file)
{
    int fileno     = -1;
    int duplicates = 0;
    struct node_s *child = node->first_child;
    if(!child)
    {
        return 0;
    }
    char *str = child->val.str;
    char buf[32];

    /* r-shells can't redirect output */
    if(startup_finished && option_set('r'))
    {
        char c = node->val.chr;
        if(c == IO_FILE_LESSGREAT || c == IO_FILE_CLOBBER || c == IO_FILE_GREAT ||
           c == IO_FILE_GREATAND  || c == IO_FILE_DGREAT  || c == IO_FILE_AND_GREAT_GREAT)
        {
            /*
             * NOTE: consequences of failed redirection are handled by the caller, 
             *       i.e. do_simple_command().
             */
            fprintf(stderr," %s: restricted shells can't redirect output\n", SHELL_NAME);
            return 0;
        }
    }
    
    switch(node->val.chr)
    {
        case IO_FILE_LESS     :
            io_file->open_mode = MODE_READ;
            break;
            
        case IO_FILE_LESSAND  :
            duplicates = 1;
            io_file->open_mode = MODE_READ;
            break;
            
        case IO_FILE_LESSGREAT:
            io_file->open_mode = MODE_READ|MODE_WRITE;
            break;
            
        case IO_FILE_CLOBBER  :
            io_file->open_mode   = MODE_WRITE;
            io_file->extra_flags = NOCLOBBER_FLAG;
            break;
            
        case IO_FILE_GREAT    :
            io_file->open_mode = MODE_WRITE;
            break;
            
        case IO_FILE_GREATAND :
            duplicates = 1;
            io_file->open_mode = MODE_WRITE;
            break;
            
        case IO_FILE_AND_GREAT_GREAT:
            duplicates = 1;
            io_file->open_mode = MODE_APPEND;
            break;
            
        case IO_FILE_DGREAT   :
            io_file->open_mode = MODE_APPEND;
            break;
    }

    if(duplicates && strcmp(str, "-"))
    {
        /* I/O from coprocess */
        if(strcmp(str, "p-") == 0 || strcmp(str, "p") == 0)
        {
            switch(node->val.chr)
            {
                case IO_FILE_LESSAND  :
                    if(wfiledes[0] == -1)
                    {
                        goto invalid;
                    }
                    fileno = wfiledes[0];
                    break;
                    
                case IO_FILE_GREATAND :
                    if(rfiledes[1] == -1)
                    {
                        goto invalid;
                    }
                    fileno = rfiledes[1];
                    break;
                    
                default:
                    goto invalid;
            }
        }
        else
        {
            char *str2 = NULL;
            /* get the file number from the shell variable in the >{$var} type of redirection */
            if(str[0] == '$')
            {
                str2 = word_expand_to_str(str);
                if(str2)
                {
                    str = get_malloced_str(str2);
                    if(!str)
                    {
                        return 0;
                    }
                    free(str2);
                }
            }
            char *strend;
            fileno = strtol(str, &strend, 10);
            if(strend == str)
            {
                io_file->duplicates = -1;
                io_file->path       = str;
                return 1;
            }
        }
        if(fileno < 0 || fileno >= FOPEN_MAX)
        {
            goto invalid;
        }
        /* >&n- and <&n-, but don't close coproc files */
        if(str[strlen(str)-1] == '-' && strcmp(str, "p-"))
        {
            io_file->extra_flags |= CLOOPEN_FLAG;
        }
        io_file->duplicates = fileno;
        io_file->path       = NULL;
    }
    else
    {
        io_file->duplicates = -1;
        io_file->path       = str;
    }
    
    return 1;
    
invalid:
    sprintf(buf, "%d", fileno);
    BACKEND_RAISE_ERROR(INVALID_REDIRECT_FILENO, buf, NULL);
    /*
     * NOTE: consequences of failed redirection are handled by the caller, 
     *       i.e. do_simple_command().
     */
    return 0;
}


/*
 * when preparing a here-document redirection, perform word expansion on the word
 * starting at *p and counting len characters.
 * this function calls the function passed in the third parameter to do the actual
 * expansion, then prints the expanded result to out, which should be a temp file.
 */
void heredoc_substitute_word(char *p, size_t len, char *(func)(char *), FILE *out)
{
    /* extract the word to be substituted */
    char *tmp = malloc(len+1);
    if(!tmp)
    {
        char *p2 = p;
        while(len--)
        {
            fputc(*p2, out);
            p2++;
        }
        return;
    }
    strncpy(tmp, p, len);
    tmp[len] = '\0';
    /* and expand it */
    char *tmp2 = func(tmp);
    if(tmp2 && tmp2 != INVALID_VAR)
    {
        fprintf(out, "%s", tmp2);
        free(tmp2);
    }
    else
    {
        fprintf(out, "%s", tmp);
    }
    free(tmp);
}


/*
 * prepare an I/O redirection for a here document.
 */
int heredoc_redirect_prep(struct node_s *node, struct io_file_s *io_file)
{
    struct node_s *child = node->first_child;
    if(!child)
    {
        return 0;
    }

    /* we implement here-documents as temp files */
    char *heredoc = child->val.str;
    FILE *tmp = tmpfile();
    if(!tmp)
    {
        return 0;
    }

    /* determine whether to word-expand the here-document body or not */
    if(node->val.chr == IO_HERE_NOEXPAND)
    {
        fprintf(tmp, "%s", heredoc);
    }
    else
    {
        size_t len, i;
        char *p = heredoc, *p2;
        char c;
        char *(*func)(char *);

        do
        {
            switch(*p)
            {
                case '\\':
                    /* skip \\n */
                    if(p[1] == '\n')
                    {
                        p++;
                        break;
                    }

                    /* skip backslash-quoted '`', '$' and '\' */
                    if(p[1] == '`' || p[1] == '$' || p[1] == '\\')
                    {
                        p++;
                    }
                    fputc(*p, tmp);
                    break;

                case '`':
                    /* find the closing back quote */
                    if((len = find_closing_quote(p, 0)) == 0)
                    {
                        /* not found. print the ` and break */
                        fputc(*p, tmp);
                        break;
                    }
                    /* otherwise, extract the command and substitute its output */
                    heredoc_substitute_word(p, len+1, command_substitute, tmp);
                    p += len;
                    break;

                /*
                 * the $ sign might introduce:
                 * - ANSI-C strings: $''
                 * - arithmetic expansions (non-POSIX): $[]
                 * - parameter expansions: ${var} or $var
                 * - command substitutions: $()
                 * - arithmetic expansions (POSIX): $(())
                 */
                case '$':
                    c = p[1];
                    switch(c)
                    {
                        /*
                         * ANSI-C string
                         */
                        case '\'':
                            /* find the closing quote */
                            if((len = find_closing_quote(p+1, 1)) == 0)
                            {
                                /* not found. print the ' and break */
                                fputc(*p, tmp);
                                break;
                            }
                            /* otherwise, extract the string and substitute its value */
                            heredoc_substitute_word(p, len+2, ansic_expand, tmp);
                            p += len;
                            break;

                    /*
                     * $[ ... ] is a deprecated form of integer arithmetic, similar to (( ... )).
                     */
                        case '{':
                        case '[':
                            /* find the closing quote */
                            if((len = find_closing_brace(p+1)) == 0)
                            {
                                /* not found. print the { or [ and break */
                                fputc(*p, tmp);
                                break;
                            }
                            /* otherwise, extract the expression and substitute its value */
                            func = (c == '[') ? arithm_expand : var_expand;
                            heredoc_substitute_word(p, len+2, func, tmp);
                            p += len;
                            break;

                        /*
                         * arithmetic expansion $(()) or command substitution $().
                         */
                        case '(':
                            /* check if we have one or two opening braces */
                            i = 0;
                            if(p[2] == '(')
                            {
                                i++;
                            }
                            /* find the closing quote */
                            if((len = find_closing_brace(p+1)) == 0)
                            {
                                /* not found. print the ( and break */
                                fputc(*p, tmp);
                                break;
                            }
                            /*
                             * otherwise, extract the expression and substitute its value.
                             * if we have one brace (i == 0), we'll perform command substitution.
                             * otherwise, arithmetic expansion.
                             */
                            func = i ? arithm_expand : command_substitute;
                            heredoc_substitute_word(p, len+2, func, tmp);
                            p += len;
                            break;

                        /*
                         * special variable substitution.
                         */
                        case '#':
                            p++;
                            /*
                             * $#@ and $#* both give the same result as $# (ksh extension).
                             */
                            if(p[1] == '@' || p[1] == '*')
                            {
                                p++;
                            }
                            char p3[4];
                            sprintf(p3, "$#");
                            heredoc_substitute_word(p3, 2, var_expand, tmp);
                            break;

                        case '@':
                        case '*':
                        case '!':
                        case '?':
                        case '$':
                        case '-':
                        case '_':
                        case '<':
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            heredoc_substitute_word(p, 2, var_expand, tmp);
                            p++;
                            break;

                        default:
                            /* var names must start with an alphabetic char or _ */
                            if(!isalpha(p[1]) && p[1] != '_')
                            {
                                fputc(*p, tmp);
                                break;
                            }
                            p2 = p+1;
                            /* get the end of the var name */
                            while(*p2)
                            {
                                if(!isalnum(*p2) && *p2 != '_')
                                {
                                    break;
                                }
                                p2++;
                            }
                            /* empty name */
                            if(p2 == p+1)
                            {
                                fputc(*p, tmp);
                                break;
                            }
                            /* perform variable expansion */
                            len = p2-p;
                            heredoc_substitute_word(p, len, var_expand, tmp);
                            p += len-1;
                            break;
                    }
                    break;

                default:
                    fputc(*p, tmp);
                    break;
            }
        } while(*(++p));
    }

    int fd = fileno(tmp);
    rewind(tmp);
    io_file->duplicates  = fd;
    io_file->path        = NULL;
    io_file->extra_flags = CLOOPEN_FLAG;
    io_file->open_mode   = fcntl(fd, F_GETFL);
    return 1;
}
