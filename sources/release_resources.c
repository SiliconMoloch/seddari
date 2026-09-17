#include <unistd.h>
#include <stdlib.h>

#include "server.h"

void	release_resources(t_server *server)
{
	t_client	*to_free;

	close(server->socket);
	if (server->metrics_mode)
		close(server->metrics_socket);
	while (server->head)
	{
		to_free = server->head;
		server->head = server->head->next;
		close(to_free->fd);
		free(to_free->message);
		free(to_free->send_buffer);
		free(to_free);
	}
}
