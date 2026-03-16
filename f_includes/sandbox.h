#ifndef SANDBOX_H
# define SANDBOX_H

# include "./Libfs/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h> //ioctl
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>    //tgetent
# include <termios.h> //tcsetattr -+1
# include <unistd.h>

typedef struct s_envl	t_envl;
typedef struct s_data	t_data;
typedef struct s_cmd	t_cmd;

typedef struct s_envl
{
	char				*key;
	char				*val;
	t_envl				*next;

}						t_envl;

typedef struct s_data
{
	t_cmd				*cmds;
	char				**envp_ori;
	t_envl				*envp_lst;
	char				**envp_updated;
}						t_data;

typedef struct s_cmd
{
	char *cmd; // filled by bert
	int					is_builtin;
	char **args; // filled by bert
	char				*path;
	char *infile; // filled by bert
	int					in_fd;
	char *outfile; // filled by bert
	int					out_fd;
	int append; // filled by bert
	struct s_cmd		*next;
}						t_cmd;

#endif