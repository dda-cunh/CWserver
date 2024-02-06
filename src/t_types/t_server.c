#include "../../inc/cwserver.h"

static void	server_up(t_server *server)
{
	t_request	*request;
	int			addr_len;
	int			client;

	addr_len = sizeof(server->address);
	ut_putendl_fd(STD_OUT, ANSI_COLOR_GREEN);
	ut_putendl_fd(STD_OUT, "Server is up");
	ut_putendl_fd(STD_OUT, ANSI_COLOR_RESET);
	while (TRUE)
	{
		request = NULL;
		ut_putstr_fd(server->logger_fd, LOG_PACK);
		ut_putendl_fd(server->logger_fd, "Waiting for connection...");
		client = accept(server->socket,
						(struct sockaddr *)&server->address,
						(socklen_t *)&(addr_len));
		if (client == -1)
		{
			ut_puterror("Server error", strerror(errno));
			continue ;
		}
		request = read_request(client);
		if (!request)
		{
			ut_putendl_fd(server->logger_fd, "Could not read/parse request");
			continue ;
		}
		if (request->path && strcmp(request->path, "/doexit") == 0)
		{
			request->dispose(request);
			close(client);
			break ;
		}
		ut_putstr_fd(server->logger_fd, "Client requested: ");
		ut_putendl_fd(server->logger_fd, request->path);
		dump_response(client, parse_response(*request), *server);
		close(client);
		request->dispose(request);
	}
}

static void	server_dispose(t_server *server)
{
	if (server)
	{
		if (server->socket != -1)
			close(server->socket);
		if (server->logger_fd != -1)
			close(server->logger_fd);
		free(server);
	}
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
	server->logger_fd = open("server.log", O_CREAT | O_WRONLY, 0644);
	if (server->logger_fd == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		free(server);
		return (NULL);
	}
	server->socket = socket(domain, service, protocol);
	if (server->socket == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	if (bind(server->socket, (struct sockaddr *)&server->address, sizeof(server->address)) == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	if (listen(server->socket, backlog) == -1)
	{
		ut_puterror("Init server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	server->dispose = server_dispose;
	server->up = server_up;
	return (server);
}
