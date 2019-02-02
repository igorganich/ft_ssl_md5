/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 15:21:12 by iganich           #+#    #+#             */
/*   Updated: 2018/09/08 15:52:51 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

int		is_file(const char *name)
{
	DIR *directory;

	directory = opendir(name);
	if (directory != NULL)
	{
		closedir(directory);
		return (0);
	}
	if (errno == ENOTDIR)
		return (1);
	return (-1);
}

void	realloc_in(t_data *ssl, char *line)
{
	char	*newstr;
	int		i;
	int		l;
	int		len[2];

	len[0] = ft_strlen(ssl->in);
	len[1] = ft_strlen(line);
	newstr = ft_memalloc(len[0] + len[1] + 2);
	i = -1;
	while (++i < len[0])
		newstr[i] = ssl->in[i];
	l = -1;
	while (++l < len[1])
		newstr[i + l] = line[l];
	newstr[i + l] = '\n';
	free(ssl->in);
	ssl->in = newstr;
}

void	read_from_in(t_data *ssl, int flag, t_func *funcs)
{
	char *line;

	if (!ssl->f_p)
	{
		ssl->f_p++;
		while (get_next_line(0, &line) > 0)
		{
			realloc_in(ssl, line);
			free(line);
		}
		if (flag == 1)
			ft_printf("%s", ssl->in);
		ft_printf("%s\n", funcs[ssl->cmd](ssl, (uint8_t*)ssl->in,
					ft_strlen(ssl->in)));
	}
	else
		ft_printf("%s\n", funcs[ssl->cmd](ssl, (uint8_t*)"", 0));
}

int		read_from_file(t_data *ssl, char *str, t_func *funcs, char **names)
{
	int		fd;
	char	*line;

	if (!is_file(str))
		return (print_bad_file(str, 2));
	if ((fd = open(str, O_RDONLY)) < 0)
		return (print_bad_file(str, 1));
	free(ssl->in);
	ssl->in = ft_strdup("");
	while (get_next_line(fd, &line))
	{
		realloc_in(ssl, line);
		free(line);
	}
	if (!ssl->f_q && !ssl->f_r)
		ft_printf("%s (%s) = ", names[ssl->cmd], str);
	ft_printf("%s", funcs[ssl->cmd](ssl, (uint8_t *)ssl->in,
		ft_strlen(ssl->in)));
	if (!ssl->f_q && ssl->f_r)
		ft_printf(" %s", str);
	ft_printf("\n");
	close(fd);
	return (1);
}

int		read_s(char *str, t_data *ssl, int flag, t_func *funcs)
{
	ssl->was_s = 1;
	ssl->f_s = 1;
	if (str[0] || flag == 0)
	{
		if (!ssl->f_q && !ssl->f_r)
			ft_printf("%s (\"%s\") = ", ssl->names[ssl->cmd], str);
		ft_printf("%s", funcs[ssl->cmd](ssl, (uint8_t*)str, ft_strlen(str)));
		if (ssl->f_r && !ssl->f_q)
			ft_printf(" \"%s\"", str);
		ft_printf("\n");
		ssl->f_s = 0;
	}
	return (1);
}
