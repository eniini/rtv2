/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:41:45 by esukava           #+#    #+#             */
/*   Updated: 2022/06/18 18:45:14 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_bool	in_shadow(t_rt *rt, t_ray light_ray, uint cur_obj)
{
	uint	i;
	float	t;

	i = 0;
	while (i < rt->objcount && i != cur_obj)
	{
		t = RAY_LIMIT;
		if (ray_object_intersect(&light_ray, &rt->object[i], &t))
		{
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

/*
*	[Phong] in this context means the refractive part of the shading style.
*	[difference] is a vector representing the relfection direction given the
*	incident (ray hitting a surface coming from a light source) [I] and surface
*	normal [N] vectors.
*	I - (2 * dot(N, I) * N).
*	Smaller angle between POV and light source to the hit point means more
*	light is reflected to the camera. [ROUGHNESS] variable controls how refined
*	the refraction is.
*/
static t_color	assign_color(t_rt *rt, t_ray lray, t_fvector n, t_color mix)
{
	float	phong;
	float	lambert;
	float	difference;
	t_color	final;

	difference = 2.0f * v_dot(n, lray.dir);
	phong = fmaxf(v_dot(v_sub(v_mult(n, difference), lray.dir), \
			v_normalize(rt->cam.pos)), 0.0f);
	phong = powf(ft_clamp_d(phong, 0.0f, 1.0f), ROUGHNESS);
	lambert = v_dot(lray.dir, n);
	mix = col_blend(mix, rt->lightcolor, (lambert * 0.8));
	final = col_add(mix, col_multiply((t_color){1, 1, 1}, phong), phong);
	return (final);
}

/*
*	[Ray] is the point of interception between ray & [cur_obj].
*	If a point is not directly illuminated or is in a shadow of another object,
*	no new color is applied.
*/
static void	calculate_lighting(t_rt *rt, t_ray *ray, int cur_obj, t_color *c)
{
	t_ray		lr;
	t_fvector	dist;
	t_fvector	n;
	uint		i;

	n = find_object_normal(&rt->object[cur_obj], ray);
	i = 0;
	while (i < rt->lightcount)
	{
		dist = v_normalize(v_sub(rt->light[i].pos, ray->start));
		if (v_dot(n, dist) > 0)
		{
			rt->lightcolor = col_blend(rt->lightcolor, rt->light[i].color, \
				v_dot(n, dist));
			*c = col_blend(*c, rt->object[cur_obj].color, v_dot(n, dist));
		}
		lr.start = v_add(ray->start, v_mult(n, 0.00001f));
		lr.dir = dist;
		if (in_shadow(rt, lr, cur_obj))
			rt->shadow_lvl++;
		*c = assign_color(rt, lr, n, *c);
		i++;
	}
	if (rt->shadow_lvl)
		*c = col_blend(*c, rt->amb_col, rt->shadow_lvl / (float)rt->lightcount);
}

static t_color	ray_col(t_rt *rt, int cur_obj, float t)
{
	t_color		mixer;

	rt->ray_light.start = v_add(rt->ray_prime.start, \
		v_mult(rt->ray_prime.dir, t));
	rt->ray_light.dir = rt->ray_prime.dir;
	uv_map(rt, &rt->ray_light, cur_obj);
	mixer = rt->amb_col;
	calculate_lighting(rt, &rt->ray_light, cur_obj, &mixer);
	mixer = col_blend(mixer, apply_check_pattern(rt, 25, cur_obj, mixer), 0.1f);
	rt->lightcolor = (t_color){1, 1, 1};
	rt->shadow_lvl = 0;
	return (mixer);
}

void	raytracer(t_rt *rt, int x, int y)
{
	uint		i;
	float		t;
	int			cur_obj;

	cur_obj = -1;
	ray_trough_screen(rt, x, y);
	t = RAY_LIMIT;
	i = 0;
	while (i < rt->objcount)
	{
		if (ray_object_intersect(&rt->ray_prime, &rt->object[i], &t))
			cur_obj = i;
		i++;
	}
	if (draw_light(rt, &t, x, y) || cur_obj == -1)
		return ;
	if (rt->is_grayscale)
		draw_pixel(x, y, &rt->rend.win_buffer, \
			convert_to_grayscale(col_to_uint(ray_col(rt, cur_obj, t))));
	else
		draw_pixel(x, y, &rt->rend.win_buffer, \
			col_to_uint(ray_col(rt, cur_obj, t)));
}
