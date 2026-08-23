#include "server.h"
#include <stdio.h>
#include <string.h>

int	main(void)
{
	t_server	server;

	bzero(&server, sizeof(t_server));
	server.ip = 0x7F000001;
	server.port = 8080;
	server.stop = &g_stop;
	switch (server_start(&server))
	{
		case (ERR_NONE):
			return (0);
		case (ERR_SOCKET):
			dprintf(2, ERROR_SOCKET_MSG, strerror(server.errno_code));
			return (1);
		case (ERR_BIND):
			dprintf(2, ERROR_BIND_MSG, strerror(server.errno_code));
			return (1);
		case (ERR_LISTEN):
			dprintf(2, ERROR_LISTEN_MSG, strerror(server.errno_code));
			return (1);
	}
}
