/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 21:18:00 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/22 21:55:38 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "libft.h"
# include "environment.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

//		** execute **
int		execute(t_pipeline *p, t_shell *shell);

//		** execute builtin **
bool	is_builtin(t_command *command);
void	execute_builtin(t_command *command, t_shell *shell);

//		** execute utils **
char	**env_list_to_array(t_env *env);
char	*find_sys_cmd(char *cmd, t_env *env);

//		** execute command **
int		execute_command(t_command *cmd, int pipefd[2], t_env *env);

//		** execute process **
void	handle_parent_process(pid_t pid, t_shell *shell);
pid_t	handle_child_process(t_command *cmd, int pipefd[2], t_env *env);

//		** builtin functions **
int		cmd_cd(t_shell *shell, char **args);
int		cmd_echo(const char **args);
int		cmd_env(t_shell *shell);
void	cmd_exit(t_shell *shell, char **args);
int		cmd_export(t_shell *shell, const char *key_value);
int		cmd_pwd(void);
void	cmd_unset(t_shell *shell, const char *key);

#endif
