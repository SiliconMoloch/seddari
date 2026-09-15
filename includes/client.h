#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/ip.h>

typedef struct s_client
{
	struct sockaddr	address;
	socklen_t		address_length;
	int				fd;
	uint64_t		id;
	char			*message;
	char			recv_buffer[256];
	size_t			recv_size;
	char			nickname[32];
	struct s_client	*next;
}	t_client;

t_client_error	set_non_blocking_client(const int fd);

#endif
