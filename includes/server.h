#ifndef SERVER_H
#define SERVER_H

#include "status.h"
#include "client.h"
#include "signals.h"
#include <stdbool.h>

typedef struct s_server
{
	volatile sig_atomic_t				*stop;
	t_client							*head;
	int									max_fd;		
	uint64_t								next_id;
	fd_set								active;
	fd_set								readfds;
	int									socket;
	uint16_t							errno_code;
	struct sockaddr_in					address;
	uint32_t							ip;
	uint16_t							port;
	char								timestamp[22];
	char								log_buffer[256];
}	t_server;

t_error			server_start(t_server *server);
t_error			initialize_(t_server *server);
t_error			run_(t_server *server);
t_error			create_socket(t_server *server);
t_error			bind_address(t_server *server);
t_error			set_passive_socket(t_server *server);
t_error			set_non_blocking_server(const int fd, uint16_t *errno_code);
t_error			handle_commands(t_server *server, t_client *client);
void			handle_arguments(t_server *server, int argc, const char *argv[]);
void			update_timestamp(t_server *server);
void			accept_new_client(t_server *server);
void			handle_client(t_server *server, const int fd);
void			remove_client(t_server *server, const int fd);
t_client_error	handle_message(t_server *server, const int fd, const char *r);
void			broadcast(t_server *server, const int except);
void			add_to_list(t_server *server, t_client *client);
void			remove_from_list(t_server *server, t_client *client);
void			release_resources(t_server *server);
t_client		*find_client(t_server *server, const int fd);

#endif
