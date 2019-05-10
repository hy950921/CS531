#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// #define DEBUG 1

#define MAX_LINE 4096
#define MAX_WORDS MAX_LINE / 2

struct sigaction old_action;

int cs531_system(char *s);

void signal_INT(int signal_no);

void signal_CHLD(int signo, siginfo_t *info, void *context);

void reg_signal_handler(int signal, int flag, void *handler, int save_old_act);

void tokenize(char *line, char **words, int *nwords);

int main(int argc, char *argv[]) {
    int status = 0;
    int fd, fd1;

    if (argc == 2) {
        fd = open("/dev/tty", O_RDWR);
        status = cs531_system(argv[1]);
        printf("Parent process is sleeping...\n");
        // #if (!DEBUG)
        sleep(5);
        // #endif
        fd1 = dup(fd);
        close(fd1);
        close(fd);
    } else {
        fprintf(stderr, "Wrong parameter number, expected 1, recieve %d.\n",
                argc - 1);
        fprintf(stderr, "Usage: ./xxx \"ls -l\"\n");
        exit(1);
    }

    return status;
}

int cs531_system(char *s) {
    char buffer[MAX_LINE], *words[MAX_WORDS];
    int pid = 1, status = 0, ret, nwords = 0;

#if (DEBUG)
    int i;
    printf("%s\n", s);
#endif

    tokenize(s, words, &nwords);

#if (DEBUG)
    for (i = 0; i < nwords; ++i) {
        printf("%s|", words[i]);
    }
    printf("\n");
#endif

    if (nwords == 0) {
        return 0;
    }

    pid = fork();
    if (pid == 0) {  // child
        strcpy(buffer, "./");
        strcat(buffer, words[0]);
        if ((ret = execv(buffer, words)) ==
            -1) {  // try exec program in the working dir
            if ((ret = execvp(words[0], words)) ==
                -1) {  // try exec program in system PATH
                fprintf(stderr, "Cmd not found: %s.\n", words[0]);

                return 0;
            }
        }
        // reg_signal_handler(SIGSTOP, SA_NODEFER, signal_STOP, 0);
    } else if (pid > 0) {
        reg_signal_handler(SIGINT, SA_RESETHAND | SA_RESTART, signal_INT, 1);
        // printf("sig stop\n");
        // signal(SIGCHLD, signal_STOP);
        reg_signal_handler(SIGCHLD, SA_SIGINFO | SA_RESTART, signal_CHLD, 0);

        while (wait(&status) != -1) {  // print_exit(status);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                return WTERMSIG(status);
            } else if (WIFSTOPPED(status)) {
                return WSTOPSIG(status);
            } else if (WIFCONTINUED(status)) {
                continue;
            }
        }
    } else if (pid < 0) {
        execlp("rev", "rev", 0);
    }
    return -1;
}

void tokenize(char *line, char **words, int *nwords) {
    *nwords = 1;

    for (words[0] = strtok(line, " \t\n"); /* the first cut */
         (*nwords < MAX_WORDS) &&
         (words[*nwords] = strtok(
              NULL, " \t\n")); /* rest cuts, words[*nwords] remember the
                                  place where each parameters start, exit when
                                  reach the MAX_WORDS or no more parameters */
             *nwords = *nwords + 1)
        ; /* empty body */
    return;
}

void reg_signal_handler(int signal, int flag, void *handler, int save_old_act) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = flag;
    if (save_old_act) {
        sigaction(signal, &action, 0);
    } else {
        sigaction(signal, &action, &old_action);
    }
}

void signal_INT(int signal_no) {
    // printf("INT\n");
    // restore the old action
    sigaction(SIGINT, &old_action, 0);
    // printf("INT\n");
}

void signal_CHLD(int signo, siginfo_t *info, void *context) {
    // printf("%d %d %d\n", info->si_status, info->si_signo, info->si_code);
    if (info->si_status == SIGSTOP) {
        printf("stopped by signal 17\n");
    } else if (info->si_status == SIGCONT) {  // SIGCONT SIGSTOP
        // nothing todo here
    }
}
