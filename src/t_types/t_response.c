#include "../../inc/cwserver.h"

void	dump_response(int client_fd, t_response *response, t_server server)
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
	ut_putendl_fd(server.logger_fd, "Response sent");
	ut_putendl_fd(server.logger_fd, response->content_type);
	ut_putstr_fd(server.logger_fd, LOG_PACK);
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
	if (request.path == NULL)
		return (t_response_new(NULL, strdup("Internal Server Error"),
								strdup("text/plain"), 500));
	if (request.method == HTTP_GET)
		return (get(request));
	else if (request.method == HTTP_POST)
		return (post(request));
	else
		return (t_response_new(NULL, strdup("Method Not Allowed"),
								strdup("text/plain"), 405));
}
