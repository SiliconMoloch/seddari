#include "server.h"
#include <stdio.h>
#include <string.h>

void	handle_message(t_server *server, const int32_t fd, const char *msg)
{
	t_client	*client;

	client = find_client(server, fd);
	if (!client)
		return ;
	sprintf(server->buffer, "[%s]: %s", client->nickname, msg);
	broadcast(server, -1);
	bzero(server->buffer, strlen(server->buffer));
}
