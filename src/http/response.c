#include "../../inc/cwserver.h"

void	put_response(int client_fd, char *line)
{
	char	*carr_ret_line;

	if (client_fd == -1 || !line)
		return ;
	carr_ret_line = ut_strjoin(line, "\r\n");
	if (!carr_ret_line)
		return ;
	ut_putstr_fd(client_fd, carr_ret_line);
	free(carr_ret_line);
}

void	put_not_found(int client)
{
	put_response(client, "HTTP/1.1 404 Not Found");
	put_response(client, "Content-Type: text/html");
	put_response(client, "");
	put_response(client, "<html><body><h1>404 Not Found</h1></body></html>");
}

void	put_server_error(int client)
{
	put_response(client, "HTTP/1.1 500 Internal Server Error");
	put_response(client, "Content-Type: text/html");
	put_response(client, "");
	put_response(client, "<html><body><h1>500 Internal Server Error</h1></body></html>");
}

void	handle_response(t_request request, int client_fd)
{
	if (request.path == NULL)
		put_not_found(client_fd);
	if (request.method == HTTP_GET)
		get(request, client_fd);
	else if (request.method == HTTP_POST)
		post(request, client_fd);
	else
		put_server_error(client_fd);
}
