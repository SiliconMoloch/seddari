#include "server.h"
#include <stdio.h>
#include <string.h>

int	main(int argc, const char *argv[])
{
	t_server	server;

	memset(&server, 0, sizeof(t_server));
	server.ip = htonl(0x7F000001);
	server.port = 8080;
	handle_arguments(&server, argc, argv);
	server.stop = &g_stop;
	switch (server_start(&server))
	{
		case (ERR_NONE):
			printf("Goodbye!\n");
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
		case (ERR_SELECT):
			dprintf(2, ERROR_SELECT_MSG, strerror(server.errno_code));
			return (1);
	}
}
