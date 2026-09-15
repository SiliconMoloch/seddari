#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

typedef uint8_t t_error;
enum
{
    ERR_NONE = 0,
    ERR_SOCKET,
    ERR_BIND,
    ERR_LISTEN,
    ERR_SELECT
};

typedef uint8_t t_client_error;
enum
{
    CLIENT_ERR_NONE = 0,
    CLIENT_ERR_NOT_FOUND,
    CLIENT_ERR_NON_PRINTABLE_CHARACTERS,
    CLIENT_ERR_MALLOC
};

typedef uint8_t t_warning;
enum
{
    WARN_NONE = 0,
    WARN_UNKNOWN_COMMAND,
    WARN_NICKNAME_TOO_LONG
};

typedef uint8_t t_command_status;
enum
{
    CMD_STATUS_NONE = 0,
    CMD_STATUS_NICKNAME_CHANGED,
    CMD_STATUS_CLIENT_LIST_SENT,
    CMD_STATUS_CLIENT_QUIT
};

#define ERROR_SOCKET_MSG "seddari: Fatal error\nsocket(): %s\n"
#define ERROR_BIND_MSG "seddari: Fatal error\nbind(): %s\n"
#define ERROR_LISTEN_MSG "seddari: Fatal error\nlisten(): %s\n"
#define ERROR_SELECT_MSG "seddari: Fatal error\nselect(): %s\n"

#endif
