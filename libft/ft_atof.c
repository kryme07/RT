/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_atof.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mkulhand <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/11 12:41:47 by mkulhand     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/11 12:41:48 by mkulhand    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "includes/libft.h"

double		ft_atof(const char *str)
{
	double	ret;
	char	**split;
	int		i;
	double	expo;

	if ((split = ft_strsplit(str, '.')) != NULL)
	{
		ret = ft_atoi(split[0]);
		i = 0;
		expo = 10;
		if (!split[1])
		{
			ft_free2d(&split);
			return (ret);
		}
		while (split[1][i])
		{
			ret += (double)(split[1][i] - '0') / expo;
			i++;
			expo *= 10;
		}
		ft_free2d(&split);
		return (ret);
	}
	return (0);
}