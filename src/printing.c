/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 15:29:30 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 15:35:12 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

int		print_bad_cmd(char *str, t_data *ssl, t_func *funcs, char **names)
{
	ft_printf("ft_ssl: Error: '%s' is an invalid command.\n", str);
	ft_printf("\nStandart commands:\n");
	ft_printf("\nMessage Digest commands:\nmd5\nsha256\n");
	ft_printf("\nCipher commands:\n");
	return (freessl(ssl, 0, funcs, names));
}

int		print_bad_flag(char c)
{
	printf("ft_ssl: illegal option -- %c\n", c);
	printf("options: [-pqrs]\n");
	return (-1);
}

int		print_bad_file(char *str, int flag)
{
	if (flag == 2)
		ft_printf("ft_ssl: %s: Path is a directory\n", str);
	if (flag == 1)
		ft_printf("ft_ssl: %s: No such file or directory\n", str);
	return (1);
}
