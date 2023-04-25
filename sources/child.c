/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 16:02:46 by zsyyida           #+#    #+#             */
/*   Updated: 2023/04/25 17:53:46 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_dupe_pipes(t_shell *shell, int i, int **fd)
{
	if (i < shell->nbr_pipes)
		dup2(fd[i][1], STDOUT_FILENO);
	if (i > 0)
		dup2(fd[i - 1][0], STDIN_FILENO);
	close_fds(shell, fd);
}

int	**open_pipes(t_shell *shell)
{
	int	i;
	int	**fd;

	i = 0;
	fd = (int **)ft_calloc(shell->nbr_pipes, sizeof(int *));
	while (i < shell->nbr_pipes)
		fd[i++] = (int *)ft_calloc(2, sizeof(int));
	i = -1;
	while (++i < shell->nbr_pipes)
		if (pipe (fd[i]) == -1 || fd[i][0] == -1 || fd[i][1] == -1)
			error(fd[i]);
	return (fd);
}

void	ft_fork(t_shell *shell, t_list_cmd *cmd_list, int *pid, int **fd)
{
	pid[cmd_list->cmd_nbr] = fork();
	if (pid[cmd_list->cmd_nbr] == -1)
		error_pipe(cmd_list->cmd);
	if (pid[cmd_list->cmd_nbr] == 0)
		child_process(shell, cmd_list, fd);
	else
		signal(SIGINT, SIG_IGN);
}

void	handle_error(t_list_cmd *cmd_list)
{
	struct stat	info;

	info.st_mode = 0;
	stat(cmd_list->cmd[0], &info);
	g_exit_status = 1;
	if (access(cmd_list->cmd[0], X_OK) == -1)
	{
		if (access(cmd_list->cmd[0], F_OK) == -1)
			nosuch_error(cmd_list->cmd[0], 127);
		else
			perm_error(126);
	}
	else if (S_ISDIR(info.st_mode))
	{
		ft_putstr_fd("ruhan_zahra_shell: ", 2);
		ft_putstr_fd(cmd_list->cmd[0], 2);
		ft_putstr_fd(": is a directory", 2);
		g_exit_status = 126;
	}
	else
		nosuch_error(cmd_list->cmd[0], 1);
	exit(g_exit_status);
}

void	child_process(t_shell *shell, t_list_cmd *cmd_list, int **fd)
{
	signal(SIGINT, handle_sig_child);
	signal(SIGQUIT, handle_sig_child);
	if (shell->nbr_pipes > 0)
		ft_dupe_pipes(shell, cmd_list->cmd_nbr, fd);
	exec_rdr(shell, cmd_list->rdr);
	g_exit_status = 0;
	if (check_builtins(cmd_list->cmd[0]) == 1)
		ft_builtins_child(shell, cmd_list);
	else if (cmd_list->path != NULL && cmd_list->cmd != NULL)
	{
		if (execve(cmd_list->path, cmd_list->cmd, shell->env) == -1)
			handle_error(cmd_list);
	}
	else if (!cmd_list->path && cmd_list->cmd)
	{
		ft_putstr_fd("ruhan_zahra_shell: ", 2);
		ft_putstr_fd(cmd_list->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
	}
	exit(g_exit_status);
}
