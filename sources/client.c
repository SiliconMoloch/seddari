#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void			accept_new_client(t_server *server);
void			handle_client(t_server *server, const int fd);
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
	else if (new_client->fd >= FD_SETSIZE || set_non_blocking_client(new_client->fd) ^ CLIENT_ERR_NONE)
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
	snprintf(server->log_buffer, sizeof(server->log_buffer), "%s Welcome client %lu!\n",server->timestamp, new_client->id);
	broadcast(server, -1);
}

void	handle_client(t_server *server, const int fd)
{
	t_client		*client;
	char			buffer[BUFFER_SIZE];
	const ssize_t	bytes_received = recv(fd, buffer, BUFFER_SIZE - 1, 0);
	
	switch (bytes_received)
	{
		case (-1):
			if (errno ^ EINTR && errno ^ EAGAIN && errno ^ EWOULDBLOCK)
				remove_client(server, fd);
			return ;
		case (0):
			remove_client(server, fd);
			return ;
		default:
			client = find_client(server, fd);
			if (!client)
				return ;
			if (append_to_recv_buffer(client, buffer, bytes_received))
			{
				remove_client(server, fd);
				return ;
			}
			process_recv_buffer(server, client);
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
	FD_CLR(fd, &server->readfds);
	FD_CLR(fd, &server->active);
	FD_CLR(fd, &server->writefds);
	FD_CLR(fd, &server->active_write);
	close(fd);
	if (fd == server->max_fd)
	{
		while (server->max_fd > 0 && !FD_ISSET(server->max_fd, &server->active))
			--server->max_fd;
	}
	remove_from_list(server, client);
	update_timestamp(server);
	snprintf(server->log_buffer, sizeof(server->log_buffer), "%s Goodbye %s!\n", server->timestamp, nickname[0] ? nickname : "(unnamed)");
	broadcast(server, -1);
}

void    handle_write(t_server *server, const int fd)
{
    t_client	*client;
    ssize_t		bytes_sent;

    client = find_client(server, fd);
    if (!client)
        return ;
    else if (!client->send_buffer || !client->send_buffer_size)
    {
        FD_CLR(fd, &server->writefds);
        FD_CLR(fd, &server->active_write);
        return ;
    }
    bytes_sent = send(fd, client->send_buffer, client->send_buffer_size, 0);
    switch (bytes_sent)
    {
        case (-1):
            if (errno ^ EINTR && errno ^ EAGAIN && errno ^ EWOULDBLOCK)
                remove_client(server, fd);
            return ;
        case (0):
            remove_client(server, fd);
            return ;
        default:
            memmove(client->send_buffer, client->send_buffer + bytes_sent, client->send_buffer_size - bytes_sent);
            client->send_buffer_size -= bytes_sent;
            if (!client->send_buffer_size)
            {
                FD_CLR(fd, &server->writefds);
                FD_CLR(fd, &server->active_write);
                free(client->send_buffer);
                client->send_buffer = NULL;
                client->send_buffer_size = 0;
            }
    }
}
