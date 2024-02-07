#include "../../inc/cwserver.h"

static char	*alloc_concat(char *line, char *buff)
{
	size_t	concat_l;
	size_t	concat_i;
	size_t	buff_i;
	char	*concat;

	concat_l = line ? strlen(line) : 0;
	buff_i = 0;
	while (buff_i < FILE_BUFFER_SIZE && !buff[buff_i])
		buff_i++;
	while (buff_i < FILE_BUFFER_SIZE)
	{
		concat_l++;
		if (buff[buff_i] == '\n')
			break ;
		buff_i++;
	}
	concat = malloc((concat_l + 1) * sizeof(char));
	concat_i = 0;
	while (concat_i < concat_l + 1)
	{
		concat[concat_i] = '\0';
		concat_i++;
	}
	return (concat);
}

static char	*line_join(char *line, char *buff)
{
	long	buff_empty_i;
	long	breakline_i;
	long	concat_i;
	char	*concat;

	concat = alloc_concat(line, buff);
	if (!concat)
		return (NULL);
	concat_i = line ? strlen(line) : 0;
	ut_memmove(concat, line, concat_i);
	if (line)
		free(line);
	buff_empty_i = 0;
	while (buff_empty_i < FILE_BUFFER_SIZE && !buff[buff_empty_i])
		buff_empty_i++;
	breakline_i = buff_empty_i;
	while (breakline_i < FILE_BUFFER_SIZE)
		if (buff[breakline_i++] == '\n')
			break ;
	ut_memmove(concat + concat_i, buff + buff_empty_i, breakline_i - buff_empty_i);
	ut_memset(buff + buff_empty_i, 0, breakline_i - buff_empty_i);
	return (concat);
}

static bool	buff_has_residual(char *buffer)
{
	int	i;

	i = -1;
	while (++i < FILE_BUFFER_SIZE)
		if (buffer[i])
			return (true);
	return (false);
}

static bool	str_has_c(const char *str, char c)
{
	int	i;

	if (str)
	{
		i = 0;
		while (str[i])
		{
			if (str[i] == c)
				return (true);
			i++;
		}
	}
	return (false);
}

static char	*seek_line(char *buffer, char *line, long bytes_read, int fd)
{
	if (str_has_c(line, '\n'))
		return (line);
	bytes_read = read(fd, buffer, FILE_BUFFER_SIZE);
	if (bytes_read < 0)
	{
		if (line)
			free(line);
		return (NULL);
	}
	if (bytes_read == 0)
		return (line);
	line = line_join(line, buffer);
	if (!line)
		return (NULL);
	return (seek_line(buffer, line, bytes_read, fd));
}

char	*get_next_line(int fd)
{
	static char	buffer[FILE_BUFFER_SIZE] = {0};
	char		*line;

	line = NULL;
	if (buff_has_residual(buffer))
		line = line_join(line, buffer);
	line = seek_line(buffer, line, 1, fd);
	return (line);
}
