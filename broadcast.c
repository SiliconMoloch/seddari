#include "server.h"
#include <string.h>
#include <unistd.h>

void	broadcast(t_server *server, const int16_t except)
{
	const uint64_t	message_length = strlen(server->buffer);

	for (uint8_t fd = 0; fd <= server->max_fd; ++fd)
	{
		if (fd ^ server->socket && fd ^ except && FD_ISSET(fd, &server->active))
			send(fd, server->buffer, message_length, MSG_NOSIGNAL);
	}
}

