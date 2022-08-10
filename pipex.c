/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnaceur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 01:17:57 by tnaceur           #+#    #+#             */
/*   Updated: 2022/03/03 01:22:27 by tnaceur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"
#include <stdbool.h>

char	*get_path(char **path, char *av2)
{
	int	i;

	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], av2);
		if (access(path[i], F_OK) == 0)
			return (path[i]);
		i++;
	}
	return (path[i]);
}

void	first_child(char *av1, char *av2, char **path, int *fd)
{
	char	**ag;
	char	*new_path;
	int		file;

	file = open(av1, 'r', 0777);
	if (file == -1)
	{
		perror(NULL);
		exit(1);
	}
	close(fd[0]);
	ag = ft_split(av2, ' ');
	if (access(ag[0], F_OK) == 0)
		new_path = ag[0];
	else
		new_path = get_path(path, ag[0]);
	dup2(file, 0);
	dup2(fd[1], 1);
	close(file);
	close(fd[0]);
	if (execve(new_path, ag, NULL) == -1)
	{
		ft_putstr("command not found : ", ag[0]);
		exit(EXIT_FAILURE);
	}
}

void	second_child(char **path, char *av3, char *av4, int *fd)
{
	char	**ag;
	char	*new_path;
	int		file;

	ag = ft_split(av3, ' ');
	if (access(ag[0], F_OK) == 0)
		new_path = ag[0];
	else
		new_path = get_path(path, ag[0]);
	file = open(av4, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	close(fd[1]);
	dup2(file, 1);
	dup2(fd[0], 0);
	close(fd[0]);
	close(file);
	if (execve(new_path, ag, NULL) == -1)
	{
		ft_putstr("command not found : ", ag[0]);
		exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;
	int		i;

	i = 0;
	if (ac == 5)
	{
		if (pipe(pipex.fd) == -1)
			return (-1);
		while (env[i])
		{
			if (env[i][0] == 'P' && env[i][1] == 'A')
				break ;
			i++;
		}
		pipex.path = ft_split(env[i] + 5, ':');
		pipex.id = fork();
		if (pipex.id == 0)
			first_child(av[1], av[2], pipex.path, pipex.fd);
		pipex.id2 = fork();
		if (pipex.id2 == 0)
			second_child(pipex.path, av[3], av[4], pipex.fd);
		close(pipex.fd[1]);
		close(pipex.fd[0]);
		waitpid(pipex.id, NULL, 0);
		waitpid(pipex.id2, NULL, 0);
	}
	else
		write(1, "WRONG ARGUMENT", 15);
	return (0);
}
