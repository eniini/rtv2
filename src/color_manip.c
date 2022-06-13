/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_manip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 21:21:54 by eniini            #+#    #+#             */
/*   Updated: 2022/06/13 12:53:21 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//linear interpolation of two t_color structs.
t_color	col_lerp(t_color c1, t_color c2, float p)
{
	t_color	result;

	if (p < 0.0f)
		p = 0.0f;
	if (p > 1.0f)
		p = 1.0f;
	result.red = (float)ft_d_lerp(c1.red, c2.red, p);
	result.green = (float)ft_d_lerp(c1.green, c2.green, p);
	result.blue = (float)ft_d_lerp(c1.blue, c2.blue, p);
	return (result);
}

//mix/blend two colors together
t_color	col_blend(t_color base, t_color mix, float p)
{
	t_color	result;
	float	pm;

	if (p < 0.0f)
		p = 0.0f;
	if (p > 1.0f)
		p = 1.0f;
	pm = 1.0f - p;
	result.red = pm * base.red + p * mix.red;
	result.green = pm * base.green + p * mix.green;
	result.blue = pm * base.blue + p * mix.blue;
	return (result);
}

t_color	col_multiply(t_color color, float m)
{
	t_color	result;

	result.red = ft_clamp_d(color.red * m, 0.0f, 1.0f);
	result.green = ft_clamp_d(color.green * m, 0.0f, 1.0f);
	result.blue = ft_clamp_d(color.blue * m, 0.0f, 1.0f);
	return (result);
}

t_color	col_add(t_color base, t_color mix, float p)
{
	t_color	result;

	if (p == 0.0f)
		return (base);
	if (p == 1.0f)
	{
		result.red = ft_clamp_d(base.red + mix.red, 0.0f, 1.0f);
		result.green = ft_clamp_d(base.green + mix.green, 0.0f, 1.0f);
		result.blue = ft_clamp_d(base.blue + mix.blue, 0.0f, 1.0f);
		return (result);
	}
	result.red = ft_clamp_d(base.red + (p * mix.red), 0.0f, 1.0f);
	result.green = ft_clamp_d(base.green + (p * mix.green), 0.0f, 1.0f);
	result.blue = ft_clamp_d(base.blue + (p * mix.blue), 0.0f, 1.0f);
	return (result);
}

t_color	col_sub(t_color base, t_color mix, float p)
{
	t_color	result;

	if (p == 0.0f)
		return (base);
	if (p == 1.0f)
	{
		result.red = ft_clamp_d(base.red - mix.red, 0.0f, 1.0f);
		result.green = ft_clamp_d(base.green - mix.green, 0.0f, 1.0f);
		result.blue = ft_clamp_d(base.blue - mix.blue, 0.0f, 1.0f);
		return (result);
	}
	result.red = ft_clamp_d(base.red - (p * mix.red), 0.0f, 1.0f);
	result.green = ft_clamp_d(base.green - (p * mix.green), 0.0f, 1.0f);
	result.blue = ft_clamp_d(base.blue - (p * mix.blue), 0.0f, 1.0f);
	return (result);
}
