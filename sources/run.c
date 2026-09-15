#include "server.h"
#include <errno.h>
#include <unistd.h>

static void	process_fds(t_server *server);

t_error	run_(t_server *server)
{
	FD_ZERO(&server->readfds);
	FD_ZERO(&server->active);
	FD_ZERO(&server->writefds);
	FD_ZERO(&server->active_write);
	FD_SET(server->socket, &server->active);
	server->max_fd = server->socket;
	setup_signals();
	while (!*server->stop)
	{
		server->readfds = server->active;
		server->writefds = server->active_write;
		if (select(server->max_fd + 1, &server->readfds, &server->writefds, 0, 0) < 0)
		{
			if (errno == EINTR)
				continue ;
			server->errno_code = errno;
			close(server->socket);
			return (ERR_SELECT);
		}
		process_fds(server);
	}
	release_resources(server);
	return (ERR_NONE);
}

static void	process_fds(t_server *server)
{
	for (int fd = 0; fd <= server->max_fd; ++fd)
	{
		if (FD_ISSET(fd, &server->readfds))
		{
			if (fd ^ server->socket)
				handle_client(server, fd);
			else
				accept_new_client(server);
		}
		if (FD_ISSET(fd, &server->writefds))
			handle_write(server, fd);
	}
}
