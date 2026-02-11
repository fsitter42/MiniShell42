#include "../shinimell/minishell.h"

void printh(void)
{
		HIST_ENTRY **h;
		int i = 0;

		h = history_list();
		while (h[i])
		{
				printf("%s\n", h[i]->line);
				i++;
		}
}

int count_pipes(char *prompt)
{
		int i = 0;
		int pipes = 1;
		while (prompt[i])
		{
				if (prompt[i] == '|')
						pipes++;
				i++;
		}
		return (pipes);
}

int handle_pipes(char *prompt, int pipe_count)
{
		while (1)
			return (0);
}

int main()
{
	int pipes;	
	while (1)
	{
    	char *prompt = readline("minishell$ ");
		if (!strcmp(prompt, "exit"))
		{
				free(prompt);
				prompt = NULL;
				break;
		}
		add_history(prompt);
		pipes = count_pipes(prompt);
    	free(prompt);
    	prompt = NULL;
    	printf("%i\n", pipes);
	}
	printh();
	rl_clear_history();
}

// compile with -lreadline
