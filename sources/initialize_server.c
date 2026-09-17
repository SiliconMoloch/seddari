#include "server.h"
#include <unistd.h>

t_error			initialize_(t_server *server, int *fd, const uint16_t port);
static t_error	close_and_return_error(const int fd, const t_error error_code);

t_error			initialize_(t_server *server, int *fd, const uint16_t port)
{
	if (create_socket(server, fd))
		return (ERR_SOCKET);

	t_error	error_code;

	error_code = ERR_NONE;
	error_code = bind_address(server, port, *fd);
	if (error_code)
		return (close_and_return_error(*fd, error_code));
	error_code = set_passive_socket(server, fd);
	if (error_code)
		return (close_and_return_error(*fd, error_code));
	error_code = set_non_blocking_server(*fd, &server->errno_code);
	if (error_code)
		return(close_and_return_error(*fd, error_code));
	return (error_code);
}

static t_error	close_and_return_error(const int fd, const t_error error_code)
{
	close(fd);
	return (error_code);
}
