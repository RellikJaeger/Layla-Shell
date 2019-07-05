/* 
 *    Programmed By: Mohammed Isam Mohammed [mohammed_isam1984@yahoo.com]
 *    Copyright 2019 (c)
 * 
 *    file: unsetenv.c
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

/* macro definitions needed to use setenv() */
#define _POSIX_C_SOURCE 200112L

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../cmd.h"
#include "../symtab/symtab.h"
#include "../debug.h"

extern char **environ;

#define UTILITY             "unsetenv"


static inline void unset_entry(char *name)
{
    struct symtab_entry_s *entry = get_symtab_entry(name);
    if(name) symtab_entry_setval(entry, NULL);
}


/*
 * the unsetenv utility is a tcsh non-POSIX extension. bash doesn't have it.
 */

int __unsetenv(int argc, char **argv)
{
    /****************************
     * process the arguments
     ****************************/
    int v = 1, c;
    set_shell_varp("OPTIND", NULL);
    argi = 0;   /* args.c */
    while((c = parse_args(argc, argv, "hv", &v, 1)) > 0)
    {
        switch(c)
        {
            case 'h':
                print_help(argv[0], REGULAR_BUILTIN_UNSETENV, 1, 0);
                return 0;
                
            case 'v':
                printf("%s", shell_ver);
                return 0;
        }
    }
    /* unknown option */
    if(c == -1) return 2;

    if(v >= argc)
    {
        return 0;
    }
    
    int res = 0;
    for( ; v < argc; v++)
    {
        char *arg = argv[v];
        res = !unsetenv(arg) ? res : 1;
        unset_entry(arg);
    }
    return res;
}