#include "../../inc/cwserver.h"

static char	*get_content_type(char *path)
{
	char	*extension;

	extension = strrchr(path, '.');
	if (extension == NULL)
		return (strdup("text/plain"));
	else if (strcmp(extension, ".html") == 0)
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

t_response	*get(t_request req, t_server server)
{
	t_byte_array	*body;
	char			*content_type;
	char			*status_message;
	int				status_code;
	int				fd;

	body = NULL;
	fd = open_server_file(req.path, O_RDONLY, 0);
	if (fd == -1)
	{
		status_message = strdup("Not Found");
		content_type = strdup("text/plain");
		status_code = 404;
	}
	else
	{
		content_type = get_content_type(req.path);
		if (strcmp(content_type, "text/html") == 0)
			body = build_html(fd, server);
		else
			body = read_all_from_file(fd);
		close(fd);
		status_message = strdup("OK");
		status_code = 200;
	}
	return (t_response_new(body, status_message, content_type, status_code));
}

//--	NON FUNCTIONAL YET --//
t_response	*post(t_request req)
{
	int	fd;

	fd = open_server_file(req.path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (t_response_new(NULL, strdup("Internal Server Error"),
								strdup("text/plain"), 500));
	else
	{
		write(fd, req.body->bytes, req.body->length);
		close(fd);
		return (t_response_new(NULL, strdup("OK"), strdup("text/plain"), 200));
	}
}
