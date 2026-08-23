#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/ip.h>

#define BUFFER_SIZE (1 << 16)

typedef struct s_client
{
	struct sockaddr	address;
	socklen_t		address_length;
	int32_t			fd;
	uint8_t			id;
	char			message[BUFFER_SIZE];
	char			nickname[32];
	struct s_client	*next;
}	t_client;

#endif
