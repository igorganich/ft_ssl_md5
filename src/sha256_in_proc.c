/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_in_proc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganich <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 14:56:59 by iganich           #+#    #+#             */
/*   Updated: 2018/09/06 15:40:41 by iganich          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	process_cycle(uint32_t *w, uint32_t *temp, t_data *ssl, uint32_t *t)
{
	int i;

	i = -1;
	while (++i < 64)
	{
		t[1] = temp[7] + EPSILON_1(temp[4]) + CH(temp[4], temp[5], temp[6])
			+ ssl->k[i] + w[i];
		t[2] = EPSILON_0(temp[0]) + MAJ(temp[0], temp[1], temp[2]);
		temp[7] = temp[6];
		temp[6] = temp[5];
		temp[5] = temp[4];
		temp[4] = temp[3] + t[1];
		temp[3] = temp[2];
		temp[2] = temp[1];
		temp[1] = temp[0];
		temp[0] = t[1] + t[2];
	}
}
