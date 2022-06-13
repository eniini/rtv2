/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:12:21 by esukava           #+#    #+#             */
/*   Updated: 2022/06/13 12:54:33 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	Colors the pixel in given coordinates of the given buffer as the given
*	hexadecimal value [0xAARRGGBB]. (00 for AA being zero transparency)
*/
void	draw_pixel(uint32_t x, uint32_t y, t_buffer *buf, uint32_t color)
{
	uint32_t	i;

	i = (buf->w * y) + x;
	if (x < buf->w && y < buf->h)
		buf->px[i] = color;
}

//convert t_color to ARGB uint for SDL pixel painting
uint32_t	col_to_uint(t_color color)
{
	uint8_t	ir;
	uint8_t	ig;
	uint8_t	ib;

	ir = (uint8_t)(255 * ft_clamp_d(color.red, 0.0f, 1.0f));
	ig = (uint8_t)(255 * ft_clamp_d(color.green, 0.0f, 1.0f));
	ib = (uint8_t)(255 * ft_clamp_d(color.blue, 0.0f, 1.0f));
	return (255 << 24 | ir << 16 | ig << 8 | ib);
}
