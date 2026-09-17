#include "server.h"
#include <errno.h>

t_error	set_passive_socket(t_server *server, int *fd)
{
	if (listen(*fd, 128) ^ -1)
		return (ERR_NONE);
	server->errno_code = errno;
	return (ERR_LISTEN);
}
