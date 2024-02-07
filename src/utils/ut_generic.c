#include "../../inc/cwserver.h"

static char	*url_decode(char *str)
{
	size_t	str_len;
	size_t	i;
	size_t	j;
	char	*decoded_str;
	char	*decoded;

	if (!str)
		return (NULL);
	str_len = strlen(str);
	decoded = (char *)malloc(str_len + 1);
	if (!decoded)
		return (NULL);
	i = 0;
	j = 0;
	while (i < str_len)
	{
		if (str[i] == '%')
		{
			decoded_str = strndup(str + i + 1, 2);
			decoded[j] = (char)strtol(decoded_str, NULL, 16);
			free(decoded_str);
			i += 3;
		}
		else if (str[i] == '+')
			decoded[j] = ' ';
		else
			decoded[j] = str[i];
		i++;
		j++;
	}
	decoded[j] = '\0';
	return (decoded);
}

t_str_map	*url_encoded_to_args(t_byte_array *body)
{
	t_str_map	*args;
	char		**split;
	char		**pair;
	char		*decoded;
	char		*body_str;

	if (!body)
		return (NULL);
	body_str = byte_arr_to_str(body);
	if (!body_str)
		return (NULL);
	split = ut_split(body_str, '&');
	free(body_str);
	if (!split)
	{
		ut_puterror("Error: ", "ut_split failed in url_encoded_to_args");
		return (NULL);
	}
	args = NULL;
	for (int i = 0; split[i]; i++)
	{
		pair = ut_split(split[i], '=');
		if (!pair)
		{
			ut_puterror("Error: ", "ut_split failed in url_encoded_to_args");
			args->dispose(args);
			free_2d_str(split);
			return (NULL);
		}
		decoded = url_decode(pair[1]);
		if (decoded)
		{
			if (args == NULL)
				args = t_str_map_new(pair[0], decoded);
			else
				args->add(&args, pair[0], decoded);
			free(decoded);
		}
		free_2d_str(pair);
	}
	free_2d_str(split);
	return (args);
}

/**
*	Executes a PHP script with optional arguments.
*	<p>
*	@param script_path	The path to the PHP script to execute.
*	@param args			An array of arguments to pass to the PHP script.
*	@return				A byte array containing the output of the PHP script.
*	@see				t_byte_array
*/
t_byte_array	*exec_php(char *script_path, t_str_map *args)
{
	unsigned char	buffer[FILE_BUFFER_SIZE];
	t_byte_array	*bytes;
	t_byte_array	*exec;
	FILE			*file;
	char			*bin;
	int				read_bytes;

	bytes = t_byte_array_new();
	exec = t_byte_array_new();
	append_str_to_bytes(exec, "php ");
	append_str_to_bytes(exec, ROOT_PATH);
	append_str_to_bytes(exec, script_path);
	while (args)
	{
		append_str_to_bytes(exec, " ");
		append_str_to_bytes(exec, args->key);
		append_str_to_bytes(exec, "=");
		append_str_to_bytes(exec, args->value);
		args = args->next;
	}
	bin = byte_arr_to_str(exec);
	exec->dispose(exec);
	file = popen(bin, "r");
	free(bin);
	if (file == NULL)
		return (bytes);
	while ((read_bytes = fread(buffer, 1, FILE_BUFFER_SIZE, file)) > 0)
		append_to_bytes(bytes, (t_byte_array){buffer, read_bytes,
						read_bytes, NULL});
	pclose(file);
	return (bytes);
}

void	free_2d_str(char **split)
{
	for (int i = 0; split[i]; i++)
		free(split[i]);
	free(split);
}

/**
*	Reads all data from a file descriptor and returns it as a byte array.
*	<p>
*	@param fd	The file descriptor to read from.
*	@return		A dynamically allocated byte array containing the data read from
					the file.
*	@see		t_byte_array
*/
t_byte_array	*read_all_from_file(int fd)
{
	unsigned char buffer[FILE_BUFFER_SIZE];
	t_byte_array *bytes;
	int read_bytes;

	bytes = t_byte_array_new();
	if (fd < 0)
		return (bytes);
	while ((read_bytes = read(fd, buffer, FILE_BUFFER_SIZE)) > 0)
		append_to_bytes(bytes, (t_byte_array){buffer, read_bytes, read_bytes, NULL});
	close(fd);
	return (bytes);
}

/**
*	Opens a server file with the specified path, open flags, and permissions.
*	<p>
*	@param path			The path of the file to open.
*	@param open_flags	The flags used for opening the file (e.g., O_RDONLY, O_WRONLY, O_RDWR).
*	@param permissions	The permissions to set for the file (e.g., 0644).
*	@return				The file descriptor of the opened file, or -1 if an error
							occurred.
*/
int open_server_file(char *path, int open_flags, int permissions)
{
	char *full_path;
	int fd;

	full_path = ut_strjoin(ROOT_PATH, path);
	fd = open(full_path, open_flags, permissions);
	free(full_path);
	return fd;
}

/**
*	Checks if a given file descriptor is valid.
*	<p>
*	@param fd			The file descriptor.
*	@return				True if valid.
*/
bool	is_valid_fd(int fd)
{
	return (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
}
