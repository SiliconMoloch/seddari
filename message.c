#include "server.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static uint8_t non_printable_characters(const char *r);

void	handle_message(t_server *server, const int32_t fd, const char *r)
{
	t_client	*client;

	if (non_printable_characters(r))
		return ;
	client = find_client(server, fd);
	if (!client)
		return ;
	strcpy(client->message, r);
	sprintf(server->buffer, "[%s]: %s", client->nickname, client->message);
	broadcast(server, -1);
}

static uint8_t	non_printable_characters(const char *r)
{
	for (uint64_t i = 0; r[i]; ++i)
		if (!isprint((unsigned char)r[i]) && r[i] ^ '\n')
			return (1);
	return (0);
}
