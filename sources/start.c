#include "server.h"
#include <unistd.h>

t_error	server_start(t_server *server)
{
	t_error	error_code;

	error_code = initialize_(server, &server->socket, server->port);
	if (error_code)
		return (error_code);
	if (server->metrics_mode)
	{
		error_code = initialize_(server, &server->metrics_socket, server->metrics_port);
		if (error_code)
		{
			close (server->socket);
			return (error_code);
		}
	}
	error_code = run_(server);
	return (error_code);
}
