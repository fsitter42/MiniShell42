#include "../minishell/minishell.h"

int main(void)
{
    char *test = getenv("yyy");
    printf("%s\n", test);
}