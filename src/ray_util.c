/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:13:52 by esukava           #+#    #+#             */
/*   Updated: 2022/06/18 14:21:55 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	Note: working with sligtly smaller integers than actual window size seems
*	to dismiss some rogue rendering artefacts.
*/
void	ray_trough_screen(t_rt *rt, int x, int y)
{
	float	u;
	float	v;
	t_ray	r;

	u = ((float)x / (WIN_W - 1));
	v = ((float)y / (WIN_H - 1));
	r.start = rt->cam.pos;
	r.dir = rt->cam.rt_start;
	r.dir = v_add(r.dir, v_mult(rt->cam.h, u));
	r.dir = v_add(r.dir, v_mult(rt->cam.v, v));
	r.dir = v_sub(r.dir, rt->cam.pos);
	r.dir = v_normalize(r.dir);
	rt->ray_prime = r;
}

t_fvector	find_object_normal(t_object *o, t_ray *ray)
{
	t_fvector	n;
	float		t;

	n = (t_fvector){0, 0, 0, 1};
	if (o->type == SPHERE)
		n = v_sub(ray->start, o->pos);
	else if (o->type == PLANE)
		n = o->dir;
	else if (o->type == CONE)
	{
		n = v_sub(ray->start, o->pos);
		n = v_sub(o->dir, v_div(v_mult(n, v_dot(o->dir, n)), v_dot(n, n)));
	}
	else
	{
		t = v_dot(v_sub(ray->start, o->pos), v_normalize(o->dir));
		n = v_sub(ray->start, v_add(o->pos, v_mult(o->dir, t)));
	}
	if (v_dot(ray->dir, n) > 0)
		n = v_mult(n, -1);
	return (v_normalize(n));
}

t_bool	ray_object_intersect(t_ray *ray, t_object *obj, float *t)
{
	if (obj->type == SPHERE)
		return (ray_sphere_intersect(ray, obj, t));
	else if (obj->type == PLANE)
		return (ray_plane_intersect(ray, obj, t));
	else if (obj->type == CYL)
		return (ray_cyl_intersect(ray, obj, t));
	else if (obj->type == CONE)
		return (ray_cone_intersect(ray, obj, t));
	return (FALSE);
}

void	quadratic_equation(t_fvector abc, float *t0, float *t1)
{
	float	discriminant;
	float	denominator;

	discriminant = abc.y * abc.y - 4 * (abc.x * abc.z);
	denominator = 2 * abc.x;
	if (discriminant < 0 || denominator == 0 || fabsf(discriminant) < 0.001f)
	{
		*t0 = -1;
		*t1 = -1;
		return ;
	}
	*t0 = (-abc.y - sqrtf(discriminant)) / denominator;
	*t1 = (-abc.y + sqrtf(discriminant)) / denominator;
}

t_bool	draw_light(t_rt *rt, float *t, int x, int y)
{
	uint	i;

	i = 0;
	while (i < rt->lightcount)
	{
		if (ray_sphere_intersect(&rt->ray_prime, &rt->light[i], t))
		{
			draw_pixel(x, y, &rt->rend.win_buffer, 0xFFFF00);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
