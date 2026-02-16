#include "../minishell/minishell.h"
#include <curses.h>


#ifdef unix
static char term_buffer[2048];
#else
#define term_buffer 0
#endif

void init_terminal_data ()
{
  char *termtype = getenv ("TERM");
  int success;

  if (termtype == 0)
    printf ("Specify a terminal type with `setenv TERM <yourtype>'.\n");

  success = tgetent (term_buffer, termtype);
  if (success < 0)
    printf ("Could not access the termcap data base.\n");
  if (success == 0)
    printf ("Terminal type `%s' is not defined.\n", termtype);
}

int main()
{
    init_terminal_data();
    printf("done\n");
}

// compile with lncurses