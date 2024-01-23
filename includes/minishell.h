#ifndef MINI_SHELL_H
#define MINI_SHELL_H


#include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
#include <stdio.h>              // printf
#include <stdlib.h>             // malloc, free, exit
#include <unistd.h>             // access, open, read, close, dup, dup2, pipe, getcwd, chdir
#include <sys/types.h>          // fork, wait, waitpid, wait3, wait4
#include <sys/wait.h>           // wait, waitpid, wait3, wait4
#include <signal.h>             // signal, sigaction, sigemptyset, sigaddset, kill
#include <sys/stat.h>           // stat, lstat, fstat, unlink, execve
#include <dirent.h>             // opendir, readdir, closedir
#include <string.h>             // strerror, perror
#include <termios.h>            // isatty, ttyname, tcsetattr, tcgetattr
#include <sys/ioctl.h>          // ioctl
#include <term.h>               // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <stdlib.h>             // getenv






#endif