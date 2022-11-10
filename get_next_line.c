/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 04:23:59 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/10 03:00:51 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// main function, reads file line by line.
char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	int			len;
	t_line		*line;
	char		*tmp;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	tmp = deplete_buf(buf);
	if (tmp != NULL)
		return (tmp);
	len = 0;
	if (read_line(fd, &line, &len, buf) == -1)
		return (clear_line(&line), NULL);
	return (make_line(&line, len));
}

// removes a line from buf if it is complete
char	*deplete_buf(char *buf)
{
	char	*end;
	char	*tmp;
	int		i;

	end = get_endl(buf);
	if (end == NULL || (*end != '\n'))
		return (NULL);
	tmp = malloc(sizeof(char) * (end - buf + 2));
	if (tmp == NULL)
		return (NULL);
	i = 0;
	while (buf[i] != '\n')
	{
		tmp[i] = buf[i];
		++i;
	}
	tmp[i] = '\n';
	tmp[i + 1] = '\0';
	while (*(end + 1))
		*buf++ = *(end++ + 1);
	*buf = '\0';
	return (tmp);
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
