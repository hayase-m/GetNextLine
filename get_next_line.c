/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmaruyam <hmaruyam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:16:08 by hmaruyam          #+#    #+#             */
/*   Updated: 2025/04/28 21:45:21 by hmaruyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	append_buf(char **line_buf, const char *buf)
{
	char	*new_line_buf;

	new_line_buf = ft_strjoin(*line_buf, buf);
	if (!new_line_buf)
		return (0);
	free(*line_buf);
	*line_buf = new_line_buf;
	return (1);
}

static ssize_t	read_until_newline(int fd, char **line_buf)
{
	char	*buf;
	ssize_t	bytes_read;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	while (!ft_strchr(*line_buf, '\n'))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == 0 || bytes_read == -1)
		{
			free(buf);
			return (bytes_read);
		}
		buf[bytes_read] = '\0';
		if (!append_buf(line_buf, buf))
		{
			free(buf);
			return (-1);
		}
	}
	free(buf);
	return (1);
}

static int	make_rest(char *nl_ptr, char **line_buf)
{
	char	*tmp;

	if (*(nl_ptr + 1))
	{
		tmp = ft_strdup(nl_ptr + 1);
		if (!tmp)
		{
			free(*line_buf);
			*line_buf = NULL;
			return (0);
		}
		free(*line_buf);
		*line_buf = tmp;
	}
	else
	{
		free(*line_buf);
		*line_buf = NULL;
	}
	return (1);
}

static char	*extract_line(char **line_buf)
{
	char	*nl_ptr;
	char	*result;
	size_t	line_len;

	nl_ptr = ft_strchr(*line_buf, '\n');
	if (!nl_ptr)
	{
		result = *line_buf;
		*line_buf = NULL;
		return (result);
	}
	line_len = nl_ptr - *line_buf + 1;
	result = malloc(line_len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, *line_buf, line_len);
	result[line_len] = '\0';
	if (!make_rest(nl_ptr, line_buf))
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*line_buf = NULL;
	ssize_t		read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!line_buf)
	{
		line_buf = ft_strdup("");
		if (!line_buf)
			return (NULL);
	}
	read_result = read_until_newline(fd, &line_buf);
	if (read_result == -1 || (read_result == 0 && line_buf[0] == '\0'))
	{
		free(line_buf);
		line_buf = NULL;
		return (NULL);
	}
	return (extract_line(&line_buf));
}

// #include <stdio.h>

// int	main(int argc, char **argv)
// {
// 	int		fd;
// 	char	*line;

// 	(void)argc;
// 	fd = open(argv[1], O_RDONLY);
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
