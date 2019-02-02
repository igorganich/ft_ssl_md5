/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 14:45:38 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 15:40:08 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void		sha256_process_block(uint32_t *h, unsigned char *m, t_data *ssl)
{
	uint32_t		temp[8];
	uint32_t		t[3];
	uint32_t		w[64];
	unsigned int	i;

	i = -1;
	while (++i < 16)
		w[i] = m[i * 4 + 3] | (m[i * 4 + 2] << 8) | (m[i * 4 + 1] << 16) |
			(m[i * 4 + 0] << 24);
	i--;
	while (++i < 64)
		w[i] = SIGMA_1(w[i - 2]) + w[i - 7] + SIGMA_0(w[i - 15]) + w[i - 16];
	i = -1;
	while (++i < 8)
		temp[i] = h[i];
	process_cycle(w, temp, ssl, t);
	i = -1;
	while (++i < 8)
		h[i] += temp[i];
}

void		sha256_init(t_data *ssl, uint8_t *start_msg, size_t start_len)
{
	static uint32_t k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98,
	0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe,
	0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6,
	0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3,
	0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138,
	0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e,
	0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116,
	0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814,
	0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
	static uint32_t h_0[8] = {0x6a09e667, 0xbb67ae85,
	0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c,
	0x1f83d9ab, 0x5be0cd19};

	ssl->k = k;
	ssl->h_0 = h_0;
	ssl->start_msg = start_msg;
	ssl->start_len = start_len;
	ft_memcpy(ssl->h, ssl->h_0, 32);
}

static void	third_step(unsigned char buffer[64], t_data *ssl)
{
	int			i;
	static char	*base = "0123456789abcdef";

	free(ssl->result);
	ssl->result = ft_memalloc(65);
	i = -1;
	while (++i < 8)
	{
		buffer[i * 4 + 0] = ssl->h[i] >> 24;
		buffer[i * 4 + 1] = ssl->h[i] >> 16;
		buffer[i * 4 + 2] = ssl->h[i] >> 8;
		buffer[i * 4 + 3] = ssl->h[i];
	}
	i = -1;
	while (++i < 32)
	{
		ssl->result[i * 2 + 1] = base[buffer[i] % 16];
		buffer[i] /= 16;
		ssl->result[i * 2] = base[buffer[i]];
	}
}

void		second_step(t_data *ssl, size_t len, uint64_t bits,
			unsigned char buffer[64])
{
	unsigned int i;

	if (len < 56)
	{
		buffer[len] = 0x80;
		i = len;
	}
	else
	{
		buffer[len] = 0x80;
		i = len;
		while (++len < 64)
			buffer[i] = 0x00;
		sha256_process_block(ssl->h, buffer, ssl);
		i = -1;
	}
	while (++i < 56)
		buffer[i] = 0x00;
	i = -1;
	while (++i < 8)
		buffer[63 - i] = bits >> (i * 8);
	sha256_process_block(ssl->h, buffer, ssl);
	third_step(buffer, ssl);
}

char		*sha256(t_data *ssl, uint8_t *start_msg, size_t start_len)
{
	unsigned char	buffer[64];
	uint64_t		bits;
	size_t			len;

	sha256_init(ssl, start_msg, start_len);
	len = start_len;
	while (1)
	{
		ft_memcpy(buffer, (unsigned char *)start_msg + (start_len - len),
				len < 64 ? len : 64);
		if (len < 64)
			break ;
		sha256_process_block(ssl->h, buffer, ssl);
		len -= 64;
	}
	bits = 8 * start_len;
	second_step(ssl, len, bits, buffer);
	return (ssl->result);
}
