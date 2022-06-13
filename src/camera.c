/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 12:59:25 by eniini            #+#    #+#             */
/*   Updated: 2022/06/13 12:59:50 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_cam(t_rt *rt, t_fvector camdir, float ar, float vfov)
{
	t_fvector	w;
	t_fvector	vup;
	float		h;

	if (camdir.x == 0 && camdir.y == 0 && camdir.z == 0)
		ft_getout("Invalid camera init! (Must be a real vector)");
	h = tanf((vfov * DEG_TO_RAD) / 2.0f);
	vup = (t_fvector){0, 1, 0, 1};
	w = v_normalize(camdir);
	if (w.x == 0 && w.y == -1 && w.z == 0)
		vup = (t_fvector){0, 0, -1, 1};
	rt->cam.h = v_mult(v_normalize(v_cross(vup, w)), ar * 2.0f * h);
	rt->cam.v = v_mult(v_cross(w, v_normalize(v_cross(vup, w))), 2.0f * h);
	rt->cam.start = v_sub(rt->cam.pos, v_div(rt->cam.h, 2.0f));
	rt->cam.start = v_sub(rt->cam.start, v_div(rt->cam.v, 2.0f));
	rt->cam.start = v_sub(rt->cam.start, w);
}
