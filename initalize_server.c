#include "server.h"
#include <unistd.h>

t_error	initalize_(t_server *server)
{
	if (create_socket(server))
		return (ERR_SOCKET);

	t_error	error_code;

	error_code = ERR_NONE;
	error_code = bind_address(server);
	if (error_code)
	{
		close(server->socket);
		return (error_code);
	}
	error_code = set_passive_socket(server);
	if (error_code)
		close(server->socket);
	return (error_code);
}
