 *    Copyright 2016, 2017, 2018, 2019, 2020 (c)
#include "../debug.h"
#include "builtins.h"
 * TODO: according to POSIX, regular builtin utilities should be implemented 
 *       such that they can be invoked through exec() or directly by env, ...
 * 
 * NOTE: the %% sequence in a utility's synopsis will be converted to the 
 *       utility's name when the synopsis is printed.
struct builtin_s shell_builtins[] =
        ".", "execute commands in the current environment",
        dot_builtin,       /* POSIX */
        "%% file",
        "file        execute commands from this file in the current environment\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        ":", "expand arguments (the null utility)",
        colon_builtin,       /* POSIX */
        "%% [argument...]",
        "argument    command arguments to expand\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "[", "test file attributes and compare strings",
        test_builtin,       /* POSIX */
        "%% -option expression ]",
        "For the list of options and their meanings, run `help [[`\n",
        "[[", "test file attributes and compare strings",
        test_builtin,       /* POSIX */
        "%% -abcdefgGhkLNOprsSuwx file ]]\n"
        "%% [-nz] string ]]\n"
        "%% -o [?]op ]]\n"
        "%% -t fd ]]\n"
        "%% file1 op file2 ]]\n"
        "%% expr1 -op expr2 ]]\n"
        "%% !expr ]]\n"
        "%% expr1 && expr2 ]]\n"
        "%% expr1 || expr2 ]]",
        "alias", "define or display aliases",
        alias_builtin,       /* POSIX */
        "%% [-hvp] [alias-name[=string] ...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "bg", "run jobs in the background",
        bg_builtin,       /* POSIX */
        "%% [-hv] [job_id...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "break", "exit from for, while, or until loop",
        break_builtin,       /* POSIX */
        "%% [n]",
        "n           exit the the n-th enclosing for, while, or until loop\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "bugreport", "send bugreports to the shell's author(s)",
        bugreport_builtin,       /* non-POSIX */
        "%%",
        "builtin", "print the list of shell builtin utilities",
        builtin_builtin,       /* non-POSIX */
        "%% [-hvsra] [name [args]]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "caller", "print the context of any active subroutine call",
        caller_builtin,       /* non-POSIX */
        "%% [n]",
        "cd", "change the working directory",
        cd_builtin,       /* POSIX */
        "%% [-h] [-nplv] [-L|-P] [directory]\n"
        "%% [-h] [-nplv] -",
        "command", "execute a simple command",
        command_builtin,       /* POSIX */
        "%% [-hp] command_name [argument ...]\n"
        "%% [-hp][-v|-V] command_name",
        "continue", "continue for, while, or until loop",
        continue_builtin,       /* POSIX */
        "%% [n]",
        "n           return to the top of the n-th enclosing for, while, or until loop\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
#if 0
    {
        "coproc", "execute commands in a coprocess (subshell with pipe)",
        coproc_builtin,       /* non-POSIX */
        "%% command [redirections]",
#endif
        "declare", "declare variables and give them attributes",
        declare_builtin,       /* non-POSIX */
        "%% [-hvfFgrxlut] [-p] [name=[value]...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "dirs", "display the contents of the directory stack",
        dirs_builtin,       /* non-POSIX */
        "%% [-hclpv] [+N | -N]\n"
        "%% -S|-L [filename]",
        "disown", "not send HUP signal to jobs",
        disown_builtin,       /* non-POSIX */
        "%% [-arsv] [-h] [job...]",
        "dump", "dump memory values of the passed arguments",
        dump_builtin,       /* non-POSIX */
        "%% [-hv] [argument ...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "echo", "echo arguments",
        echo_builtin,       /* non-POSIX */
        "%% [-enE] [args...]",
        "enable", "enable/disable shell builtins",
        enable_builtin,       /* non-POSIX */
        "%% [-ahnprsv] [name ...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "eval", "construct command by concatenating arguments",
        eval_builtin,       /* POSIX */
        "%% [argument...]",
        "argument    construct a command by concatenating arguments together\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "exec", "execute commands and open, close, or copy file descriptors",
        exec_builtin,       /* POSIX */
        "%% [-cl] [-a name] [command [argument...]]",
        "command     path to the command to be executed\n"
        "argument    execute command with arguments and open, close, or copy file descriptors\n\n"
        "Options:\n"
        "  -a        set argv[0] to 'name' instead of 'command'\n"
        "  -c        clear the environment before performing exec\n"
        "  -l        place a dash in front of argv[0], just as the login utility does\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "exit", "exit the shell",
        exit_builtin,       /* POSIX */
        "%% [n]",
        "n           exit the shell returning n as the exit status code\n\n",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "export", "set the export attribute for variables",
        export_builtin,       /* POSIX */
        "%% [-hvn] [-p] [name[=word]...]",
        "name        set the export attribute to the variable name\n"
        "word        set the value of variable name to word\n\n"
        "Options:\n"
        "  -n        remove the export attribute of passed variable names\n"
        "  -p        print the names and values of all exported variables\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,
    },
    {
        "false", "return false value",
        false_builtin,       /* POSIX */
        "%%",
        "fc", "process the command history list",
        fc_builtin,       /* POSIX */
        "%% [-hvr] [-e editor] [first [last]]\n"
        "%% -l [-hvnr] [first [last]]\n"
        "%% -s [-hv] [old=new] [first]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "fg", "run jobs in the foreground",
        fg_builtin,       /* POSIX */
        "%% [-hv] [job_id]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "getopts", "parse utility options",
        getopts_builtin,       /* POSIX */
        "%% optstring name [arg...]",
        "glob", "echo arguments, delimited by NULL characters",
        glob_builtin,       /* non-POSIX */
        "%% [-eE] [args...]",
        "hash", "remember or report utility locations",
        hash_builtin,       /* POSIX */
        "%% [-hvld] [-p path] [-r] utility...\n"
        "%% -a",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "help", "show help for builtin utilities and commands",
        help_builtin,       /* non-POSIX */
        "%% [-ds] [command]",
        "  -d        print a short description for each command\n"
        "  -m        print a manpage-like help page for each command\n"
        "  -s        print the usage or synopsis for each command\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "history", "print command history",
        history_builtin,       /* non-POSIX */
        "%% [-hR] [n]\n"
        "%% -c\n"
        "%% -d offset\n"
        "%% -d start-end\n"
        "%% [-anrwSL] [filename]\n"
        "%% -ps arg ...",
        "hup", "run a command, receiving SIGHUP",
        hup_builtin,       /* non-POSIX */
        "%% [command]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "jobs", "display status of jobs in the current session",
        jobs_builtin,       /* POSIX */
        "%% [-hnrsv] [-l|-p] [job_id...]\n"
        "%% -x command [argument...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "kill", "terminate or signal processes",
        kill_builtin,       /* POSIX */
        "%% [-hv]\n"
        "%% -s signal_name pid...\n"
        "%% -n signal_number pid...\n"
        "%% [-l|-L] [exit_status]\n"
        "%% [-signal_name] pid...\n"
        "%% [-signal_number] pid...",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "let", "evaluate arithmetic expressions",
        let_builtin,       /* non-POSIX */
        "%% [args...]",
    { 
        "local", "define local variables",
        local_builtin,       /* non-POSIX */
        "%% name[=word] ...",
        "name        set the local attribute to the variable 'name'\n"
        "word        set the value of the variable named 'name' to 'word'\n\n"
        "Options:\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "logout", "exit a login shell",
        logout_builtin,       /* non-POSIX */
        "%% [n]",
        "n           exit a login shell returning n as the exit status code\n\n",
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
        "mailcheck", "check for mail at specified intervals",
        mailcheck_builtin,       /* non-POSIX */
        "%% [-hvq]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "memusage", "show the shell's memory usage",
        memusage_builtin,       /* non-POSIX */
        "%% arg...",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "newgrp", "change to a new group",
        newgrp_builtin,       /* POSIX */
        "%% [-hv] [-l] [group]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "nice", "run a command with the given priority",
        nice_builtin,       /* non-POSIX */
        "%% [+n] [command]\n"
        "%% [-n] [command]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "nohup", "run a command, ignoring SIGHUP",
        hup_builtin,       /* non-POSIX */
        "%% [command]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "notify", "notify immediately when jobs change status",
        notify_builtin,       /* non-POSIX */
        "%% [job ...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "popd", "pop directories off the stack and cd to them",
        popd_builtin,       /* non-POSIX */
        "%% [-chlnpsv] [+N | -N]",
        "printenv", "print the names and values of environment variables",
        printenv_builtin,       /* non-POSIX */
        "%% [-hv0] [name ...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "pushd", "push directories on the stack and cd to them",
        pushd_builtin,       /* non-POSIX */
        "%% [-chlnpsv] [+N | -N | dir]",
        "pwd", "return working directory name",
        pwd_builtin,       /* POSIX */
        "%% [-hv] [-L|-P]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "read", "read a line from standard input",
        read_builtin,       /* POSIX */
        "%% [-hv] [-rs] [-d delim] [-nN num] [-t secs] [-u fd] [-p msg] [var...]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "readonly", "set the readonly attribute for variables",
        readonly_builtin,       /* POSIX */
        "%% name[=word]...\n"
        "%% -p",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "repeat", "repeat a command count times",
        repeat_builtin,       /* non-POSIX */
        "%% [-hv] count command",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "return", "return from a function or dot script",
        return_builtin,       /* POSIX */
        "%% [n]",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "set", "set or unset options and positional parameters",
        set_builtin,       /* POSIX */
        "%% [-abCdeEfhHkmnprtTuvx] [-o option ...] [argument...]\n"
        "%% [+abCdeEfhHkmnprtTuvx] [+o option ...] [argument...]\n"
        "%% -- [argument...]\n"
        "%% -o\n"
        "%% +o",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "setenv", "set environment variable values",
        setenv_builtin,       /* non-POSIX */
        "%% [-hv] [name[=value] ...]",
        "name        the environment variable to set\n"
        "value       the value to give to name, NULL if no value is given\n\n"
        "This utility sets both the environment variable and the shell variable with\n"
        "the same name. If no arguments are given, it prints the names and values of\n"
        "all the set environment variables.\n\n"
        "Options:\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "setx", "set/unset optional (extra) shell options",
        setx_builtin,       /* non-POSIX */
        "%% [-hvpsuqo] option",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "shift", "shift positional parameters",
        shift_builtin,       /* POSIX */
        "%% [n]",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "shopt", "set/unset optional (extra) shell options",
        setx_builtin,       /* non-POSIX */
        "%% [-hvpsuqo] option",
        "For an explanation of all the options and arguments, run `help setx`\n",
        "source", "execute commands in the current environment",
        source_builtin,       /* POSIX */
        "%% [-hv] file",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,  /* print only the -v option */
        "stop", "stop background jobs",
        stop_builtin,       /* non-POSIX */
        "%% [-hv] job",
        "job         the background job to stop\n\n"
        "Options:\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "suspend", "suspend execution of the shell",
        suspend_builtin,       /* non-POSIX */
        "%% [-fhv]",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "test", "test file attributes and compare strings",
        test_builtin,       /* POSIX */
        "%% -option expression",
        "For the list of options and their meanings, run `help [[`\n",
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
    },
    {
        "times", "write process times",
        times_builtin,       /* POSIX */
        "%%",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "trap", "trap signals",
        trap_builtin,       /* POSIX */
        "%% [-hvlp] n [condition...]\n"
        "%% [-hvlp] [action condition...]",
        "   EXIT     set/reset the exit trap\n"
        "   ERR      set/reset the error trap\n"
        "   CHLD     set/reset the child exit trap\n"
        "   DEBUG    set/reset the debug trap\n"
        "   RETURN   set/reset the return (from function or script) trap\n"
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,
    },
    {
        "true", "return true value",
        true_builtin,       /* POSIX */
        "%%",
        "",
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
    },
    {
        "type", "write a description of command type",
        type_builtin,       /* POSIX */
        "%% name...",
        "command     the name of a command or function for which to write description\n\n"
        "Options:\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "typeset", "declare variables and give them attributes",
        declare_builtin,       /* non-POSIX */
        "%% [-hvfFgrxlut] [-p] [name=[value]...]",
        "For an explanation of all the options and arguments, run `help declare`\n",
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "ulimit", "set or report shell resource limits",
        ulimit_builtin,       /* POSIX */
        "%% [-h] [-acdflmnpstuv] [limit]",
        "limit       the new limit for the given resource\n\n"
        "Options:\n"
        "  -a        report all current limits\n"
        "  -c        set/report the maximum size of core files created\n"
        "  -d        set/report the maximum size of a process's data segment\n"
        "  -e        set/report the maximum nice value (scheduling priority)\n"
        "  -f        set/report the maximum size of files written by a process\n"
        "  -i        set/report the maximum number of pending signals\n"
        "  -l        set/report the maximum size of memory a process may lock\n"
        "  -m        set/report the maximum resident set size (RSS)\n"
        "  -n        set/report the maximum number of open file descriptors\n"
        "  -p        set/report the pipe buffer size in kbytes\n"
        "  -q        set/report the maximum number of kbytes in POSIX message queues\n"
        "  -r        set/report the maximum real-time priority\n"
        "  -s        set/report the maximum stack size\n"
        "  -t        set/report the maximum amount of cpu time (seconds)\n"
        "  -u        set/report the maximum number of user processes\n"
        "  -v        set/report the size of virtual memory\n"
        "  -x        set/report the maximum number of file locks\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_ENABLED,  /* print only the -h option */
    },
    {
        "umask", "get or set the file mode creation mask",
        umask_builtin,       /* POSIX */
        "%% [-hvp] [-S] [mask]",
        "mask        the new file mode creation mask\n\n"
        "Options:\n"
        "  -S        produce symbolic output\n"
        "  -p        print output that can be reused as shell input\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "unalias", "remove alias definitions",
        unalias_builtin,       /* POSIX */
        "%% [-hv] alias-name...\n"
        "%% [-hv] -a",
        "alias-name  the name of an alias to be removed\n\n"
        "Options:\n"
        "  -a        remove all alias definitions\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "unlimit", "remove limits on system resources",
        unlimit_builtin,       /* non-POSIX */
        "%% [-hHfSv] [limit ...]\n"
        "%% [-HS] -a",
        "limit       the name of a system resource, which can be one of the following:\n"
        "  core    , -c        the maximum size of core files created\n"
        "  data    , -d        the maximum size of a process's data segment\n"
        "  nice    , -e        the maximum nice value (scheduling priority)\n"
        "  file    , -f        the maximum size of files written by a process\n"
        "  signal  , -i        the maximum number of pending signals\n"
        "  mlock   , -l        the maximum size of memory a process may lock\n"
        "  rss     , -m        the maximum resident set size (RSS)\n"
        "  fd      , -n        the maximum number of open file descriptors\n"
        "  buffer  , -p        the pipe buffer size in kbytes\n"
        "  message , -q        the maximum number of kbytes in POSIX message queues\n"
        "  rtprio  , -r        the maximum real-time priority\n"
        "  stack   , -s        the maximum stack size\n"
        "  cputime , -t        the maximum amount of cpu time (seconds)\n"
        "  userproc, -u        the maximum number of user processes\n"
        "  virtmem , -v        the size of virtual memory\n"
        "  flock   , -x        the maximum number of file locks\n"
        "  all     , -a        all the above\n\n"
        "Options and limit names must be passed separately. To remove all hard limits, invoke\n"
        "either of the following commands:\n"
        "  unlimit -H -a\n"
        "  unlimit -H all\n\n"
        "Options:\n"
        "  -a        remove limits on all resources\n"
        "  -f        ignore errors\n"
        "  -H        remove hard limits (only root can do this)\n"
        "  -S        remove soft limits (the default)\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
        "unset", "unset values and attributes of variables and functions",
        unset_builtin,       /* POSIX */
        "%% [-fv] name...",
        BUILTIN_SPECIAL_BUILTIN | BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
    },
    {
        "unsetenv", "unset environment variable values",
        unsetenv_builtin,       /* non-POSIX */
        "%% [-hv] [name ...]",
        "name        the environment variable to unset\n\n"
        "This utility unsets both the environment variable and the shell variable with\n"
        "the same name. If no arguments are given, nothing is done.\n\n"
        "Options:\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    {
        "ver", "show shell version",
        ver_builtin,       /* non-POSIX */
        "%%",
        "",
    {
        "wait", "await process completion",
        wait_builtin,       /* POSIX */
        "%% [-hfnv] [pid...]",
        "pid...      process ID or Job ID to wait for\n\n"
        "Options:\n"
        "  -f        force jobs/processes to exit\n"
        "  -n        wait for any job or process\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_PRINT_VOPTION | BUILTIN_ENABLED,
    },
    { 
        "whence", "write a description of command type",
        whence_builtin,       /* non-POSIX */
        "%% [-afhpv] name...",
        "name        the name of a command or function for which to write description\n\n"
        "Options:\n"
        "  -a        output all possible interpretations of the command\n"
        "  -f        don't search for functions\n"
        "  -p        perform path search even if command is an alias, keyword or function name\n"
        "  -v        verbose output (the default)\n",
        BUILTIN_PRINT_HOPTION | BUILTIN_ENABLED,  /* print only the -h option */
    },
    {
        NULL, NULL, NULL, NULL, NULL, 0,
    },
 * if cmd is a builtin utility, return the utility's struct builtin_s, or
 * NULL otherwise.
struct builtin_s *is_builtin(char *cmd)
    if(!cmd)
    {
        return NULL;
    }

    struct builtin_s *u = shell_builtins;
    for( ; u->name; u++)
    {
        if(strcmp(u->name, cmd) == 0)
        {
            return u;
        }
    }
    return NULL;
struct builtin_s *is_enabled_builtin(char *cmd)
    struct builtin_s *builtin = is_builtin(cmd);

    if(builtin && flag_set(builtin->flags, BUILTIN_ENABLED))
        return builtin;
    
    return NULL;
}


/*
 * return 1 if the given cmd name is a special builtin utility, 0 otherwise.
 */
struct builtin_s *is_special_builtin(char *cmd)
{
    struct builtin_s *builtin = is_builtin(cmd);

    if(builtin && flag_set(builtin->flags, BUILTIN_SPECIAL_BUILTIN))
        return builtin;
    
    return NULL;
}


/*
 * return 1 if the given cmd name is a regular builtin utility, 0 otherwise.
 */
struct builtin_s *is_regular_builtin(char *cmd)
{
    struct builtin_s *builtin = is_builtin(cmd);

    if(builtin && !flag_set(builtin->flags, BUILTIN_SPECIAL_BUILTIN))
        return builtin;
    
    return NULL;
}


/*
 * search the list of builtin utilities looking for a utility whose name matches
 * argv[0] of the passed **argv list.. if found, we execute the builtin utility,
 * passing it the **argv list as if we're executing an external command, then 
 * we return 1.. otherwise, we return 0 to indicate we failed in finding a 
 * builtin utility whose name matches the command we are meant to execute (that
 * is, argv[0]).. if the 'special_utility' flag is non-zero, we search for a special
 * builtin utility with the given name, otherwise we search for a regular one.
 * 
 * returns 1 if the builtin utility is executed, otherwise 0.
 */
int do_builtin(int argc, char **argv, int special_utility)
{
    char   *cmd = argv[0];
    struct builtin_s *utility = special_utility ?
                                is_special_builtin(cmd) : is_regular_builtin(cmd);
                                  
    if(utility && flag_set(utility->flags, BUILTIN_ENABLED))
    {
        int (*func)(int, char **) = (int (*)(int, char **))utility->func;
        int status = do_exec_cmd(argc, argv, NULL, func);
        set_internal_exit_status(status);
        return 1;
    }

 * execute a builtin utility internally from within the shell.. we first reset
 * $OPTIND, so that the builtin utility can call getopts() to parse its options..
 * we then call the utlity, and reset $OPTIND to its previous value so that user
 * commands won't be distrubted by our execution of the utility.
 * 
 * returns the exit status of the executed utility.
int do_builtin_internal(int (*builtin)(int, char **), int argc, char **argv)
    /*
     * all builtins (except getopts) may change $OPTIND, so save it and reset
     * its value to NULL.
     */
    if(builtin != getopts_builtin)
        save_OPTIND();

    /* execute the builtin */
    int res = builtin(argc, argv);

    /* reset $OPTIND to its previous value */
    if(builtin != getopts_builtin)
        reset_OPTIND();
    return res;
 * this function does what its name says it does.
void disable_nonposix_builtins(void)
    BUGREPORT_BUILTIN.flags &= ~BUILTIN_ENABLED;
    BUILTIN_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    CALLER_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    COPROC_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    DECLARE_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    DIRS_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    DISOWN_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    DUMP_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    ECHO_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    GLOB_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    HISTORY_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    HUP_BUILTIN.flags       &= ~BUILTIN_ENABLED;
    LET_BUILTIN.flags       &= ~BUILTIN_ENABLED;
    LOCAL_BUILTIN.flags     &= ~BUILTIN_ENABLED;
    LOGOUT_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    MAILCHECK_BUILTIN.flags &= ~BUILTIN_ENABLED;
    MEMUSAGE_BUILTIN.flags  &= ~BUILTIN_ENABLED;
    NICE_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    NOHUP_BUILTIN.flags     &= ~BUILTIN_ENABLED;
    NOTIFY_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    POPD_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    PRINTENV_BUILTIN.flags  &= ~BUILTIN_ENABLED;
    PUSHD_BUILTIN.flags     &= ~BUILTIN_ENABLED;
    REPEAT_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    SETENV_BUILTIN.flags    &= ~BUILTIN_ENABLED;
    SETX_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    SHOPT_BUILTIN.flags     &= ~BUILTIN_ENABLED;
    STOP_BUILTIN.flags      &= ~BUILTIN_ENABLED;
    SUSPEND_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    TYPESET_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    UNLIMIT_BUILTIN.flags   &= ~BUILTIN_ENABLED;
    UNSETENV_BUILTIN.flags  &= ~BUILTIN_ENABLED;
    VER_BUILTIN.flags       &= ~BUILTIN_ENABLED;
    WHENCE_BUILTIN.flags    &= ~BUILTIN_ENABLED;

    /*
     * we won't disable the 'enable' builtin so the user can selectively enable builtins
     * when they're in the POSIX mode. if you insist on disabling ALL non-POSIX builtins,
     * uncomment the next line.
     */

    /*
     * the 'help' builtin should also be available, even in POSIX mode. if you want to
     * disable it in POSIX mode, uncomment the next line.
     */
}


/*
 * helper function to print the list of builtin utilities.. the function
 * has a 'special_list' argument, which contains 1 if the list to be printed is
 * the special builtin utilities list, 0 if its the regular builtin utilities list.
 */
void __list(int special_list)
{
    struct builtin_s *u = shell_builtins;
    for( ; u->name; u++)
        int is_special = flag_set(u->flags, BUILTIN_SPECIAL_BUILTIN);
        if(special_list == is_special)
            printf("  %s%*s%s\n", u->name, (int)(10-strlen(u->name)), " ", u->explanation);
            __list(1);
            __list(0);
            __list(1);
            __list(0);
    int which = 0;

                print_help(argv[0], &BUILTIN_BUILTIN, 0);
                if(which == 'r' || which == 'a')
                {
                    /* list all (special + regular) utilities */
                    which = 'a';
                }
                else
                {
                    /* list only special utilities */
                    which = 's';
                }
                break;
                if(which == 's' || which == 'a')
                {
                    /* list all (special + regular) utilities */
                    which = 'a';
                }
                else
                {
                    /* list only regular utilities */
                    which = 'r';
                }
                break;
                which = 'a';
                break;

    if(which || v >= argc)
        list(which ? which : 'a');
    if(!do_builtin(argc, &argv[v], 1))
        if(!do_builtin(argc, &argv[v], 0))
            PRINT_ERROR("%s: not a shell builtin: %s\n", UTILITY, argv[v]);