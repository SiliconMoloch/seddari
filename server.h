#ifndef SERVER_H
#define SERVER_H

#include "error.h"
#include "client.h"
#include "signals.h"

typedef struct s_server
{
	volatile sig_atomic_t				*stop;
	t_client							*head;
	int32_t								max_fd;		
	uint8_t								next_id;
	fd_set								active;
	fd_set								readfds;
	int32_t								socket;
	uint16_t							errno_code;
	struct sockaddr_in					address;
	uint32_t							ip;
	uint16_t							port;
	char								buffer[128];
}	t_server;

t_error		server_start(t_server *server);
t_error		initialize_(t_server *server);
t_error		run_(t_server *server);
t_error		create_socket(t_server *server);
t_error		bind_address(t_server *server);
t_error		set_passive_socket(t_server *server);
void		accept_new_client(t_server *server);
void		handle_client(t_server *server, const int32_t fd);
void		remove_client(t_server *server, const int32_t fd);
void		handle_message(t_server *server, const int32_t fd, const char *r);
void		broadcast(t_server *server, const int16_t except);
void		add_to_list(t_server *server, t_client *client);
void		remove_from_list(t_server *server, t_client *client);
void		release_ressources(t_server *server);
t_client	*find_client(t_server *server, const int32_t fd);

#endif
