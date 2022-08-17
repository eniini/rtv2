/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/28 19:38:04 by eniini            #+#    #+#             */
/*   Updated: 2022/06/18 14:21:18 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
*	We're not handling floats here, instead clamping input to RGB scale and
*	then turning that to a %.
*/
t_color	read_color(char *line)
{
	char	*ptr;
	t_color	c;

	if (!line || *line == '\0')
		ft_getout("Missing color definition");
	ptr = line + 1;
	if (!(*ptr >= '0' && *ptr <= '9'))
		ft_getout("Invalid color definition");
	c.red = ft_clamp_i(ft_atoi(ptr), 0, 255) / 255.0f;
	ptr = ft_strchr(line, ',');
	if (!ptr)
		ft_getout("Invalid color definition! (missing RGB information)");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9'))
		ft_getout("Invalid color definition! (Incorrect symbol)");
	c.green = ft_clamp_i(ft_atoi(ptr), 0, 255) / 255.0f;
	ptr = ft_strchr(ptr, ',');
	if (!ptr)
		ft_getout("Invalid color definition! (missing RGB information)");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9'))
		ft_getout("Invalid color definition! (Incorrect symbol)");
	c.blue = ft_clamp_i(ft_atoi(ptr), 0, 255) / 255.0f;
	return (c);
}

/*
*	Read (x,y,z) values from a character string [line].
*	Only floating point values with a period decimal separator delimited with
*	commas are allowed.
*/
t_fvector	read_3dvec(char *line)
{
	t_fvector	v;
	char		*ptr;

	ptr = line;
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.x = ft_atof(ptr);
	ptr = ft_strchr(line, ',');
	if (!ptr)
		ft_getout("Invalid vector init! (Missing vector Y value");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.y = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',');
	if (!ptr)
		ft_getout("Invalid vector init! (Missing vector Z value");
	ptr = (ptr + 1);
	if (!(*ptr >= '0' && *ptr <= '9') && (*ptr != '-'))
		ft_getout("Invalid vector init! (Incorrect symbol)");
	v.z = ft_atof(ptr);
	v.w = 1.0f;
	return (v);
}

/*
*	Note: as long as we are rendering only a single camera, it doesn't need to
*	be initalized. Multiple initalizations should not break the program, only
*	rewrite previous coordinates.
*/
void	read_cam(t_rt *rt, char *line)
{
	char		*ptr;
	t_fvector	lookfrom;
	t_fvector	lookat;

	ptr = line;
	lookfrom = read_3dvec(ptr);
	ptr = ft_strchr(line, ' ');
	if (!ptr)
		ft_getout("Invalid camera init! (Incorrect delimiter character)");
	lookat = read_3dvec((ptr + 1));
	rt->cam.pos = lookfrom;
	rt->cam.dir = lookat;
	rt->redraw = TRUE;
	ptr = ft_strchr(ptr + 1, ' ');
	if (ptr)
		rt->amb_col = read_color(ptr);
	else
	{
		ft_printf("No ambient light set!\n");
		rt->amb_col = (t_color){0, 0, 0};
	}
}

/*
*	If no number of lights is defined, initialize one into camera's coordinates.
*	Also lots of wrangling to make sure it does not break formatting checks
*	later down the line.
*	Individual light definitions down the line should be ignored with this
*	config as well.
*/
static void	deal_with_nolight(t_rt *rt, t_fvector *values)
{
	ft_printf("No lightcount given, defaulting to camera coordinates...\n");
	rt->lightcount = 1;
	values->w = 1;
	rt->light = (t_object *)malloc(sizeof(t_object));
	if (!rt->light)
		ft_getout("Failed to allocate memory for default light!");
	init_sphere(&rt->light[0], rt->cam.pos, 0.1f, (t_color){1, 1, 1});
}

//	Note: we are not dealing with a chance for malloc(0) here.
void	read_objcount(t_rt *rt, char *line, t_fvector *values)
{
	char	*ptr;

	rt->objcount = ft_atoi(line);
	if (!rt->objcount)
		ft_getout("Invalid object count declaration!");
	rt->object = (t_object *)malloc(sizeof(t_object) * rt->objcount);
	if (!rt->object)
		ft_getout("Failed to allocate memory for objects!");
	ptr = ft_strchr(line, ' ');
	if (!ptr)
	{
		deal_with_nolight(rt, values);
		return ;
	}
	rt->lightcount = ft_atoi(ptr);
	if (rt->lightcount == 0)
		ft_getout("Explicitly defined lightcount must be a nonzero value!");
	rt->light = (t_object *)malloc(sizeof(t_object) * rt->lightcount);
	if (!rt->light)
		ft_getout("Failed to allocate memory for lights!");
}
