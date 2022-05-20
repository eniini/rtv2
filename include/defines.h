/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:56:17 by esukava           #+#    #+#             */
/*   Updated: 2022/05/20 20:42:58 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

//WINDOW
# define WIN_W	800
# define WIN_H	600
# define WIN_HW 400
# define WIN_HH 300
# define WIN_NAME	"esukava/eniini interactive raytracer ver. 0.1"

# define ROUGHNESS		120.0f
# define RAY_LIMIT		20000.0f
# define RAY_DEPTH		3
# define REFRAC_FALLOFF	5.0f

typedef struct s_fvector {
	float	x;
	float	y;
	float	z;
}			t_fvector;
typedef t_fvector	t_fvec3;

//Datatype to store an ARBG image data. Each pixel can be found by calculating
//[(h * y) + (x % w)]
typedef struct s_buffer {
	uint32_t	*px;
	uint32_t	w;
	uint32_t	h;
}				t_buffer;

#endif