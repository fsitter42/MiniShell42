#include "../minishell/minishell.h"

int main(int ac, char **av, char **envp)
{
    while(envp != NULL)
    {
        if (*envp != NULL)
            printf("%s\n", *envp++);
        else
            break ;
    }

    char *test = getenv("SHELL");
    printf("%s\n", test);
}