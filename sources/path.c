/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 14:25:13 by zsyyida           #+#    #+#             */
/*   Updated: 2023/04/23 13:59:09 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int access(const char *pathname, int how);
// pathname is the name of the file whose accessibility you want to test.
// The how argument indicates the access modes you want to test.
// The following symbols are defined in the unistd.h header file for
// use in the how argument:
// F_OK
// Tests whether the file exists.

char	*check_path(char *cmd, char **paths)
{
	int		i;
	char	*path;

	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin_ft(paths[i], cmd);
		free(paths[i]);
		paths[i] = path;
		if (access(paths[i], F_OK) == 0)
			return (ft_strdup_ft(paths[i]));
	}
	return (NULL);
}

char	**append_path(t_shell *shell)
{
	char	**paths;
	int		i;
	char	*path;

	path = ft_getenv(shell, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free (path);
	i = -1;
	while (paths[++i])
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
		{
			path = ft_strjoin_ft (paths[i], "/");
			free(paths[i]);
			paths[i] = path;
		}
	}
	return (paths);
}

char	*path(t_shell *shell, char *cmd)
{
	char	**paths;
	char	*valid_path;
	int		i;

	paths = append_path(shell);
	if (!paths)
		exit(127);
	valid_path = check_path(cmd, paths);
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (valid_path);
}
