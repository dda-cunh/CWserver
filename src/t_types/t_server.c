#include "../../inc/cwserver.h"

static void	server_up(t_server *server)
{
	int	addr_len;
	int	client;

	addr_len = sizeof(server->address);
	ut_putendl_fd(STD_OUT, "Server is up");
	while (TRUE)
	{
		unsigned char	buffer[BUFFER_SIZE] = {0};
		ut_putendl_fd(STD_OUT, "Waiting for connection...");
		client = accept(server->socket,
						(struct sockaddr *)&server->address,
						(socklen_t *)&(addr_len));
		if (client == -1)
		{
			ut_puterror("Server error", strerror(errno));
			continue ;
		}
		ut_putendl_fd(STD_OUT, "Client connected");
		read(client, buffer, BUFFER_SIZE);
		ut_putendl_fd(1, (char *)buffer);
		send_response(client, (t_response){(t_byte_array){(unsigned char *)"HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>", 90}, "OK", "text/html", 200});
		close(client);
		return ;
	}
}

void	t_server_del(t_server *server)
{
	if (server->socket != -1)
		close(server->socket);
	free(server);
}

t_server	*t_server_new(unsigned long interface, int domain, int protocol,
							int service, int port, int backlog)
{
	t_server	*server;

	server = (t_server *)malloc(sizeof(t_server));
	if (server == NULL)
		return (NULL);
	server->interface = interface;
	server->domain = domain;
	server->protocol = protocol;
	server->service = service;
	server->port = port;
	server->backlog = backlog;
	server->address.sin_family = domain;
	server->address.sin_addr.s_addr = htonl(interface);
	server->address.sin_port = htons(port);
	server->socket = socket(domain, service, protocol);
	if (server->socket == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		t_server_del(server);
		return (NULL);
	}
	if (bind(server->socket, (struct sockaddr *)&server->address, sizeof(server->address)) == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		t_server_del(server);
		return (NULL);
	}
	if (listen(server->socket, backlog) == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		t_server_del(server);
		return (NULL);
	}
	server->up = server_up;
	return (server);
}
