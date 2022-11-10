/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 04:23:59 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/10 02:43:46 by nlegrand         ###   ########.fr       */
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
	char			*tmp;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	buf = get_fd_buf(fd, &fds);
	if (buf == NULL)
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

// get corresponding fd buffer from chained list, creates one if non-existant
char	*get_fd_buf(int fd, t_fds **fds)
{
	t_fds	*curr;
	t_fds	*prev;

	curr = *fds;
	while (curr && (curr->fd != fd))
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
	{
		curr = malloc(sizeof(t_fds));
		if (curr == NULL)
			return (NULL);
		curr->fd = fd;
		curr->next = NULL;
		fd = 0;
		while (fd < BUFFER_SIZE + 1)
			curr->buf[fd++] = '\0';
		if (*fds == NULL)
			*fds = curr;
		else
			prev->next = curr;
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
