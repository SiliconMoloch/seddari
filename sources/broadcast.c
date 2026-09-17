#include "server.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

void	broadcast(t_server *server, const int except)
{
	++server->metrics.complete_messages_received;

	t_client		*client;
	const size_t	message_length = strlen(server->log_buffer);
	t_client_error	error;
	int				to_remove[FD_SETSIZE];
	int				remove_count;

	client = server->head;
	remove_count = 0;
	while (client)
	{
		if (client->fd ^ except)
		{
			error = append_to_send_buffer(server, client->fd, server->log_buffer, message_length);
			if (error == CLIENT_ERR_NONE)
				FD_SET(client->fd, &server->active_write);
			else if (error == CLIENT_ERR_EXCEEDS_MAX_LENGTH)
				to_remove[remove_count++] = client->fd;
		}
		client = client->next;
	}
	for (int i = 0; i < remove_count; ++i)
		remove_client(server, to_remove[i]);
}
