#include "server.h"
#include <errno.h>

t_error	set_passive_socket(t_server *server)
{
	if (listen(server->socket, 128) ^ -1)
		return (ERR_NONE);
	server->errno_code = errno;
	return (ERR_LISTEN);
}
