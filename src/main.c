#include "../inc/cwserver.h"

int main(void)
{
	t_server	*server;

	server = t_server_new(INADDR_ANY, AF_INET, IPPROTO_TCP, SOCK_STREAM, 8080, 10);
	if (server == NULL)
		return (EXIT_FAILURE);
	server->up(server);
	server->dispose(server);
	return (EXIT_SUCCESS);
}
