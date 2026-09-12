/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabokhar <yabokhar@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 21:19:44 by yabokhar          #+#    #+#             */
/*   Updated: 2026/08/15 00:42:01 by yabokhar         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <errno.h>

static void	process_fds(t_server *server);

t_error	run_(t_server *server)
{
	FD_ZERO(&server->readfds);
	FD_ZERO(&server->active);
	FD_SET(server->socket, &server->active);
	server->max_fd = server->socket;
	setup_signals();
	while (!*server->stop)
	{
		server->readfds = server->active;
		if (select(server->max_fd + 1, &server->readfds, 0, 0, 0) < 0)
		{
			if (errno == EINTR)
				continue ;
			server->errno_code = errno;
			return (ERR_SELECT);
		}
		process_fds(server);
	}
	release_ressources(server);
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
	}
}
