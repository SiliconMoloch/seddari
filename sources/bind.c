#include "server.h"
#include <errno.h>

t_error	bind_address(t_server *server)
{
	server->address.sin_family = AF_INET;
	server->address.sin_port = htons(server->port);
	server->address.sin_addr.s_addr = htonl(server->ip);
	if (!bind(server->socket, (const struct sockaddr *)&server->address, sizeof(server->address)))
		return (ERR_NONE);
	server->errno_code = errno;
	return (ERR_BIND);
}
