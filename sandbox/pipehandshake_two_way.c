#include "../minishell/minishell.h"

int main()
{
    int p_2_c[2];
    int c_2_p[2];

    char dummy;

    if (pipe(p_2_c) == -1)
        exit (-1);

    if (pipe(c_2_p) == -1)
        exit (-1);

    pid_t pid = fork();

    if (pid == 0)
    {
        close(p_2_c[1]); // closes the write
        close(c_2_p[0]); // close the read

        printf("Child: Waiting for parent.\n");
        read(p_2_c[0], &dummy, 1);
        printf("Child: Parent is done.. starting now\n");
        sleep(1);
        printf("Child: Im done..\n");
        close(c_2_p[1]);
        printf("Child: Waiting for parent.\n");
        read(p_2_c[0], &dummy, 1);
        printf("Child: thank you for telling me.\n");
        


        close(p_2_c[0]); // close read chanel
        sleep(2);
        exit(0);
    }
    else
    {
        close(p_2_c[0]); //close the read
        close(c_2_p[1]); //close the write

        printf("Parent: Im doing someting important the child needs.\n");
        sleep(2);
        printf("Parent: Im done. Lets go child.\n");
        write(p_2_c[1], "1", 1);

        printf("Parent: Im now waiting for my child to be ready.\n");
        read(c_2_p[0], &dummy, 1);
        printf("Parent: Ok ill start now.\n");
        sleep(1);
        printf("Parent: Done lets go home.\n");
        close(p_2_c[1]); // 2nd use
        waitpid(pid, NULL, 0);
        printf("Parent: Child is done.\n");
    }
    return (0);
}


// using write instead of close lets me reuse the pipe because
// close sends EOF
// write sends whatever i send