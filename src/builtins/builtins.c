        "["      , 1, "test file attributes and compare strings",
        test_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "[["     , 2, "test file attributes and compare strings",
        test_builtin, 9,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "alias"  , 5, "define or display aliases",
        alias_builtin, 1,       /* POSIX */
        "bg"     , 2, "run jobs in the background",
        bg_builtin, 1,       /* POSIX */
        "bugreport", 9, "send bugreports to the shell's author(s)",
        bugreport_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "builtin" , 7, "print the list of shell builtin utilities",
        builtin_builtin, 1,       /* non-POSIX */
        "caller"  , 6, "print the context of any active subroutine call",
        caller_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "cd"     , 2, "change the working directory",
        cd_builtin, 2,       /* POSIX */
        "command", 7, "execute a simple command",
        command_builtin, 2,       /* POSIX */
        "coproc"  , 6, "execute commands in a coprocess (subshell with pipe)",
        coproc_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "declare" , 7, "declare variables and give them attributes",
        declare_builtin, 1,       /* non-POSIX */
        "dirs"    , 4, "display the contents of the directory stack",
        dirs_builtin, 2,       /* non-POSIX */
        "disown"  , 6, "not send HUP signal to jobs",
        disown_builtin, 1,       /* non-POSIX */
        "dump"    , 4, "dump memory values of the passed arguments",
        dump_builtin, 1,       /* non-POSIX */
        "echo"    , 4, "echo arguments",
        echo_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "enable"  , 6, "enable/disable shell builtins",
        enable_builtin, 1,       /* non-POSIX */
        "false"  , 5, "return false value",
        false_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "fc"     , 2, "process the command history list",
        fc_builtin, 3,       /* POSIX */
        "fg"     , 2, "run jobs in the foreground",
        fg_builtin, 1,       /* POSIX */
        "getopts", 7, "parse utility options",
        getopts_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
        "glob"   , 4, "echo arguments, delimited by NULL characters",
        glob_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
        "hash"   , 4, "remember or report utility locations",
        hash_builtin, 2,       /* POSIX */
        "help"    , 4, "show help for builtin utilities and commands",
        help_builtin, 1,       /* non-POSIX */
        "history" , 7, "print command history",
        history_builtin, 6,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "hup"    , 3, "run a command, receiving SIGHUP",
        hup_builtin, 1,       /* non-POSIX */
        "jobs"   , 4, "display status of jobs in the current session",
        jobs_builtin, 2,       /* POSIX */
        "kill"   , 4, "terminate or signal processes",
        kill_builtin, 6,       /* POSIX */
        "let"    , 3, "evaluate arithmetic expressions",
        let_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "mailcheck", 9, "check for mail at specified intervals",
        mailcheck_builtin, 1,       /* non-POSIX */
        "memusage", 8, "show the shell's memory usage",
        memusage_builtin, 1,       /* non-POSIX */
        "newgrp" , 6, "change to a new group",
        newgrp_builtin, 1,       /* POSIX */
        "nice"   , 4, "run a command with the given priority",
        nice_builtin, 2,       /* non-POSIX */
        "nohup"  , 5, "run a command, ignoring SIGHUP",
        hup_builtin, 1,       /* non-POSIX */
        "notify" , 6, "notify immediately when jobs change status",
        notify_builtin, 1,       /* non-POSIX */
        "popd"   , 4, "pop directories off the stack and cd to them",
        popd_builtin, 1,       /* non-POSIX */
        "printenv", 8, "print the names and values of environment variables",
        printenv_builtin, 1,       /* non-POSIX */
        "pushd"  , 5, "push directories on the stack and cd to them",
        pushd_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "pwd"    , 3, "return working directory name",
        pwd_builtin, 1,       /* POSIX */
        "read"   , 4, "read a line from standard input",
        read_builtin, 1,       /* POSIX */
        "setenv" , 6, "set environment variable values",
        setenv_builtin, 1,       /* non-POSIX */
        "stop"   , 4, "stop background jobs",
        stop_builtin, 1,       /* non-POSIX */
        "test"   , 4, "test file attributes and compare strings",
        test_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
        "true"   , 4, "return true value",
        true_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,       /* don't print neither the -v nor the -h options */
        "type"   , 4, "write a description of command type",
        type_builtin, 1,       /* POSIX */
        3 | BUILTIN_ENABLED,   /* print both -v and -h options */
        "ulimit" , 6, "set or report shell resource limits",
        ulimit_builtin, 1,       /* POSIX */
        "umask"  , 5, "get or set the file mode creation mask",
        umask_builtin, 1,       /* POSIX */
        "unalias", 7, "remove alias definitions",
        unalias_builtin, 2,       /* POSIX */
        "unlimit", 7, "remove limits on system resources",
        unlimit_builtin, 1,       /* non-POSIX */
        "unsetenv", 8, "unset environment variable values",
        unsetenv_builtin, 1,       /* non-POSIX */
        "ver"     , 3, "show shell version",
        ver_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "wait"   , 4, "await process completion",
        wait_builtin, 1,       /* POSIX */
        "whence" , 6, "write a description of command type",
        whence_builtin, 1,       /* non-POSIX */
        "break"   , 5, "exit from for, while, or until loop",
        break_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        ":"       , 1, "expand arguments (the null utility)",
        colon_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "continue", 8, "continue for, while, or until loop",
        continue_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "."       , 1, "execute commands in the current environment",
        dot_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "eval"    , 4, "construct command by concatenating arguments",
        eval_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "exec"    , 4, "execute commands and open, close, or copy file descriptors",
        exec_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "exit"    , 4, "exit the shell",
        exit_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "export"  , 6, "set the export attribute for variables",
        export_builtin, 1,       /* POSIX */
        "local"   , 5, "define local variables",
        local_builtin, 1,       /* non-POSIX */
        "logout"  , 6, "exit a login shell",
        logout_builtin, 1,       /* non-POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "readonly", 8, "set the readonly attribute for variables",
        readonly_builtin, 2,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "repeat"  , 6, "repeat a command count times",
        repeat_builtin, 1,       /* non-POSIX */
        "return"  , 6, "return from a function or dot script",
        return_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "set"     , 3, "set or unset options and positional parameters",
        set_builtin, 5,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "setx"    , 4, "set/unset optional (extra) shell options",
        setx_builtin, 1,       /* non-POSIX */
        "shift"   , 5, "shift positional parameters",
        shift_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "source"  , 6, "execute commands in the current environment",
        source_builtin, 1,       /* POSIX */
        "suspend" , 7, "suspend execution of the shell",
        suspend_builtin, 1,       /* non-POSIX */
        "times"   , 5, "write process times",
        times_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
        "trap"    , 4, "trap signals",
        trap_builtin, 2,       /* POSIX */
        "unset"   , 5, "unset values and attributes of variables and functions",
        unset_builtin, 1,       /* POSIX */
        BUILTIN_ENABLED,      /* don't print neither the -v nor the -h options */
int builtin_builtin(int argc, char **argv)