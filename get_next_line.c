/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:16:08 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/05/10 19:20:50 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_and_null(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

static ssize_t	read_and_append(int fd, char **line_buf)
{
	char	*buf;
	ssize_t	bytes_read;
	char	*new_line_buf;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		free(buf);
		return (bytes_read);
	}
	buf[bytes_read] = '\0';
	new_line_buf = ft_strjoin(*line_buf, buf);
	free(buf);
	if (!new_line_buf)
		return (-1);
	free(*line_buf);
	*line_buf = new_line_buf;
	return (bytes_read);
}

static char	*extract_and_update_rest(char **line_buf, char *nl_ptr)
{
	char	*line;
	char	*rest;
	size_t	line_len;

	line_len = nl_ptr - *line_buf + 1;
	line = malloc(line_len + 1);
	if (!line)
		return (free_and_null(line_buf));
	ft_memcpy(line, *line_buf, line_len);
	line[line_len] = '\0';
	if (*(nl_ptr + 1))
	{
		rest = ft_strdup(nl_ptr + 1);
		if (!rest)
		{
			free(line);
			return (free_and_null(line_buf));
		}
	}
	else
		rest = NULL;
	free(*line_buf);
	*line_buf = rest;
	return (line);
}

static char	*process_remaining_buffer(char **line_buf)
{
	char	*nl_ptr;
	char	*line;

	if (!line_buf || !*line_buf)
		return (NULL);
	if (**line_buf == '\0')
		return (free_and_null(line_buf));
	nl_ptr = ft_strchr(*line_buf, '\n');
	if (!nl_ptr)
	{
		line = *line_buf;
		*line_buf = NULL;
		return (line);
	}
	return (extract_and_update_rest(line_buf, nl_ptr));
}

char	*get_next_line(int fd)
{
	static char	*line_buf = NULL;
	ssize_t		read_status;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_and_null(&line_buf));
	if (!line_buf)
	{
		line_buf = ft_strdup("");
		if (!line_buf)
			return (NULL);
	}
	read_status = 1;
	while (read_status > 0 && !ft_strchr(line_buf, '\n'))
		read_status = read_and_append(fd, &line_buf);
	if (read_status == -1)
		return (free_and_null(&line_buf));
	return (process_remaining_buffer(&line_buf));
}

// #include <stdio.h>

// int	main(int argc, char **argv)
// {
// 	int		fd;
// 	char	*line;

// 	(void)argc;
// 	(void)argv;
// 	fd = open(argv[1], O_RDONLY);
// 	// fd = 0;
// 	line = NULL;
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 		line = NULL;
// 	}
// 	close(fd);
// 	return (0);
// }
