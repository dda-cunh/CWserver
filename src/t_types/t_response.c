#include "../../inc/cwserver.h"

void	dump_response(int client_fd, t_response *response)
{
	t_byte_array	*bytes;
	char			*code_str;

	bytes = t_byte_array_new();
	append_str_to_bytes(bytes, "HTTP/1.1 ");
	code_str = ut_itoa(response->status_code);
	append_str_to_bytes(bytes, code_str);
	free(code_str);
	append_str_to_bytes(bytes, " ");
	append_str_to_bytes(bytes, response->status_message);
	append_str_to_bytes(bytes, "\r\n");
	append_str_to_bytes(bytes, "Content-Type: ");
	append_str_to_bytes(bytes, response->content_type);
	append_str_to_bytes(bytes, "\r\n");
	append_str_to_bytes(bytes, "\r\n");
	if (response->body)
		append_to_bytes(bytes, *(response->body));
	write(client_fd, bytes->bytes, bytes->length);
	bytes->bytes[bytes->length - 1] = '\0';
	ut_putendl_fd(STD_OUT, (char *)bytes->bytes);
	bytes->dispose(bytes);
	response->dispose(response);
}

static void	t_response_dispose(t_response *self)
{
	if (self)
	{
		if (self->body)
			self->body->dispose(self->body);
		if (self->status_message)
			free(self->status_message);
		if (self->content_type)
			free(self->content_type);
		free(self);
	}
}

t_response	*t_response_new(t_byte_array *body, char *status_message,
							char *content_type, int status_code)
{
	t_response	*response;

	response = (t_response *)malloc(sizeof(t_response));
	if (!response)
		return (NULL);
	response->body = body;
	response->status_message = status_message;
	response->content_type = content_type;
	response->status_code = status_code;
	response->dispose = t_response_dispose;
	return (response);
}

t_response	*parse_response(t_request request)
{
	t_byte_array	*body;
	char			*status_message;
	char			*content_type;
	char			*path;
	int				status_code;
	int				fd;

	body = NULL;
	if (request.path == NULL)
	{
		status_message = strdup("Bad Request");
		content_type = strdup("text/plain");
		status_code = 400;
	}
	else
	{
		if (strcmp(request.path, "/") == 0)
			path = ut_strjoin(ROOT_PATH, "/index.html");
		else
			path = ut_strjoin(ROOT_PATH, request.path);
		if (request.method == HTTP_GET)
		{
			fd = open(path, O_RDONLY);
			free(path);
			if (fd == -1)
			{
				status_message = strdup("Not Found");
				content_type = strdup("text/plain");
				status_code = 404;
			}
			else
			{
				body = read_all_from_file(fd);
				close(fd);
				status_message = strdup("OK");
				content_type = strdup("text/html");
				status_code = 200;
			}
		}
		else if (request.method == HTTP_POST)
		{
			fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
			free(path);
			if (fd == -1)
			{
				status_message = strdup("Internal Server Error");
				content_type = strdup("text/plain");
				status_code = 500;
			}
			else
			{
				write(fd, request.body->bytes, request.body->length);
				close(fd);
				status_message = strdup("OK");
				content_type = strdup("text/plain");
				status_code = 200;
			}
		}
		else
		{
			status_message = strdup("Method Not Allowed");
			content_type = strdup("text/plain");
			status_code = 405;
			free(path);
		}
	}
	return (t_response_new(body, status_message, content_type, status_code));
}
