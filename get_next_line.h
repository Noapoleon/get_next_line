/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 04:25:54 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/02 07:20:14 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_line
{
	char			buf[BUFFER_SIZE + 1];
	struct s_line	*next;
}				t_line;

char	*get_next_line(int fd);
char	*deplete_buf(char *buf);
int		read_line(int fd, t_line **line, int *len, char *buf);
t_line	*read_one(int fd, int *ret, int *len, char *buf);
char	*make_line(t_line **line, int len);
void	split_buf_rest(char *tmpbuf, char *buf);
void	clear_line(t_line **line);
char	*get_endl(char *buf);

#endif
