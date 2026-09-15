#include "server.h"
#include <stdlib.h>
#include <string.h>

void	add_to_list(t_server *server, t_client *client)
{
	if (!server->head)
	{
		server->head = client;
		return ;
	}

	t_client	*current;

	current = server->head;
	while (current->next)
		current = current->next;
	current->next = client;
}

void	remove_from_list(t_server *server, t_client *client)
{
	t_client	**current;

	current = &server->head;
	while (*current && *current != client)
		current = &(*current)->next;
	if (!*current)
		return ;
	*current = client->next;
	free(client->message);
	free(client->send_buffer);
	free(client);
}

t_client	*find_client(t_server *server, const int fd_to_look_for)
{
	t_client	*index;

	index = server->head;
	while (index && index->fd ^ fd_to_look_for)
		index = index->next;
	return (index);
}