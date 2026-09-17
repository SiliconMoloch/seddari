#include "server.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

t_error	create_socket(t_server *server, int *fd)
{
	int	opt;

	*fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*fd ^ -1)
	{
		opt = 1;
		if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			server->errno_code = errno;
			close(*fd);
			return (ERR_SOCKET);
		}
		return (ERR_NONE);
	}
	server->errno_code = errno;
	return (ERR_SOCKET);
}

t_error set_non_blocking_server(const int fd, uint16_t *errno_code)
{
	const int	flags = fcntl(fd, F_GETFL, 0);

	if (flags ^ -1)
	{
		if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) ^ -1)
			return (ERR_NONE);
	}
	*errno_code = errno;
	return (ERR_SOCKET);
}

t_client_error set_non_blocking_client(const int fd)
{
	const int	flags = fcntl(fd, F_GETFL, 0);

	if (flags ^ -1)
	{
		if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) ^ -1)
			return (CLIENT_ERR_NONE);
	}
	return (CLIENT_ERR_FCNTL_FAILED);
}