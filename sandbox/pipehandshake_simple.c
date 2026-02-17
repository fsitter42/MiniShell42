#include "../minishell/minishell.h"

int main()
{
    int my_pipe[2];
    char dummy;

    if (pipe(my_pipe) == -1)
        exit (-1);

    pid_t pid = fork();

    if (pid == 0)
    {
        close(my_pipe[1]);
        printf("Child: Waiting for parent.\n");

        read(my_pipe[0], &dummy, 1);

        printf("Child: Parent is done.. starting now\n");
        
        sleep(2);
        close(my_pipe[0]);
        exit(0);
    }
    else
    {
        close(my_pipe[0]);
        printf("Parent: Im doing someting important the child needs.\n");
        sleep(2);
        printf("Parent: Im done. Lets go child.\n");

        close(my_pipe[1]); // so here write(pipe[1], "1", 1);
        waitpid(pid, NULL, 0);
        printf("Parent: Child is done.\n");
    }
    return (0);
}

