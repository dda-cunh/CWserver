#include "../../inc/cwserver.h"

static void	*handle_client(void *arg)
{
	t_request	*request;
	t_server	*server;
	bool		running;
	int			addr_len;
	int			client;

	server = (t_server *)arg;
	addr_len = sizeof(server->address);
	running = true;
	while (running)
	{
		request = NULL;
		client = accept(server->socket,
						(struct sockaddr *)&server->address,
						(socklen_t *)&(addr_len));
		if (client != -1)
		{
			request = read_request(client, server);
			if (request)
			{
				handle_response(*request, client);
				close(client);
				request->dispose(request);
			}
		}
		pthread_mutex_lock(server->running_mutex);
		running = server->running;
		pthread_mutex_unlock(server->running_mutex);
	}
	return (NULL);
}

static void	*server_manager(void *arg)
{
	t_server	*server;
	bool		running;
	char		*input;

	server = (t_server *)arg;
	ut_putendl_fd(STD_OUT, ANSI_COLOR_GREEN);
	ut_putendl_fd(STD_OUT, "Server is up");
	ut_putendl_fd(STD_OUT, ANSI_COLOR_RESET);
	running = true;
	while (running)
	{
		input = get_next_line(STD_IN);
		if (input)
		{
			if (!strcmp(input, "exit\n"))
			{
				ut_putendl_fd(STD_OUT, ANSI_COLOR_RED);
				ut_putendl_fd(STD_OUT, "Server is shutting down...");
				ut_putendl_fd(STD_OUT, ANSI_COLOR_RESET);
				pthread_mutex_lock(server->running_mutex);
				server->running = false;
				pthread_mutex_unlock(server->running_mutex);
				running = false;
			}
			free(input);
		}
	}
	return (NULL);
}

static void	server_up(t_server *server)
{
	pthread_t	threads[server->backlog];
	pthread_t	manager;

	if (pthread_create(&manager, NULL, &server_manager, server))
	{
		ut_puterror("Server error", strerror(errno));
		return ;
	}
	for (int i = 0; i < server->backlog; i++)
	{
		if (pthread_create(&threads[i], NULL, &handle_client, server))
		{
			ut_puterror("Server error", strerror(errno));
			return ;
		}
	}
	if (pthread_join(manager, NULL))
	{
		ut_puterror("Server error", strerror(errno));
		return ;
	}
	for (int i = 0; i < server->backlog; i++)
	{
		if (pthread_join(threads[i], NULL))
		{
			ut_puterror("Server error", strerror(errno));
			return ;
		}
	}
	return ;
}

static void	server_dispose(t_server *server)
{
	if (server)
	{
		if (server->socket != -1)
			close(server->socket);
		if (server->logger_fd != -1)
			close(server->logger_fd);
		if (server->running_mutex)
		{
			pthread_mutex_destroy(server->running_mutex);
			free(server->running_mutex);
		}
		if (server->output_mutex)
		{
			pthread_mutex_destroy(server->output_mutex);
			free(server->output_mutex);
		}
		free(server);
	}
}

t_server	*t_server_new(unsigned long interface, int domain, int protocol,
							int service, int port, int backlog)
{
	struct timeval	tv;
	t_server		*server;

	server = (t_server *)malloc(sizeof(t_server));
	if (server == NULL)
		return (NULL);
	server->running_mutex = NULL;
	server->output_mutex = NULL;
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
	server->logger_fd = -1;
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
	tv.tv_sec = SOCKET_TIMEOUT;
	tv.tv_usec = 0;
	if (setsockopt(server->socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) < 0)
	{
		ut_puterror("Server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	server->running_mutex = malloc(sizeof(pthread_mutex_t));
	if (server->running_mutex == NULL)
	{
		ut_puterror("Init server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	pthread_mutex_init(server->running_mutex, NULL);
	server->output_mutex = malloc(sizeof(pthread_mutex_t));
	if (server->output_mutex == NULL)
	{
		ut_puterror("Init server error", strerror(errno));
		server_dispose(server);
		return (NULL);
	}
	server->logger_fd = open("server.log", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	pthread_mutex_init(server->output_mutex, NULL);
	server->dispose = server_dispose;
	server->up = server_up;
	server->running = true;
	return (server);
}
