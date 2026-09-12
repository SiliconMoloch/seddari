#include "server.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

void	broadcast(t_server *server, const int except)
{
	const size_t	message_length = strlen(server->buffer);
	ssize_t			bytes_sent;

	for (int fd = 0; fd <= server->max_fd; ++fd)
	{
		if (fd ^ server->socket && fd ^ except && FD_ISSET(fd, &server->active))
		{
			bytes_sent = send(fd, server->buffer, message_length, MSG_NOSIGNAL);
			if (bytes_sent < 0)
			{
				if (errno ^ EINTR)
					remove_client(server, fd);
				continue ;
			}
		}
	}
}
