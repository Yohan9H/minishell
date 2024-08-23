/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:32:47 by yohurteb          #+#    #+#             */
/*   Updated: 2024/08/23 10:51:14 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(tokentype type)
{
	if (type == INPUT_TOKEN || type == OUTPUT_TOKEN || type == APPEND_TOKEN 
		|| type == HEREDOC_TOKEN)
		return (1);
	else
		return (0);
}
