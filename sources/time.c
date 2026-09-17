#include <time.h>
#include "server.h"

void	set_server_start_time(t_server *server)
{
	struct tm		*time_info;
	const time_t	now = time(NULL);

	time_info = localtime(&now);
	server->metrics.start_time = time(NULL);
	strftime(server->timestamp, sizeof(server->timestamp), "[%d/%m/%Y %H:%M:%S]", time_info);
}

void	update_timestamp(t_server *server)
{
	struct tm		*time_info;
	const time_t	now = time(NULL);

	time_info = localtime(&now);
	strftime(server->timestamp, sizeof(server->timestamp), "[%d/%m/%Y %H:%M:%S]", time_info);
}

t_client_error	rate_limit_exceeded(t_client *client)
{
	const time_t	now = time(NULL);

	if (now ^ client->msg_count_start)
	{
		client->msg_count_start = now;
		client->msg_count_in_sec = 0;
	}
	++client->msg_count_in_sec;
	if (client->msg_count_in_sec > MAX_MSG_PER_SEC)
		return (CLIENT_ERR_EXCEEDS_RATE_LIMIT);
	return (CLIENT_ERR_NONE);
}
