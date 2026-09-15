#include "server.h"
#include <stdlib.h>
#include <string.h>

t_client_error append_to_recv_buffer(t_client *client, const char *buffer, const ssize_t bytes_received)
{
	if (client->recv_size + bytes_received >= sizeof(client->recv_buffer))
		return (CLIENT_ERR_BUFFER_OVERFLOW);
	memcpy(client->recv_buffer + client->recv_size, buffer, bytes_received);
	client->recv_size += bytes_received;
	client->recv_buffer[client->recv_size] = '\0';
	return (CLIENT_ERR_NONE);
}

void	process_recv_buffer(t_server *server, t_client *client)
{
	char		*newline;
	char		*message;
	size_t		message_length;
	const int	fd = client->fd;

	while (1)
	{
		newline = strchr(client->recv_buffer, '\n');
		if (!newline)
			return ;
		message_length = newline - client->recv_buffer + 1;
		message = calloc(message_length + 1, sizeof(char));
		if (!message)
			return ;
		memcpy(message, client->recv_buffer, message_length);
		message[message_length] = '\0';
		handle_message(server, client->fd, message);
		free(message);
		client = find_client(server, fd);
		if (!client)
			return ;
		memmove(client->recv_buffer,
			client->recv_buffer + message_length,
			client->recv_size - message_length);
		client->recv_size -= message_length;
		client->recv_buffer[client->recv_size] = '\0';
	}
}