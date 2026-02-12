#include "../minishell/minishell.h"
#include "libft.h"

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
		int i = 0;
		int index = 1;
		int j;
		char **pipes = ft_split(prompt, '|');
		if (!pipes)
			return (1);
		while (pipes[i])
		{
			//printf("Pipe %i, %s\n",index + i, pipes[i]);
			char **words = ft_split(pipes[i], ' ');
			if (!words)
					return (free(pipes), 1); // free each pipe missing
			j = 0;
			while (words[j])
			{
					printf("Pipe %i, Word %i: x%sx\n",index + i, index + j, words[j]);
					j++;
			}
			i++;
		}
		return (0);
}

int main()
{
	int x_pipes;	
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
		x_pipes = count_pipes(prompt);
		handle_pipes(prompt, x_pipes);
    	free(prompt);
    	prompt = NULL;
	}
	printh();
	rl_clear_history();
}

// compile with -lreadline
