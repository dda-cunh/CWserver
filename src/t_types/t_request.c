#include "../../inc/cwserver.h"


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
			free_2d_str(cookies);
			return (NULL);
		}
		if (cookie[0] && cookie[1])
		{
			if (!cookie_map)
				cookie_map = t_str_map_new(cookie[0], cookie[1]);
			else
				cookie_map->add(&cookie_map, cookie[0], cookie[1]);
		}
		free_2d_str(cookie);
	}
	free_2d_str(cookies);
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

static t_request   *parse_request(t_byte_array *req_bytes)
{
	t_request		*request;
	char			**lines;
	char			**split;
	char			*bytes_str;

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
	bytes_str = byte_arr_to_str(req_bytes);
	if (!bytes_str)
	{
		request->dispose(request);
		return (NULL);
	}
	lines = ut_split(bytes_str, '\n');
	if (!lines)
	{
		request->dispose(request);
		free(bytes_str);
		return (NULL);
	}
	free(bytes_str);
	for (size_t i = 0; lines[i]; i++)
	{
		if (strcmp(lines[i], "\r") == 0)
		{
			request->body = t_byte_array_new();
			i++;
			if (lines[i])
				append_str_to_bytes(request->body, lines[i]);
			break ;
		}
		else if (i == 0)
		{
			split =  ut_split(lines[i], ' ');
			if (!split || !split[0] || !split[1] || !split[2])
			{
				if (split)
					free_2d_str(split);
				t_request_dispose(request);
				ut_puterror("Error: ", "ut_split failed in parse_request");
				return (NULL);
			}
			request->method = get_method(split[0]);
			if (strcmp(split[1], "/") == 0)
				request->path = strdup("/index");
			else
			{
				request->path_extension = strrchr(split[1], '.');
				if (request->path_extension && request->path_extension + 1)
					request->path_extension = strdup(request->path_extension);
				else
					request->path_extension = NULL;
				request->path = strdup(split[1]);
			}
		}
		else
		{
			if (lines[i][strlen(lines[i]) - 1] == '\r')
				lines[i][strlen(lines[i]) - 1] = '\0';
			split = ut_split(lines[i], ':');
			if (!split)
			{
				request->dispose(request);
				free_2d_str(lines);
				return (NULL);
			}
			if (split[0] && split[1])
				if (strcmp(split[0], "Cookie") == 0)
					request->cookies = parse_cookies(split[1]);
		}
		free_2d_str(split);
	}
	free_2d_str(lines);
	return (request);
}

t_request	*read_request(int client_fd, t_server *server)
{
	unsigned char	buffer[SOCK_BUFFER_SIZE];
	t_byte_array	*bytes;
	t_request		*request;
	char			*req_str;
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
		req_str = byte_arr_to_str(bytes);
		if (!req_str)
		{
			bytes->dispose(bytes);
			return (NULL);
		}
		pthread_mutex_lock(server->output_mutex);
		ut_putendl_fd(server->logger_fd, req_str);
		free(req_str);
		pthread_mutex_unlock(server->output_mutex);
		request = parse_request(bytes);
	}
	bytes->dispose(bytes);
	return (request);
}
