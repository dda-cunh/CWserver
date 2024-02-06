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

t_response	*get(t_request req)
{
	t_byte_array	*body;
	char			*content_type;
	char			*new_path;
	int				fd;

	body = NULL;
	content_type = get_content_type(req.path_extension);
	if (!req.path_extension)
	{
		new_path = ut_strjoin(req.path, ".php");
		body = exec_php(new_path, NULL);
		free(new_path);
	}
	else if (strcmp(req.path_extension, ".php") == 0)
		return (t_response_new(NULL, strdup("Not Found"), strdup("text/html"), 404));
	else
	{
		fd = open_server_file(req.path, O_RDONLY, 0);
		if (fd == -1)
			return (t_response_new(NULL, strdup("Not Found"), strdup("text/html"), 404));
		body = read_all_from_file(fd);
		close(fd);
	}
	return (t_response_new(body, strdup("OK"), content_type, 200));
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
