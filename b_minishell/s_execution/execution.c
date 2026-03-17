/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slambert <slambert@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:03:13 by slambert          #+#    #+#             */
/*   Updated: 2026/03/17 17:15:16 by slambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
*   handles the readline portion of << until the delimiter is read
*   read lines are put in an hidden file called .heredoc_dump
*   after the reading is complete the in_fd of the command is
*   changed to the fd of this file
*   only handles one command and looks for the FIRST occurence of <<
*   TODO: correct bash-like behaviour if more than 1 heredoc per cmd
*   TODO: weird errors on readline function, is this the thing that
*   comes from readline itself?
*   TODO when input is << d no heredocing is happening
*   TODO the above case prints the heredoced stuff back to the user,
*   i have to do that too (find out in what cases exactly)
*   TODO while loop that does this for each occurence of heredoc
*   TODO cmd->heredoc kommt weg & da is nix mehr drin, die info is
*   jetzt in den entsprechenden t_redirs
*/
void do_heredoc_stuff(t_cmd *cmd)
{
    int     fd;
    char    *line;
    //TODO add cmd 
    int i = 0;

    char end = ft_itoa(i);
    fd = open("./.heredoc_dump", O_CREAT | O_TRUNC | O_WRONLY, 0600);
    //if (!fd)
    //afbhrgbkerjvberb
    while (1)
    {
        line = readline(">");
        //if (!line)
        //jkwerbvhjklwebvhjk
        if (line[0] == '\0')
        {
            free (line);
            write (fd, "\n", 1);
            continue;
        }
        if (ft_strlen(line) == ft_strlen(cmd->delimiter) && ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
            break ;
        //write line to file
        write (fd, (const void *) line, ft_strlen(line));
        free (line);
        write (fd, "\n", 1);
    }
    free (line);
    close (fd);

    //add_redir_node function (new node) new node function gibt es von frido
    //cmd->redirs added ein list element
    
    //cmd->in_fd = open ("./.heredoc_dump", O_RDONLY);
   // if (cmd->in_fd == -1)
   // {
        //hvkgehjkvggblwr
   // }
}

/*
 *  TODO 
 *  can only be called AFTER execution
 */ 
void remove_heredoc_file()
{

}

//TODO return return von f_exec_pipeline
//TODO nur einmal ausführen, keine loop
void eggsecute (t_data *data)
{
    while (data->cmds)
    {
        if (data->cmds->has_heredoc)
            do_heredoc_stuff(data->cmds);
        f_exec_pipeline(data, data->cmds);
        //f_exec_pipeline(data, cmd_list);
        //delete .heredoc_dump
        
        data->cmds = data->cmds->next;
    }
    //TODO cleanup all cmds
    //TODO here we have to delete the temp heredoc files
   
}
