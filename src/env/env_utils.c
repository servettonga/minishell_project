/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sehosaf <sehosaf@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:42:49 by sehosaf           #+#    #+#             */
/*   Updated: 2024/06/05 09:27:33 by sehosaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Gets the environment variable node with the specified key.
 * @param env The environment variable list of the shell structure
 * @param key The key of the environment variable
 * @return The node with the specified key
*/
t_env	*get_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/**
 * @brief Gets the last node of the environment variable list.
 * @param env The environment variable list of the shell structure
 * @return The last node of the list
*/
t_env	*get_last_node(t_env *env)
{
	t_env	*last_node;

	last_node = env;
	while (last_node->next)
		last_node = last_node->next;
	return (last_node);
}

/**
 * @brief Splits a string into two parts using a delimiter.
 * @param str The string to split
 * @param c The delimiter
 * @return The two parts of the string
*/
char	**split_key_value(const char *str, char c)
{
	char	**result;
	char	*index;

	result = (char **)malloc(sizeof(char *) * 3);
	if (!result)
		return (perror("malloc"), NULL);
	index = ft_strchr(str, c);
	if (index)
	{
		result[0] = ft_substr(str, 0, index - str);
		result[1] = ft_strdup(index + 1);
	}
	else
	{
		result[0] = ft_strdup(str);
		result[1] = NULL;
	}
	result[2] = NULL;
	return (result);
}
