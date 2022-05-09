/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 16:33:42 by eniini            #+#    #+#             */
/*   Updated: 2022/05/09 22:35:17 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

/*
*	Returns a color corresponding to a checkered pattern. [Scale] directly
*	corresponds to how many tiles are generated onto the plane.
*/
t_color	apply_check_pattern(t_rt *rt, float scale, t_object obj)
{
	t_bool	yresult;
	t_bool	xresult;
	t_bool	result;

	if (obj.type == PLANE)
		scale = 1.0f;
	if (((rt->uv_v * scale) - floorf(rt->uv_v * scale)) < 0.5f)
		yresult = TRUE;
	else
		yresult = FALSE;
	if (((rt->uv_u * scale) - floorf(rt->uv_u * scale)) < 0.5f)
		xresult = TRUE;
	else
		xresult = FALSE;
	result = (xresult ^ yresult);
	if (result)
		return ((t_color){0,0,0});
	else
		return ((t_color){1,1,1});
}

/*
*	TESTING
*/
t_color	apply_texture(t_rt *rt, float x, float y)
{
	float	a = 20.0f;
	float	b = 50.0f;
	float	t = (a * x + b * y);
	float	s = t - floorf(t);
	//float	s = 1.0f + sinf(t) / 2.0f;
	return (col_blend((t_color){0,0,0}, (t_color){1,1,1}, s));
	//(1.0+sin(t))/2.0, abs(sin(t)) and (t−floor(t))

}

/*
*	[pos] is assumed to be a point in the surface of the sphere.
*/
static void	spherical_map(t_rt *rt, t_fvector pos, t_fvector hp, t_object obj)
{
	float	theta;
	float	phi;

	theta = atan2f(pos.x, pos.z);
	rt->uv_u = 1.0f - (theta / (2.0f * M_PI) + 0.5f);
	phi = acosf(pos.y / v_len(v_sub(hp, obj.pos)));
	rt->uv_v = 1.0f - (phi / M_PI);
	rt->uv_v *= 0.5;
}

static void planar_map(t_rt *rt, t_fvector hp)
{
	rt->uv_u = fmodf(hp.x, 1.0f);
	rt->uv_v = fmodf(hp.z, 1.0f);
}

static void	cylindrical_map(t_rt *rt, t_fvector pos)
{
	float	theta;

	theta = atan2f(pos.x, pos.z);
	rt->uv_u = 1.0f - (theta / (2.0f * M_PI) + 0.5f);
	rt->uv_v = fmodf(pos.y, (2 * M_PI)) * 1 / (2 * M_PI);
}

void	uv_map(t_rt *rt, t_ray *ray, int cur_obj)
{
	if (rt->object[cur_obj].type == SPHERE)
	{
		spherical_map(rt, ray->start, ray->start, rt->object[cur_obj]);
	}
	if (rt->object[cur_obj].type == PLANE)
	{
		planar_map(rt, ray->start);
	}
	if (rt->object[cur_obj].type == CYL)
	{
		cylindrical_map(rt, ray->start);
	}
}