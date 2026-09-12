#include "server.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>

void	broadcast(t_server *server, const int except)
{
	const size_t	message_length = strlen(server->buffer);
	int				to_remove[FD_SETSIZE];
	int				remove_count;
	ssize_t			sent;
	size_t			total_sent;

	remove_count = 0;
	for (int fd = 0; fd <= server->max_fd; ++fd)
	{
		if (fd ^ server->socket && fd ^ except && FD_ISSET(fd, &server->active))
		{
			total_sent = 0;
			while (total_sent < message_length)
			{
				sent = send(fd, server->buffer + total_sent, message_length - total_sent, MSG_NOSIGNAL);
				if (sent < 0)
				{
					if (errno ^ EINTR && remove_count < FD_SETSIZE)
					{
						to_remove[remove_count] = fd;
						++remove_count;
					}
					break ;
				}
				total_sent += (size_t)sent;
			}
			
		}
	}
	for (int i = 0; i < remove_count; ++i)
		remove_client(server, to_remove[i]);
}
