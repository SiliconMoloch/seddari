#include <unistd.h>
#include <stdlib.h>

#include "server.h"

void	release_ressources(t_server *server)
{
	t_client	*to_free;

	close(server->socket);
	while (server->head)
	{
		to_free = server->head;
		server->head = server->head->next;
		close(to_free->fd);
		free(to_free);
	}
}
