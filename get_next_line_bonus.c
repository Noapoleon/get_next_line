/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 04:23:59 by nlegrand          #+#    #+#             */
/*   Updated: 2022/10/26 12:30:13 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

// main function, reads file line by line.
char	*get_next_line(int fd)
{
	static t_fds	*fds;
	char			*buf;
	int				len;
	t_line			*line;

	buf = get_fd_buf(fd, &fds);
	if (buf == NULL)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	len = 0;
	line = NULL;
	if (read_line(fd, &line, &len, buf) == -1)
		return (clear_line(&line), NULL);
	return (make_line(&line, len));
}

// get corresponding fd buffer from chained list, creates one if non-existant
char	*get_fd_buf(int fd, t_fds **fds)
{
	t_fds	*curr;
	t_fds	*prev;
	int		i;

	curr = *fds;
	while (curr && (curr->fd != fd))
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
	{
		curr = malloc(sizeof(t_fds));
		curr->fd = fd;
		curr->next = NULL;
		i = 0;
		while (i < BUFFER_SIZE + 1)
			curr->buf[i++] = '\0';
		if (*fds == NULL)
			*fds = curr;
		else
			prev->next = curr;
		return (curr->buf);
	}
	return (curr->buf);
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
