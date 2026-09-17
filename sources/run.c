#include "server.h"
#include "utils.h"
#include <errno.h>
#include <unistd.h>

static void	process_fds(t_server *server);

t_error	run_(t_server *server)
{
	FD_SET(server->socket, &server->active);
	if (server->metrics_mode)
		FD_SET(server->metrics_socket, &server->active);
	server->max_fd = (server->socket > server->metrics_socket) ? server->socket : server->metrics_socket;
	setup_signals();
	set_server_start_time(server);
	server->metrics.stop = (bool *)server->stop;
	while (!*server->stop)
	{
		if (metrics_updated(server->last_metrics, server->metrics))
		{
			server->last_metrics = server->metrics;
			if (server->print_metrics)
				print_metrics(server->metrics);
		}
		server->readfds = server->active;
		server->writefds = server->active_write;
		if (select(server->max_fd + 1, &server->readfds, &server->writefds, 0, 0) < 0)
		{
			if (errno == EINTR)
				continue ;
			server->errno_code = errno;
			close(server->socket);
			if (server->metrics_mode)
				close(server->metrics_socket);
			return (ERR_SELECT);
		}
		process_fds(server);
	}
	release_resources(server);
	return (ERR_NONE);
}

static void	process_fds(t_server *server)
{
	bool	is_socket;
	bool	is_metrics_socket;

	for (int fd = 0; fd <= server->max_fd; ++fd)
	{
		if (FD_ISSET(fd, &server->readfds))
		{
			is_socket = (fd == server->socket);
			is_metrics_socket = (server->metrics_mode && (fd == server->metrics_socket));
			if (!is_socket && !is_metrics_socket)
				handle_client(server, fd);
			else if (!is_metrics_socket)
				accept_new_client(server);
			else
				;
		}
		if (FD_ISSET(fd, &server->writefds))
			handle_write(server, fd);
	}
}
