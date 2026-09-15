#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void			accept_new_client(t_server *server);
void			handle_client(t_server *server, const int fd);
static uint8_t	append_to_recv_buffer(t_client *client, const char *buffer, const ssize_t bytes_received);
static void		process_recv_buffer(t_server *server, t_client *client);
void			remove_client(t_server *server, const int fd);

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
	else if (new_client->fd >= FD_SETSIZE)
	{
		close(new_client->fd);
		free(new_client);
		return ;
	}
	if (new_client->fd > server->max_fd)
		server->max_fd = new_client->fd;
	FD_SET(new_client->fd, &server->active);
	new_client->id = server->next_id++;
	new_client->next = NULL;
	add_to_list(server, new_client);
	snprintf(new_client->nickname, sizeof(new_client->nickname), "Client %lu", new_client->id);
	update_timestamp(server);
	snprintf(server->buffer, sizeof(server->buffer), "%s Welcome client %lu!\n",server->timestamp, new_client->id);
	broadcast(server, -1);
}

void	handle_client(t_server *server, const int fd)
{
	t_client		*client;
	char			buffer[BUFFER_SIZE];
	const ssize_t	bytes_received = recv(fd, buffer, BUFFER_SIZE - 1, 0);
	
	if (bytes_received > 0)
	{
		client = find_client(server, fd);
		if (!client)
			return ;
		if (!append_to_recv_buffer(client, buffer, bytes_received))
		{
			remove_client(server, fd);
			return ;
		}
		process_recv_buffer(server, client);
	}
	else if (!bytes_received)
		remove_client(server, fd);
	else if (errno ^ EINTR)
			remove_client(server, fd);
}

static uint8_t append_to_recv_buffer(t_client *client, const char *buffer, const ssize_t bytes_received)
{
	if (client->recv_size + bytes_received >= sizeof(client->recv_buffer))
		return (0);
	memcpy(client->recv_buffer + client->recv_size, buffer, bytes_received);
	client->recv_size += bytes_received;
	client->recv_buffer[client->recv_size] = '\0';
	return (1);
}

static void	process_recv_buffer(t_server *server, t_client *client)
{
	char	*newline;
	char	*message;
	size_t	message_length;

	while (1)
	{
		newline = strchr(client->recv_buffer, '\n');
		if (!newline)
			return ;
		message_length = newline - client->recv_buffer + 1;
		message = calloc(message_length + 1, sizeof(char));
		if (!message)
			return ;
		memcpy(message, client->recv_buffer, message_length);
		message[message_length] = '\0';
		handle_message(server, client->fd, message);
		free(message);
		memmove(client->recv_buffer,
			client->recv_buffer + message_length,
			client->recv_size - message_length);
		client->recv_size -= message_length;
		client->recv_buffer[client->recv_size] = '\0';
	}
}


void	remove_client(t_server *server, const int fd)
{
	t_client	*client;
	uint64_t	client_id;
	char		nickname[32];

	client = find_client(server, fd);
	if (!client)
		return ;
	client_id = client->id;
	memcpy(nickname, client->nickname, sizeof(nickname));
	FD_CLR(fd, &server->active);
	close(fd);
	if (fd == server->max_fd)
	{
		while (server->max_fd > 0 && !FD_ISSET(server->max_fd, &server->active))
			--server->max_fd;
	}
	remove_from_list(server, client);
	update_timestamp(server);
	snprintf(server->buffer, sizeof(server->buffer), "%s Goodbye client %s! (id %lu)\n", server->timestamp, nickname[0] ? nickname : "(unnamed)", client_id);
	broadcast(server, -1);
}
