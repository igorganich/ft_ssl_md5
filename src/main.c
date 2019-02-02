/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:54:58 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 17:47:29 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"
#include "../includes/ft_ssl.h"

int		freessl(t_data *ssl, int ret, t_func *funcs, char **names)
{
	int i;

	i = 0;
	while (++i < 3)
		free(names[i]);
	free(names);
	free(ssl->in);
	free(ssl->result);
	free(ssl);
	free(funcs);
	return (ret);
}

int		get_cmd(char *str)
{
	if (!ft_strcmp(str, "md5"))
		return (CMD_MD5);
	if (!ft_strcmp(str, "sha256"))
		return (CMD_SHA256);
	return (0);
}

int		next_arg(t_data *ssl, char *str, t_func *funcs, char **names)
{
	if (!ssl->is_file && !ssl->f_s)
	{
		if (str[0] == '-')
			while (*(++str))
			{
				if (str[0] == 'p')
					read_from_in(ssl, 1, funcs);
				else if (str[0] == 'q')
					ssl->f_q = 1;
				else if (str[0] == 's')
					return (read_s(str + 1, ssl, 1, funcs));
				else if (str[0] == 'r')
					ssl->f_r = 1;
				else
					return (print_bad_flag(str[0]));
			}
		else
			ssl->is_file = 1;
	}
	if (ssl->is_file)
		return (read_from_file(ssl, str, funcs, names));
	else if (ssl->f_s)
		return (read_s(str, ssl, 0, funcs));
	return (1);
}

t_func	*create_func_links(char ***names, t_data *ssl)
{
	t_func *funcs;

	*names = ft_memalloc(sizeof(char*) * 3);
	(*names)[CMD_MD5] = ft_strdup("MD5");
	(*names)[CMD_SHA256] = ft_strdup("SHA256");
	funcs = ft_memalloc(sizeof(t_func) * 3);
	funcs[CMD_MD5] = md5;
	funcs[CMD_SHA256] = sha256;
	ssl->names = *names;
	return (funcs);
}

int		main(int argc, char **argv)
{
	t_data	*ssl;
	int		i;
	t_func	*funcs;
	char	**names;

	ssl = ft_memalloc(sizeof(t_data));
	funcs = create_func_links(&names, ssl);
	ssl->in = ft_strdup("");
	if ((i = 1) && argc == 1)
	{
		ft_printf("usage: ft_ssl command [command opts] [command args]\n");
		return (freessl(ssl, 0, funcs, names));
	}
	if ((ssl->cmd = get_cmd(argv[1])) == 0)
		return (print_bad_cmd(argv[1], ssl, funcs, names));
	while (++i < argc)
		if (next_arg(ssl, argv[i], funcs, names) == -1)
			break ;
	if (ssl->f_s)
		ft_printf("ft_ssl: option -s requires an argument\n");
	if (!ssl->was_s && !ssl->is_file && (!ssl->f_p || ssl->f_q || ssl->f_r))
		read_from_in(ssl, 0, funcs);
	return (freessl(ssl, 1, funcs, names));
}
