#include "../../inc/cwserver.h"
#include <string.h>

static size_t	seek_carr_ret(unsigned char *bytes, size_t start, size_t len)
{
	int	transversed;

	transversed = 0;
	while (transversed + start < len - 1)
	{
		if (bytes[transversed + start] == '\r'
			&& bytes[transversed + start + 1] == '\n')
		{
			bytes[transversed + start] = '\0';
			break ;
		}
		transversed++;
	}
	return (transversed);
}

static t_str_map	*parse_cookies(char *cookies_line)
{
	t_str_map	*cookie_map;
	char		**cookies;
	char		**cookie;

	cookies = ut_split(cookies_line, ';');
	if (!cookies)
	{
		ut_puterror("Error: ", "ut_split failed in parse_cookies");
		return (NULL);
	}
	cookie_map = NULL;
	for (int i = 0; cookies[i]; i++)
	{
		cookie = ut_split(cookies[i], '=');
		if (!cookie)
		{
			ut_puterror("Error: ", "ut_split failed in parse_cookies");
			cookie_map->dispose(cookie_map);
			free_split(cookies);
			return (NULL);
		}
		if (cookie[0] && cookie[1])
			cookie_map->add(cookie_map, cookie[0], cookie[1]);
		free_split(cookie);
	}
	free_split(cookies);
	return (cookie_map);
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
		if (self->path_extension)
			free(self->path_extension);
		if (self->cookies)
			self->cookies->dispose(self->cookies);
		free(self);
	}
}

t_request   *parse_request(t_byte_array *req_bytes)
{
	t_request		*request;
	size_t			line_end;
	size_t			i;
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
	request->cookies = NULL;
	request->path_extension = NULL;
	request->dispose = t_request_dispose;
	i = 0;
	while (i < req_bytes->length)
	{
		line_end = seek_carr_ret(req_bytes->bytes, i, req_bytes->length);
		if (line_end == 2)
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
			split = ut_split((char *)(req_bytes->bytes + i), ' ');
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
						request->path = strdup("/index");
					else
					{
						request->path_extension = strrchr(split[1], '.');
						if (request->path_extension && request->path_extension + 1)
							request->path_extension = strdup(request->path_extension + 1);
						else
							request->path_extension = NULL;
						request->path = strdup(split[1]);
					}
				}
			}
		}
		else
		{
			split = ut_split((char *)(req_bytes->bytes + i), ':');
			if (!split)
			{
				request->dispose(request);
				return (NULL);
			}
			if (split[0] && split[1])
				if (strcmp(split[0], "Cookie") == 0)
					request->cookies = parse_cookies(split[1]);
		}
		free_split(split);
		i += line_end;
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
