#ifndef SERVER_H
#define SERVER_H

#include "status.h"
#include "client.h"
#include "signals.h"
#include "metrics.h"
#include <stdbool.h>

#define SEND_BUFFER_LIMIT_SIZE 65536
#define MAX_MSG_PER_SEC 10

typedef struct s_server
{
	volatile sig_atomic_t				*stop;
	t_client							*head;
	int									max_fd;		
	uint64_t							next_id;
	fd_set								active;
	fd_set								readfds;
	fd_set								active_write;
	fd_set								writefds;
	int									socket;
	uint16_t							errno_code;
	struct sockaddr_in					address;
	uint32_t							ip;
	uint16_t							port;
	char								timestamp[22];
	char								log_buffer[256];
	t_metrics							last_metrics;
	t_metrics							metrics;
	bool								print_metrics;
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
void			set_server_start_time(t_server *server);
void			update_timestamp(t_server *server);
void			accept_new_client(t_server *server);
void			handle_client(t_server *server, const int fd);
void			remove_client(t_server *server, const int fd);
t_client_error	handle_message(t_server *server, const int fd, const char *r);
t_client_error	append_to_send_buffer(t_server *server, const int fd, const char *message, const size_t message_length);
t_client_error	append_to_recv_buffer(t_client *client, const char *buffer, const ssize_t bytes_received);
void			process_recv_buffer(t_server *server, t_client *client);
void			broadcast(t_server *server, const int except);
void			handle_write(t_server *server, const int fd);
void			add_to_list(t_server *server, t_client *client);
void			remove_from_list(t_server *server, t_client *client);
void			release_resources(t_server *server);
t_client		*find_client(t_server *server, const int fd);


#endif
