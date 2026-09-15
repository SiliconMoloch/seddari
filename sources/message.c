#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static uint8_t non_printable_characters(const char *r);

t_client_error	handle_message(t_server *server, const int fd, const char *r)
{
	t_client			*client;
	t_command_status	command_status;
	char				*new_message;

	if (non_printable_characters(r))
		return (CLIENT_ERR_NON_PRINTABLE_CHARACTERS);
	client = find_client(server, fd);
	if (!client)
		return (CLIENT_ERR_NOT_FOUND);
	new_message = calloc(strlen(r) + 1, sizeof(char));
	if (!new_message)
		return (CLIENT_ERR_MALLOC);
	free(client->message);
	client->message = new_message;
	strcpy(client->message, r);
	command_status = handle_commands(server, client);
	if (command_status ^ CMD_STATUS_NONE)
		return (CLIENT_ERR_NONE);
	update_timestamp(server);
	snprintf(server->buffer, sizeof(server->buffer), "%s [%s]: %s", server->timestamp, client->nickname, client->message);
	broadcast(server, -1);
	return (CLIENT_ERR_NONE);
}

static uint8_t	non_printable_characters(const char *r)
{
	for (uint32_t i = 0; r[i]; ++i)
		if (!isprint((unsigned char)r[i]) && r[i] ^ '\n')
			return (1);
	return (0);
}
