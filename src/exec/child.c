/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apernot <apernot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:40:47 by apernot           #+#    #+#             */
/*   Updated: 2024/09/26 17:01:26 by apernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_line(t_exec *exec, t_data *data)
{
	char	*path;
	char	**env;

	if (!exec->cmd)
		return (0);
	path = my_get_path(exec->cmd, data);
	if (!path)
		return (error_exec(exec->cmd, errno), -1);
	env = my_env_to_tab(data->my_env);
	if (!env)
		return (free(path), -1);
	if (!exec->args)
		return (free(path), freetab(env), -1);
	if (execve(path, exec->args, env) == -1)
	{
		error_exec(exec->cmd, errno);
		free(path);
		freetab(env);
		return (-2);
	}
	return (0);
}

int	open_clean(t_redir *redir, t_data *data, t_execom *execom)
{
	int	fd;
	int	flags;

	if (redir->type == APPEND_TK)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (redir->type == OUTPUT_TK)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (redir->type == INPUT_TK || redir->type == HEREDOC_TK)
		fd = open(redir->filename, O_RDONLY);
	else
		fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		exit_clean(data, NOTHING, C_EXIT);
		if (execom->pipe_fd[0] != -1)
			close(execom->pipe_fd[0]);
		if (execom->pipe_fd[1] != -1)
			close(execom->pipe_fd[1]);
		exit (1);
	}
	return (fd);
}

int	create_child_process(t_data *data)
{
	int	id;

	id = fork();
	if (id < 0)
	{
		perror("fork");
		exit_clean(data, NOTHING, N_EXIT);
	}
	return (id);
}

void	close_fd_in_out(t_execom *execom, int fdstd)
{
	if (fdstd == 1)
	{
		close(execom->fdstdin);
		close(execom->fdstdout);
	}
	else
	{
		close(execom->pipe_fd[0]);
		close(execom->pipe_fd[1]);
	}
}

void	child_process(t_exec *exec,	t_data *data, t_execom *execom)
{
	int	exit_code;

	exit_code = 0;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	close_fd_in_out(execom, 1);
	if (exec->next && !output_redir(exec->redir))
	{
		if (execom->pipe_fd[1] != -1)
			dup2(execom->pipe_fd[1], STDOUT_FILENO);
	}
	redir(exec->redir, data, execom);
	close_fd_in_out(execom, 0);
	if (is_builtin(data, exec) == 1)
	{
		verif_builtin(data, exec, execom);
		exit_clean(data, NOTHING, C_EXIT);
		exit (exit_code);
	}
	exit_code = exec_line(exec, data);
	exit_clean(data, NOTHING, C_EXIT);
	if (exit_code == -2)
		exit (IS_A_DIRECTORY);
	exit (COMMAND_NOT_FOUND);
}
