/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lxr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:17:37 by yohurteb          #+#    #+#             */
/*   Updated: 2024/08/15 14:39:29 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini-shell.h"

int	len_db_quote(char *str, int *i, char stop)
{
	int	len;
	int	tmp;

	len = 0;
	(*i)++;
	tmp = *i;
	while (str[*i] && str[*i] != stop)
	{
		(*i)++;
		len++;
	}
	*i = tmp;
	return (len);
}

int	len_string(char *str, int *i)
{
	int	len;
	int tmp;

	len = 0;
	tmp = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '"' && str[*i] != '\'')
	{
		(*i)++;
		len++;
	}
	*i = tmp;
	return (len);
}

void	cpy_str(char *str, t_data *data, int *i, char stop)
{
	int tmp;

	tmp = 0;
	while (str[*i] && str[*i] != stop)
	{
		data->lex->string[tmp] = str[*i];
		(*i)++;
		tmp++;
		if (stop == ' ')
			if (str[*i] == '"' || str[*i] == '\'')
				break;
	}
	if (!str[*i] && stop != ' ')
		(*i)--;
	data->lex->string[tmp] = '\0';
}
