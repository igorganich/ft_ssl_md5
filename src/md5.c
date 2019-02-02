/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 12:50:54 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 17:46:19 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void		get_current_ifg(uint32_t ifg[4], uint32_t abcd[4])
{
	if (ifg[0] < 16)
	{
		ifg[1] = (abcd[1] & abcd[2]) | ((~abcd[1]) & abcd[3]);
		ifg[2] = ifg[0];
	}
	else if (ifg[0] < 32)
	{
		ifg[1] = (abcd[3] & abcd[1]) | ((~abcd[3]) & abcd[2]);
		ifg[2] = (5 * ifg[0] + 1) % 16;
	}
	else if (ifg[0] < 48)
	{
		ifg[1] = abcd[1] ^ abcd[2] ^ abcd[3];
		ifg[2] = (3 * ifg[0] + 5) % 16;
	}
	else
	{
		ifg[1] = abcd[2] ^ (abcd[1] | (~abcd[3]));
		ifg[2] = (7 * ifg[0]) % 16;
	}
}

static void	third_step(t_data *ssl, uint32_t ifg[4], uint32_t abcd[4],
		uint32_t *w)
{
	uint32_t	temp;
	int			i;

	ft_memcpy(abcd, ssl->h, sizeof(uint32_t) * 4);
	ifg[0] = -1;
	while (++ifg[0] < 64)
	{
		get_current_ifg(ifg, abcd);
		temp = abcd[3];
		abcd[3] = abcd[2];
		abcd[2] = abcd[1];
		abcd[1] = abcd[1] + LEFTROTATE((abcd[0] + ifg[1] + ssl->k[ifg[0]]
				+ w[ifg[2]]), ssl->r[ifg[0]]);
		abcd[0] = temp;
	}
	i = -1;
	while (++i < 4)
		ssl->h[i] += abcd[i];
}

char		*create_str(t_data *ssl)
{
	int			i;
	int			l;
	uint8_t		*p;
	static char *base = "0123456789abcdef";

	i = -1;
	free(ssl->result);
	ssl->result = ft_memalloc(33);
	if (!base)
		return ("asdasd");
	while (++i < 4)
	{
		p = (uint8_t *)&(ssl->h[i]);
		l = -1;
		while (++l < 4)
		{
			ssl->result[i * 8 + l * 2 + 1] = base[p[l] % 16];
			p[l] /= 16;
			ssl->result[i * 8 + l * 2] = base[p[l]];
		}
	}
	return (ssl->result);
}

char		*sec_step(t_data *ssl, uint8_t *msg, int new_len, int offset)
{
	uint32_t	bits_len;
	uint32_t	*w;
	uint32_t	afg[2][4];

	ssl->h[0] = 0x67452301;
	ssl->h[1] = 0xefcdab89;
	ssl->h[2] = 0x98badcfe;
	ssl->h[3] = 0x10325476;
	new_len = ssl->start_len * 8 + 1;
	while (new_len % 512 != 448)
		new_len++;
	new_len /= 8;
	msg = ft_memalloc(new_len + 64);
	ft_memcpy(msg, ssl->start_msg, ssl->start_len);
	msg[ssl->start_len] = 128;
	bits_len = 8 * ssl->start_len;
	ft_memcpy(msg + new_len, &bits_len, 4);
	offset = (512 / 8) * (-1);
	while ((offset += 512 / 8) < new_len)
	{
		w = (uint32_t *)(msg + offset);
		third_step(ssl, afg[0], afg[1], w);
	}
	free(msg);
	return (create_str(ssl));
}

char		*md5(t_data *ssl, uint8_t *start_msg, size_t start_len)
{
	static uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		7, 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14,
		20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
	static uint32_t	k[] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf,
		0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
		0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
		0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6,
		0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
		0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
		0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
		0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
		0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
		0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

	ssl->start_msg = start_msg;
	ssl->start_len = start_len;
	ssl->r = r;
	ssl->k = k;
	return (sec_step(ssl, NULL, 0, 0));
}
