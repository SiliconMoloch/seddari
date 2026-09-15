#include <time.h>
#include "server.h"

void	update_timestamp(t_server *server)
{
	const time_t	now = time(NULL);
	struct tm		*time_info;

	time_info = localtime(&now);
	strftime(server->timestamp, sizeof(server->timestamp), "[%d/%m/%Y %H:%M:%S]", time_info);
}

t_client_error	rate_limit_exceeded(t_client *client)
{
	const time_t	now = time(NULL);

	if (difftime(now, client->last_activity) < 0.1)
		return (CLIENT_ERR_EXCEEDS_RATE_LIMIT);
	client->last_activity = now;
	return (CLIENT_ERR_NONE);
}
