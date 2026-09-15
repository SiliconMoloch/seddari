#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/ip.h>
#define SEND_BUFFER_LIMIT_SIZE 65536
#define MAX_NICKNAME_SIZE 32

typedef struct s_client
{
	struct sockaddr	address;
	socklen_t		address_length;
	int				fd;
	uint64_t		id;
	char			*message;
	char			*send_buffer;
	size_t			send_buffer_size;	
	char			recv_buffer[256];
	size_t			recv_size;
	char			nickname[MAX_NICKNAME_SIZE];
	time_t			last_activity;
	struct s_client	*next;
}	t_client;

t_client_error	set_non_blocking_client(const int fd);
t_client_error	rate_limit_exceeded(t_client *client);

#endif
