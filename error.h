#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef uint8_t	t_error;
enum
{
	ERR_NONE = 0,
	ERR_SOCKET = 1,
	ERR_BIND = 2,
	ERR_LISTEN = 3,
	ERR_SELECT = 4
};

#define ERROR_SOCKET_MSG "seddari: Fatal error\nsocket(): %s\n"
#define ERROR_BIND_MSG "seddari: Fatal error\nbind(): %s\n"
#define ERROR_LISTEN_MSG "seddari: Fatal error\nlisten(): %s\n"
#define ERROR_SELECT_MSG "seddari: Fatal error\nselect(): %s\n"

#endif
