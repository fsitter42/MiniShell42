#include "../shinimell/minishell.h"

int main()
{
    rl_clear_history();
    rl_on_new_line();
    rl_replace_line("hi, 1", 1);
    rl_redisplay();
    add_history("huwi");
    printf("done\n");
}

// compile with -lreadline