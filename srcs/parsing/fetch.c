/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   fetch.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: ghazette <ghazette@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/06 12:08:00 by mkulhand     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/06 13:04:22 by ghazette    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../includes/rtv1.h"

static int		type_define(char *type, t_obj *obj)
{
	if (!ft_strcmp("SPHERE", type))
	{
		obj->render_func = render_sphere;
		return (SPHERE);
	}
	if (!ft_strcmp("CYLINDER", type))
	{
		obj->render_func = render_cylinder;
		vector3d(&obj->dir, 0, 1, 0);
		return (CYLINDER);
	}
	if (!ft_strcmp("CONE", type))
	{
		obj->render_func = render_cone;
		vector3d(&obj->dir, 0, 1, 0);
		return (CONE);
	}
	if (!ft_strcmp("PLANE", type))
	{
		obj->render_func = render_plane;
		vector3d(&obj->dir, 0, 0, 1);
		return (PLANE);
	}
	return (0);
}

int			init_shape(t_shape *shape, char *str)
{
	char **split;
	char **vsplit;

	split = ft_strsplit(str, ';');
	shape->len = ft_heightlen(split);
	if (shape->len >= 3)
	{
		vsplit = ft_strsplit(split[0], '_');
		shape->v0.x = ft_atoi(vsplit[0]);
		shape->v0.y = ft_atoi(vsplit[1]);
		shape->v0.z = ft_atoi(vsplit[2]);
		vsplit = ft_strsplit(split[1], '_');
		shape->v1.x = ft_atoi(vsplit[0]);
		shape->v1.y = ft_atoi(vsplit[1]);
		shape->v1.z = ft_atoi(vsplit[2]);
		vsplit = ft_strsplit(split[2], '_');
		shape->v2.x = ft_atoi(vsplit[0]);
		shape->v2.y = ft_atoi(vsplit[1]);
		shape->v2.z = ft_atoi(vsplit[2]);
	}
	return (1);
}

int				calc_n(t_obj **obj)
{
	vec3_sub(&(*obj)->shape.v1, &(*obj)->shape.v0, &(*obj)->shape.e0);
	vec3_sub(&(*obj)->shape.v2, &(*obj)->shape.v1, &(*obj)->shape.e1);
	vec3_sub(&(*obj)->shape.v0, &(*obj)->shape.v2, &(*obj)->shape.e2);
	vec3_crossproduct(&(*obj)->shape.e0, &(*obj)->shape.e1, &(*obj)->dir);
	vec3_normalize(&(*obj)->dir);
	return (1);
}

static int		fetch_object_array_help(t_obj *obj, char **split)
{
	if (!ft_strcmp(split[0], "name"))
		if (split[1] && !obj->name)
		{
			if (ft_strlen(split[1]) > 10)
				split[1][10] = '\0';
			if (!(obj->name = ft_strdup(split[1])))
				return (0);
		}
	if (!ft_strcmp(split[0], "radius"))
		obj->radius = ft_atoi(split[1]);
	if (!ft_strcmp(split[0], "specular"))
	{
		if (ft_heightlen(split) == 4)
		{
			if (!(init_vec(&obj->material.specular, split)))
				return (0);
			vec3_normalizeby(&obj->material.specular, 255);
		}
	}
	if (!ft_strcmp(split[0], "ambient"))
		if ((obj->material.ambient = (double)ft_atoi(split[1]) / 100.0) < 0)
			return (0);
	if (!ft_strcmp(split[0], "vertex"))
	{
		init_shape(&obj->shape, split[1]);
		calc_n(&obj);
		printf("%s\n", parse_vec(obj->dir));
	}
	ft_free2d(&split);
	return (1);
}

static int		fetch_object_array(t_obj *obj, char **split)
{
	if (!ft_strcmp(split[0], "position"))
		if (!(init_vec(&obj->pos, split)))
			return (0);
	if (!ft_strcmp(split[0], "rotation") && obj->type != 0)
	{
		if (!(init_vec(&obj->rot, split)))
			return (0);
	}
	if (!ft_strcmp(split[0], "color"))
	{
		if (!(init_vec(&obj->color, split)))
			return (0);
		vec3_normalizeby(&obj->color, 255);
	}
	if (!ft_strcmp(split[0], "type") && split[1])
		if (!(obj->type = type_define(split[1], obj)))
			return (0);
	return (fetch_object_array_help(obj, split));
}

static int		check_object(t_obj *obj)
{
	if (!obj->name)
		if (!(obj->name = ft_strdup("NONE")))
			return (0);
	if (!obj->type)
		return (0);
	return (1);
}

int				fetch_object(t_mlx *mlx, int fd)
{
	char *line;

	while (get_next_line(fd, &line))
	{
		mlx->line_cnt++;
		if (!ft_strcmp(line, "{"))
			if (!(mlx->scene->objs[mlx->scene->nb_obj] = new_object()))
				return (0);
		if (!ft_strcmp(line, "}"))
		{
			mlx->scene->objs[mlx->scene->nb_obj]->id = mlx->scene->nb_obj;
			if (!check_object(mlx->scene->objs[mlx->scene->nb_obj]))
				return (0);
			if (mlx->scene->objs[mlx->scene->nb_obj]->type >= 0x101)
				rotate(mlx->scene->objs[mlx->scene->nb_obj]);
			mlx->scene->nb_obj++;
			ft_strdel(&line);
			return (1);
		}
		if (!ft_strcmp(line, ""))
			return (0);
		if (!fetch_object_array(mlx->scene->objs[mlx->scene->nb_obj],
			ft_strsplit(line, ' ')))
			return (0);
		ft_strdel(&line);
	}
	return (0);
}
