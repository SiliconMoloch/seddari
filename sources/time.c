#include <time.h>
#include "server.h"

void	update_timestamp(t_server *server)
{
	time_t		now;
	struct tm	*time_info;

	now = time(NULL);
	time_info = localtime(&now);
	strftime(server->timestamp, sizeof(server->timestamp), "[%d/%m/%Y %H:%M:%S]", time_info);
}
