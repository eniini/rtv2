/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:58:49 by esukava           #+#    #+#             */
/*   Updated: 2022/06/18 15:23:00 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "SDL.h"
# include "defines.h"
# include "libft.h"
# include "ft_gfx.h"

typedef struct s_object{
	t_fvector	pos;
	t_fvector	dir;
	float		r;
	int			type;
	float		angle;
	t_color		color;
}				t_object;

typedef struct s_ray{
	t_fvector	start;
	t_fvector	dir;
}				t_ray;

typedef struct s_cam{
	t_fvector	pos;
	t_fvector	dir;
	t_fvector	h;
	t_fvector	v;
	t_fvector	rt_start;
}				t_cam;

//Holds everything related directly to SDL's drawbuffer.
typedef struct s_rend
{
	SDL_Renderer	*renderer;
	SDL_Window		*win;
	SDL_Texture		*win_tex;
	void			*win_pixels;
	t_buffer		win_buffer;
	int				win_pixel_pitch;
	t_bool			run;
}					t_rend;

//superstruct that holds all the subsystem structs.
typedef struct s_rt {
	t_rend		rend;
	t_cam		cam;
	t_ray		ray_prime;
	t_ray		ray_light;
	t_object	*object;
	t_object	*light;
	t_color		amb_col;
	t_color		lightcolor;
	float		uv_u;
	float		uv_v;
	uint		objcount;
	uint		lightcount;
	uint		shadow_lvl;
	t_bool		is_grayscale;
	t_bool		redraw;
}				t_rt;

t_fvector	find_object_normal(t_object *object, t_ray *ray);
void		raytracer(t_rt *rt, int x, int y);
void		keyevent(t_rt *rt, SDL_Event *e);
t_bool		draw_light(t_rt *rt, float *t, int x, int y);
void		draw_to_window(t_rt *rt);
void		draw_pixel(uint32_t x, uint32_t y, t_buffer *buf, uint32_t color);
int			create_bmp(char *filename, t_buffer *i);

// ray-primitive intersections

void		quadratic_equation(t_fvector abc, float *t0, float *t1);
t_bool		ray_plane_intersect(t_ray *r, t_object *p, float *t);
t_bool		ray_sphere_intersect(t_ray *r, t_object *s, float *t);
t_bool		ray_cyl_intersect(t_ray *r, t_object *obj, float *result);
t_bool		ray_cone_intersect(t_ray *r, t_object *cone, float *hit_distance);
void		ray_trough_screen(t_rt *rt, int x, int y);
t_bool		ray_object_intersect(t_ray *ray, t_object *obj, float *t);

//color manipulation

uint32_t	col_to_uint(t_color color);
t_color		col_lerp(t_color c1, t_color c2, float p);
t_color		col_blend(t_color base, t_color mix, float p);
t_color		col_multiply(t_color color, float m);
t_color		col_add(t_color base, t_color mix, float p);
t_color		col_sub(t_color base, t_color mix, float p);

uint32_t	convert_to_grayscale(uint32_t c);

//texturing

void		uv_map(t_rt *rt, t_ray *ray, int cur_obj);
t_color		apply_check_pattern(t_rt *rt, float scale, int cur_obj, t_color oc);

//parser

t_fvector	read_3dvec(char *line);
t_color		read_color(char *line);
void		read_cam(t_rt *rt, char *line);
void		read_objcount(t_rt *rt, char *line, t_fvector *values);
void		read_file(t_rt *rt, char *argv);
void		init_sphere(t_object *obj, t_fvector orig, float radius, t_color c);
void		init_plane(t_object *obj, t_fvector orig, t_fvector dir, t_color c);
void		init_cyl(t_object *obj, t_fvector orig, t_fvector dir, t_color c);
void		init_cone(t_object *obj, t_fvector orig, t_fvector dir, t_color c);
void		init_cam(t_rt *rt, t_fvector camdir, float ar, float vfov);
void		init_3d_mods(t_rt *rt, uint obj_n, t_fvector r, t_fvector t);

//matrix multiplication

t_mat4		mm_init_unitmatrix(void);
t_mat4		mm_init_translation(float x, float y, float z);
t_fvector	mm_multiply_vector(t_fvector v, t_mat4 m);
t_mat4		mm_multiply_matrix(t_mat4 ma, t_mat4 mb);

//vector operations

t_fvector	v_cross(t_fvector va, t_fvector vb);
float		v_dot(t_fvector va, t_fvector vb);
float		v_len(t_fvector v);
t_fvector	v_normalize(t_fvector v);
t_fvector	v_add(t_fvector a, t_fvector b);
t_fvector	v_sub(t_fvector a, t_fvector b);
t_fvector	v_div(t_fvector vector, float c);
t_fvector	v_mult(t_fvector vector, float c);

#endif
