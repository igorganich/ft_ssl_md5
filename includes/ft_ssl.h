/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 15:12:54 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 15:38:47 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# define CMD_MD5 1
# define CMD_SHA256 2
# define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
# define ROTL(x, n)	((x << n) | (x >> (sizeof(x)*8 - n)))
# define ROTR(x, n)	((x >> n) | (x << (sizeof(x)*8 - n)))
# define CH(x, y, z)		((x & y) ^ ((~x) & z))
# define MAJ(x, y, z)	((x & y) ^ (x & z) ^ (y & z))
# define EPSILON_0(x)	(ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
# define EPSILON_1(x)	(ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
# define SIGMA_0(x)		(ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
# define SIGMA_1(x)		(ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../ft_printf/headers/ft_printf.h"
# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <stdint.h>

typedef struct	s_data
{
	int			cmd;
	int			f_p;
	int			f_q;
	int			f_s;
	int			f_r;
	int			is_file;
	int			was_s;
	char		*in;
	uint8_t		*start_msg;
	size_t		start_len;
	uint32_t	h[8];
	uint32_t	*h_0;
	uint32_t	*k;
	uint32_t	*r;
	char		*result;
	char		**names;
}				t_data;

typedef char	*(*t_func)(t_data *ssl, uint8_t *start_msg, size_t start_len);
char			*md5(t_data *ssl, uint8_t *start_msg, size_t start_len);
char			*sha256(t_data *ssl, uint8_t *start_msg, size_t start_len);
void			process_cycle(uint32_t *w, uint32_t *temp, t_data *ssl,
				uint32_t *t);
void			read_from_in(t_data *ssl, int flag, t_func *funcs);
int				read_s(char *str, t_data *ssl, int flag, t_func *funcs);
int				read_from_file(t_data *ssl, char *str, t_func *funcs,
				char **names);
int				print_bad_cmd(char *str, t_data *ssl, t_func *funcs,
				char **names);
int				print_bad_flag(char c);
int				print_bad_file(char *str, int flag);
int				freessl(t_data *ssl, int ret, t_func *funcs, char **names);

#endif
