/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:22:52 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/23 14:22:55 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	check_n_option(char **args, int *index)
{
	bool	n_option;
	int		j;

	n_option = false;
	*index = 1;
	while (args[*index] && args[*index][0] == '-' && args[*index][1] == 'n')
	{
		j = 1;
		while (args[*index][j] == 'n')
			j++;
		if (args[*index][j] == '\0')
			n_option = true;
		else
			break;
		(*index)++;
	}
	return (n_option);
}

void	print_args(char **args, int index)
{
	while (args[index])
	{
		printf("%s", args[index]);
		if (args[index + 1])
			printf(" ");
		index++;
	}
}

int	mini_echo(t_simple_cmds *simple_cmd)
{
	int		i;
	bool	n_option;

	n_option = check_n_option(simple_cmd->str, &i);
	print_args(simple_cmd->str, i);
	if (!n_option)
		printf("\n");
	return (0);
}
