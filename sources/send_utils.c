#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

t_client_error	append_to_send_buffer(t_server *server, const int fd, const char *message, const size_t message_length)
{
	t_client	*client;
	char		*new_buffer;
    size_t       new_size;

	client = find_client(server, fd);
	if (!client)
		return (CLIENT_ERR_NOT_FOUND);
    new_size = client->send_buffer_size + message_length;
    if (new_size > SEND_BUFFER_LIMIT_SIZE)
        return (CLIENT_ERR_EXCEEDS_MAX_LENGTH);
    new_buffer = realloc(client->send_buffer, new_size  + 1);
	if (!new_buffer)
		return (CLIENT_ERR_MALLOC);
	client->send_buffer = new_buffer;
	memcpy(client->send_buffer + client->send_buffer_size, message, message_length);
	client->send_buffer[new_size] = '\0';
	client->send_buffer_size += message_length;
	return (CLIENT_ERR_NONE);
}
