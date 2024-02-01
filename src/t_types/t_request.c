#include "../../inc/cwserver.h"

static unsigned char	*seek_carr_ret(unsigned char *bytes, size_t start, size_t len)
{
	while (start < len - 1)
	{
		if (bytes[start] == '\r' && bytes[start + 1] == '\n')
		{
			bytes[start] = '\0';
			return (bytes + start);
		}
		start++;
	}
	return (NULL);
}

static t_http_method	get_method(char *method_str)
{
	if (strcmp(method_str, "GET") == 0)
		return (HTTP_GET);
	else if (strcmp(method_str, "POST") == 0)
		return (HTTP_POST);
	return (HTTP_UNKNOWN);
}

static void	t_request_dispose(t_request *self)
{
	if (self)
	{
		if (self->body)
			self->body->dispose(self->body);
		if (self->path)
			free(self->path);
		free(self);
	}
}

t_request   *parse_request(t_byte_array *req_bytes)
{
	unsigned char	*line;
	t_request   	*request;
	size_t	  		i;
	char			**split;

	request = (t_request *)malloc(sizeof(t_request));
	if (!request)
	{
		ut_puterror("Error: ", "malloc failed in parse_request");
		return (NULL);
	}
	request->method = HTTP_UNKNOWN;
	request->body = NULL;
	request->path = NULL;
	request->dispose = t_request_dispose;
	i = -1;
	while (++i < req_bytes->length)
	{
		line = seek_carr_ret(req_bytes->bytes, i, req_bytes->length);
		if (line == req_bytes->bytes + i)
		{
			if (i + 2 < req_bytes->length)
			{
				request->body = t_byte_array_new();
				append_to_bytes(request->body, (t_byte_array)
												{(req_bytes->bytes + i + 2),
												req_bytes->length - i - 2, 
												req_bytes->length - i - 2,
												NULL});
			}
			break ;
		}
		if (i == 0)
		{
			split = ut_split((char *)req_bytes->bytes, ' ');
			if (!split)
			{
				t_request_dispose(request);
				ut_puterror("Error: ", "ut_split failed in parse_request");
				return (NULL);
			}
			if (split[0])
			{
				request->method = get_method(split[0]);
				if (split[1])
				{
					if (strcmp(split[1], "/") == 0)
						request->path = strdup("/index.html");
					else
						request->path = strdup(split[1]);
				}
			}
			for (int j = 0; split[j]; j++)
				free(split[j]);
			free(split);
		}
		// else
		// {
		// 	split = ut_split((char *)req_bytes->bytes + i, ':');
		// 	if (!split)
		// 	{
		// 		free(request);
		// 		return (NULL);
		// 	}
		// 	if (strcmp(split[0], "Host") == 0)
		// 		request->host = split[1];
		// 	else if (strcmp(split[0], "Content-Length") == 0)
		// 		request->content_length = atol(split[1]);
		// 	else if (strcmp(split[0], "Content-Type") == 0)
		// 		request->content_type = split[1];
		// }
	}
	return (request);
}

t_request	*read_request(int client_fd)
{
	unsigned char	buffer[SOCK_BUFFER_SIZE];
	t_byte_array	*bytes;
	t_request		*request;
	long			bytes_read;

	request = NULL;
	ut_memset(buffer, 0, SOCK_BUFFER_SIZE);
	bytes = t_byte_array_new();
	bytes_read = read(client_fd, buffer, SOCK_BUFFER_SIZE);
	if (bytes_read == -1)
	{
		ut_puterror("Server error", strerror(errno));
		bytes->dispose(bytes);
		return (NULL);
	}
	else
	{
		append_to_bytes(bytes, (t_byte_array){buffer, bytes_read, bytes_read, NULL});
		request = parse_request(bytes);
	}
	bytes->dispose(bytes);
	return (request);
}
