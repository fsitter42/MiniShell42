#include "../minishell/minishell.h"

int main(int ac, char **av, char **envp)
{
    system("echo hello > hello.txt");
}