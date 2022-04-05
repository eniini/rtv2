/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyevents.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:12:37 by esukava           #+#    #+#             */
/*   Updated: 2022/04/04 21:34:05 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void	keyevent(t_rt *rt, SDL_Event *e)
{
	if (SDL_WaitEvent(e) > 0)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->key.keysym.sym == SDLK_w)
				rt->object[9].pos.z -= 5;
			if (e->key.keysym.sym == SDLK_a)
				rt->object[9].pos.x -= 5;
			if (e->key.keysym.sym == SDLK_s)
				rt->object[9].pos.z += 5;
			if (e->key.keysym.sym == SDLK_d)
				rt->object[9].pos.x += 5;
			if (e->key.keysym.sym == SDLK_q)
				rt->object[9].pos.y += 5;
			if (e->key.keysym.sym == SDLK_e)
				rt->object[9].pos.y -= 5;
			if (e->key.keysym.sym == SDLK_ESCAPE)
				rt->rend.run = FALSE;
			rt->cam.dir = v_rot_xyz(rt->cam.dir, rt->cam.rot);
			if (e->key.keysym.sym == SDLK_g)
				rt->keys.is_grayscale = (rt->keys.is_grayscale ? FALSE : TRUE);
			if (e->key.keysym.sym == SDLK_p)
				create_bmp("RT screencap.bmp", &rt->rend.win_buffer);
		}
		else if (e->type == SDL_WINDOWEVENT \
		&& e->window.event == SDL_WINDOWEVENT_CLOSE)
			rt->rend.run = FALSE;
		else if (e->type == SDL_WINDOWEVENT \
		&& e->window.event == SDL_WINDOWEVENT_EXPOSED)
			draw_to_window(rt);
		rt->run = TRUE;
	}
}