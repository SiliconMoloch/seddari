#include "server.h"
#include <errno.h>

t_error	bind_address(t_server *server, const uint16_t port, const int fd)
{
	server->address.sin_family = AF_INET;
	server->address.sin_port = htons(port);
	server->address.sin_addr.s_addr = server->ip;
	if (!bind(fd, (const struct sockaddr *)&server->address, sizeof(server->address)))
		return (ERR_NONE);
	server->errno_code = errno;
	return (ERR_BIND);
}
