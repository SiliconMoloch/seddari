#include "server.h"
#include <unistd.h>
#include <errno.h>

t_error	create_socket(t_server *server)
{
	int	opt;

	server->socket = socket(AF_INET, SOCK_STREAM, 0);
	opt = 1;
	if (server->socket ^ -1)
	{
		if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			server->errno_code = errno;
			close(server->socket);
			return (ERR_SOCKET);
		}
		return (ERR_NONE);
	}
	server->errno_code = errno;
	return (ERR_SOCKET);
}
