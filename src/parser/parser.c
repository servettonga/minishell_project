#include "parser.h"

/*
Main API for parsing part of the programm
Arg:
	line (char *) - prompt text
Return:
	Commands pipeline - structure which describes what should be executet

1) Split string into tokens
2) Interpret tokens and devide them by |, && and || operators to commands
3) iterrate through commands:
	3.1) redirections
	3.2) variables
	3.3) drop quotations
	3.4) wildcard

Undefined behavior:
- multiple redirections to the same command, eg. `cmd >1.out >>2.out` or `cmd <1.txt <<LIM`
*/
t_pipeline *parse(char *line)
{
	t_pipeline   *res;
	char		**tokens;

	tokens = split_line(line);
	if (!tokens)
		return (NULL);
	res = create_defalt_pipeline_node();
	split_tokens_per_command(res, tokens);
	free_split(tokens);
	set_pipeline_parameters(res);
	return (res);
}

void set_pipeline_parameters(t_pipeline *node)
{
	while (node)
	{
		if (!node->cmd.args || !node->cmd.args[0])
			break;
		set_connection_type(node);
		set_redirections(node);
		replace_vars(node);
		replace_wildcards(node);
		node = node->next;
	}
}

void set_connection_type(t_pipeline *node)
{
	if (ft_memcmp(node->cmd.args[0], "|", 2))
		node->cmd.connection_type = CON_PIPE;
	else if (ft_memcmp(node->cmd.args[0], "||", 3))
		node->cmd.connection_type = CON_OR;
	else if (ft_memcmp(node->cmd.args[0], "&&", 3))
		node->cmd.connection_type = CON_AND;
	else
		return ;
	remove_cmd_arg(node, 0);
}

void replace_wildcards(t_pipeline *node)
{
	int i;
	char *asterisk;
	// int n_obj;
	t_list *args_list;
	char **new_args;

	i = 0;
	while (node->cmd.args[i])
	{
		asterisk = ft_strchr(node->cmd.args[i], '*');
		if (asterisk && validate_asterisk(node->cmd.args[i], asterisk))
		{
			// n_obj = count_dir_obj();
			// new_args = malloc(sizeof(char *) * (n_obj + 1));
			args_list = get_args_list(node->cmd.args[i], asterisk);
			if (ft_lstsize(args_list))
			{
				new_args = malloc(sizeof(char *) * (ft_lstsize(args_list) + 1));
				fill_new_args(new_args, args_list);
				replace_asterisk(node->cmd.args, i, new_args);
				i += ft_lstsize(args_list) - 1;
			}
			ft_lstclear(&args_list, free);
		}
		i++;
	}
}

/*
Simple validation of asterisk.
Valid only if out of any type of quotes.
*/
int validate_asterisk(char *arg, char *asterisk)
{
	int sq;
	int dq;
	int i;

	i = 0;
	sq = 0;
	dq = 0;
	while (arg + i < asterisk)
	{
		if (arg[i] == '\'')
			sq++;
		else if (arg[i] == '"')
			dq++;
		i++;
	}
	if (!(sq % 2) || !(dq % 2))
		return (0);
	return (1);
}

// int count_dir_obj()
// {
// 	char *dirname;
// 	DIR *dirstream;
// 	int n_obj;
	
// 	dirname = getcwd(NULL, 0);
// 	dirstream = opendir(dirname);
// 	free(dirname);
// 	if (!dirstream)
// 		return (0);
// 	n_obj = 0;
// 	while (readdir(dirstream))
// 		n_obj++;
// 	closedir(dirstream);
// 	return (n_obj);
// }

t_list *get_args_list(char *arg, char *asterisk)
{
	t_list *res;
	char *dirname;
	DIR *dirstream;
	struct dirent *_dirent;

	res = NULL;
	dirname = getcwd(NULL, 0);
	dirstream = opendir(dirname);
	free(dirname);
	if (!dirstream)
		return (0);
	_dirent = readdir(dirstream);
	while (_dirent)
	{
		if (arg - asterisk <= ft_strlen(_dirent->d_name)
			&& !ft_memcmp(_dirent->d_name, arg, arg - asterisk)
			&& ft_strlen(asterisk + 1) <= ft_strlen(_dirent->d_name)
			&& !ft_memcmp(_dirent->d_name + ft_strlen(_dirent->d_name) - ft_strlen(asterisk + 1), asterisk + 1, ft_strlen(asterisk + 1)))
			ft_lstadd_back(&res, ft_lstnew(ft_strdup(_dirent->d_name)));
		_dirent = readdir(dirstream);
	}
	closedir(dirstream);
	return (res);
}
