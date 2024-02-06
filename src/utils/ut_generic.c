#include "../../inc/cwserver.h"

/**
*	Executes a PHP script with optional arguments.
*	<p>
*	@param script_path	The path to the PHP script to execute.
*	@param args			An array of arguments to pass to the PHP script.
*	@return				A byte array containing the output of the PHP script.
*	@see				t_byte_array
*/
t_byte_array	*exec_php(char *script_path, char **args)
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
	if (args)
	{
		for (int i = 0; args[i]; i++)
		{
			append_str_to_bytes(exec, " ");
			append_str_to_bytes(exec, args[i]);
		}
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

void	free_split(char **split)
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
