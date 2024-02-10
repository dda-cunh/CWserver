#include "../../inc/cwserver.h"

static char	*get_content_type(char *extension)
{
	if (extension == NULL || strcmp(extension, ".html") == 0)
		return (strdup("text/html"));
	else if (strcmp(extension, ".css") == 0)
		return (strdup("text/css"));
	else if (strcmp(extension, ".js") == 0)
		return (strdup("text/javascript"));
	else if (strcmp(extension, ".jpg") == 0)
		return (strdup("image/jpeg"));
	else if (strcmp(extension, ".jpeg") == 0)
		return (strdup("image/jpeg"));
	else if (strcmp(extension, ".png") == 0)
		return (strdup("image/png"));
	else if (strcmp(extension, ".gif") == 0)
		return (strdup("image/gif"));
	else if (strcmp(extension, ".ico") == 0)
		return (strdup("image/x-icon"));
	else if (strcmp(extension, ".mp4") == 0)
		return (strdup("video/mp4"));
	else if (strcmp(extension, ".mp3") == 0)
		return (strdup("audio/mpeg"));
	else
		return (strdup("text/plain"));
}

void	get(t_request req, int client)
{
	t_byte_array	*body;
	t_str_map		*args;
	char			*content_type;
	char			*client_fd_str;
	char			*new_path;
	int				fd;

	body = NULL;
	content_type = get_content_type(req.path_extension);
	if (!req.path_extension)
	{
		new_path = ut_strjoin(req.path, ".php");
		fd = open_server_file(new_path, O_RDONLY, 0);
		if (fd == -1)
			put_not_found(client);
		else
		{
			close(fd);
			client_fd_str = ut_itoa(client);
			args = t_str_map_new("CLIENT_FD", client_fd_str);
			free(client_fd_str);
			args->add(&args, "REQUEST_METHOD", "GET");
			if (req.cookies)
				args->link(&args, req.cookies->clone(req.cookies));
			put_str_response(client, "HTTP/1.1 200 OK");
			ut_putstr_fd(client, "Content-Type: ");
			put_str_response(client, content_type);
			body = exec_php(new_path, args);
			args->dispose(args);
			put_str_response(client, "");
			put_bytes_response(client, body);
			body->dispose(body);
		}
		free(new_path);
	}
	else if (strcmp(req.path_extension, ".php") == 0)
		put_not_found(client);
	else
	{
		fd = open_server_file(req.path, O_RDONLY, 0);
		if (fd == -1)
			put_not_found(client);
		else
		{
			body = read_all_from_file(fd);
			if (body == NULL)
				put_server_error(client);
			else
			{
				put_str_response(client, "HTTP/1.1 200 OK");
				ut_putstr_fd(client, "Content-Type: ");
				put_str_response(client, content_type);
				put_str_response(client, "");
				put_bytes_response(client, body);
				body->dispose(body);
			}
			close(fd);
		}
	}
	free(content_type);
}

void	post(t_request req, int client)
{
	t_byte_array	*body;
	t_str_map		*args;
	t_str_map		*form;
	char			*client_fd_str;
	char			*content_type;
	char			*body_str;
	char			*new_path;

	body = NULL;
	content_type = get_content_type(req.path_extension);
	if (!req.path_extension)
	{
		client_fd_str = ut_itoa(client);
		args = t_str_map_new("CLIENT_FD", client_fd_str);
		free(client_fd_str);
		args->add(&args, "REQUEST_METHOD", "POST");
		if (req.cookies)
			args->link(&args, req.cookies->clone(req.cookies));
		body_str = byte_arr_to_str(req.body);
		form = map_from_form(body_str);
		free(body_str);
		args->link(&args, form);
		put_str_response(client, "HTTP/1.1 200 OK");
		ut_putstr_fd(client, "Content-Type: ");
		put_str_response(client, content_type);
		free(content_type);
		new_path = ut_strjoin(req.path, ".php");
		body = exec_php(new_path, args);
		args->dispose(args);
		free(new_path);
		put_str_response(client, "");
		if (body)
		{
			put_bytes_response(client, body);
			body->dispose(body);
		}
	}
	else
		put_not_found(client);
}
