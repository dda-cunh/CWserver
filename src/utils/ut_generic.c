#include "../../inc/cwserver.h"

t_byte_array	*build_html(int fd, t_server server)
{
	t_byte_array	*html;
	t_byte_array	*read;

	html = t_byte_array_new();
	append_to_bytes(html,  *(server.html_top));
	read = read_all_from_file(fd);
	if (read == NULL)
	{
		html->dispose(html);
		return (NULL);
	}
	append_to_bytes(html, *read);
	read->dispose(read);
	append_to_bytes(html, *(server.html_bottom));
	return (html);
}

t_byte_array	*read_all_from_file(int fd)
{
	unsigned char	buffer[FILE_BUFFER_SIZE];
	t_byte_array	*bytes;
	int				read_bytes;

	bytes = t_byte_array_new();
	if (fd < 0)
		return (bytes);
	while ((read_bytes = read(fd, buffer, FILE_BUFFER_SIZE)) > 0)
		append_to_bytes(bytes, (t_byte_array){buffer, read_bytes,
						read_bytes, NULL});
	close(fd);
	return (bytes);
}

int	open_server_file(char *path, int open_flags, int permissions)
{
	char	*full_path;
	int		fd;

	full_path = ut_strjoin(ROOT_PATH, path);
	fd = open(full_path, open_flags, permissions);
	free(full_path);
	return (fd);
}
