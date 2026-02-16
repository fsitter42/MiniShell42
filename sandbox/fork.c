#include "../minishell/minishell.h"

int main(int ac, char **av, char **envp)
{
    (void) ac;
    (void) av;
    pid_t id;

    id = fork();
    if (id != 0)
    {
        //printf("parent\n");
        
        printf("\nI am the parent the id of my child is: %i\n\n", id);
        wait(&id);
        printf("\nnow that my child is done is say good bye.\n");

    }
    else
    {

        printf("\nI am the child thats why my id for myself is: %i\n", id);
        id = getpid();
        // this if i pipe/ use files 
        // int fd = open("dir.txt", O_RDWR | O_CREAT, 0644);
        // if(fd == 1)
        // {
        //     dup2(fd, STDOUT_FILENO);
        //     close(fd);
        // }
        printf("Is uesed getpid(); now that i know my real id it is: %i\n\n", id);
        char *args[] = {"echo", "hello", "my friend", NULL};

        printf("i am now using execve to use echo\n");
        if (execve("/bin/echo", args, envp) == -1) // return -1 on fail, does not return on success;
        {
            printf("execve-fail\n");
            exit(-1);
        }
        printf("im dead\n"); // is never reached.
        exit(1);
    }
}
