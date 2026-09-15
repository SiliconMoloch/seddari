#include "server.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

void	broadcast(t_server *server, const int except)
{
	t_client		*client;
	const size_t	message_length = strlen(server->log_buffer);
	t_client_error	error;

	client = server->head;
	while (client)
	{
		if (client->fd ^ except)
		{
			error = append_to_send_buffer(server, client->fd, server->log_buffer, message_length);
			if (error == CLIENT_ERR_NONE)
				FD_SET(client->fd, &server->active_write);
			else if (error == CLIENT_ERR_EXCEEDS_MAX_LENGTH)
				remove_client(server, client->fd);
		}
		client = client->next;
	}
}
