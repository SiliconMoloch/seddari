#include "server.h"
#include <errno.h>
#include <unistd.h>

t_error	server_start(t_server *server)
{
	t_error	error_code;

	error_code = initalize_(server);
	if (error_code)
		return (error_code);
	error_code = run_(server);
	close(server->socket);
	return (error_code);
}
