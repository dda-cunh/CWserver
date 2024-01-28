#include "../../inc/cwserver.h"

t_byte_array	*read_all_from_file(int fd)
{
	t_byte_array	*bytes;
	unsigned char	buffer[FILE_BUFFER_SIZE];
	int				read_bytes;

	bytes = t_byte_array_new();
	if (fd < 0)
		return (bytes);
	while ((read_bytes = read(fd, buffer, FILE_BUFFER_SIZE)) > 0)
		append_to_bytes(bytes, (t_byte_array){buffer, read_bytes,
						read_bytes, bytes->dispose});
	close(fd);
	return (bytes);
}

int	open_server_file(char *path)
{
	char	*full_path;
	int		fd;

	full_path = ut_strjoin(ROOT_PATH, path);
	fd = open(full_path, O_RDONLY);
	free(full_path);
	return (fd);
}
