#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void	accept_new_client(t_server *server)
{
	t_client		*new_client;
	struct sockaddr	*client_address;
	socklen_t		*address_length;
	
	new_client = calloc(1, sizeof(t_client));
	if (!new_client)
		return ;
	client_address = &new_client->address;
	address_length = &new_client->address_length;
	new_client->address_length = sizeof(*client_address);
	new_client->fd = accept(server->socket, client_address, address_length);
	if (new_client->fd < 0)
	{
		free(new_client);
		return ;
	}
	if (new_client->fd > server->max_fd)
		server->max_fd = new_client->fd;
	FD_SET(new_client->fd, &server->active);
	new_client->id = server->next_id++;
	new_client->next = NULL;
	add_to_list(server, new_client);
	snprintf(server->buffer, sizeof(server->buffer), "Welcome client %lu!\n", new_client->id);
	broadcast(server, -1);
}

void	handle_client(t_server *server, const int fd)
{
	char			recv_buffer[BUFFER_SIZE];
	const ssize_t	bytes_received = recv(fd, recv_buffer, BUFFER_SIZE - 1, 0);
	
	if (bytes_received > 0)
	{
		recv_buffer[bytes_received] = '\0';
		handle_message(server, fd, recv_buffer);
	}
	else if (!bytes_received)
		remove_client(server, fd);
	else
	{
		if (errno ^ EINTR)
			remove_client(server, fd);
		return ;
	}
}

void	remove_client(t_server *server, const int fd)
{
	t_client	*client;

	client = find_client(server, fd);
	if (!client)
		return ;
	snprintf(server->buffer, sizeof(server->buffer), "Goodbye client %s! (id %lu)\n", client->nickname, client->id);
	broadcast(server, -1);
	FD_CLR(fd, &server->active);
	close(fd);
	if (fd == server->max_fd)
	{
		while (server->max_fd > 0 && !FD_ISSET(server->max_fd, &server->active))
			--server->max_fd;
	}
	remove_from_list(server, client);
}