#include "../minishell/minishell.h"
#include <sys/resource.h>
#include <sys/wait.h>

int main(int ac, char **av, char **envp)
{
    (void) ac;
    (void) av;
    pid_t id;
    int exitcode = 3072; // /256 = 12

    id = fork();
    if (id != 0)
    {
        //printf("parent\n");
        
        printf("\nI am the parent the id of my child is: %i\n\n", id);

        wait(&exitcode); // wait for any child
        //wait3(&exitcode, 0, NULL); // same as above

        //waitpid(id, &exitcode, 0); //wait for specific child
        //wait4(id, &exitcode, 0, NULL); // same as above

        printf("\nmy child exited with code: %i", (exitcode & 0xFF00) >> 8); // WEXITSTATUS(s)=(s & 0xFF00)>>8
        printf("\nmy child exited with code: %i", WEXITSTATUS(exitcode));
        //printf("\nmy child exited with code: %i", exitcode);
        printf("\nnow that my child is done is say good bye.\n");

    }
    else
    {

        printf("\nI am the child thats why my id for myself is: %i\n", id);
        id = getpid();

        printf("Is uesed getpid(); now that i know my real id it is: %i\n\n", id);

        // exit(42);
        char *args[] = {"echo", "hello", "my friend", NULL};

        printf("i am now using execve to use echo\n");
        if (execve("/bin/echos", args, envp) == -1) // return -1 on fail, does not return on success;
        {
            printf("execve-fail\n");
            exit(42);
        }
        printf("im dead\n"); // is never reached.
        exit(1);
    }
}

// this if i pipe/ use files 
// int fd = open("dir.txt", O_RDWR | O_CREAT, 0644);
// if(fd == 1)
// {
//     dup2(fd, STDOUT_FILENO);
//     close(fd);
// }