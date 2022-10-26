/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 04:23:59 by nlegrand          #+#    #+#             */
/*   Updated: 2022/10/26 11:30:37 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// main function, reads file line by line.
char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	int			len;
	t_line		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	len = 0;
	line = NULL;
	if (read_line(fd, &line, &len, buf) == -1)
		return (clear_line(&line), NULL);
	return (make_line(&line, len));
}

// reads until it finds eol, makes chained list
int	read_line(int fd, t_line **line, int *len, char *buf)
{
	t_line	*tmp;
	t_line	*prev;
	int		ret;
	char	*end;

	*line = read_one(fd, &ret, len, buf);
	if (*line == NULL)
		return (-1);
	prev = *line;
	while (ret)
	{
		end = get_endl(prev->buf);
		if (end && (*end == '\n'))
			break ;
		tmp = read_one(fd, &ret, len, NULL);
		if (tmp == NULL)
			return (-1);
		if (tmp->buf[0] == '\0')
			return (free(tmp), split_buf_rest(prev->buf, buf), 0);
		prev->next = tmp;
		prev = tmp;
	}
	split_buf_rest(prev->buf, buf);
	return (0);
}
