#include "server.h"
#include <stdio.h>
#include <string.h>

t_command_status    handle_commands(t_server *server, t_client *client);
static void         handle_help_command(t_server *server, t_client *client);
static void         handle_nick_command(t_server *server, t_client *client);
static bool         is_nickname_taken(t_server *server, const char *nickname);
static void         handle_list_command(t_server *server, t_client *client);
static void         handle_quit_command(t_server *server, t_client *client);
static bool         command_terminated(const char c, const char *end_characters);

t_command_status    handle_commands(t_server *server, t_client *client)
{
    const char   end_characters[] = {'\n', '\0'};

    if (!strncmp(client->message, "/help", 5) && command_terminated(client->message[5], end_characters))
    {
        handle_help_command(server, client);
        return (CMD_STATUS_HELP_SENT);
    }
    else if (!strncmp(client->message, "/nick ", 6))
    {
        handle_nick_command(server, client);
        return (CMD_STATUS_NICKNAME_CHANGED);
    }
    else if (!strncmp(client->message, "/list", 5) && command_terminated(client->message[5], end_characters))
    {
        handle_list_command(server, client);
        return (CMD_STATUS_CLIENT_LIST_SENT);
    }
    else if (!strncmp(client->message, "/quit", 5) && command_terminated(client->message[5], end_characters))
    {
        handle_quit_command(server, client);
        return (CMD_STATUS_CLIENT_QUIT);
    }
    return (CMD_STATUS_NONE);
}

static void handle_help_command(t_server *server, t_client *client)
{
    snprintf(server->log_buffer, sizeof(server->log_buffer),
        "Available commands:\n"
        "/help - Show this help message\n"
        "/nick <nickname> - Change your nickname\n"
        "/list - List all connected clients\n"
        "/quit - Disconnect from the server\n");
    if (append_to_send_buffer(server, client->fd, server->log_buffer, strlen(server->log_buffer)) == CLIENT_ERR_NONE)
        FD_SET(client->fd, &server->active_write);
}

static void handle_nick_command(t_server *server, t_client *client)
{
    char        old_nickname[MAX_NICKNAME_SIZE];
    char        *new_nickname;
    size_t      new_nickname_size;

    memcpy(old_nickname, client->nickname, sizeof(old_nickname));
    new_nickname = client->message + 6;
    while (*new_nickname == ' ')
        ++new_nickname;
    new_nickname_size = strlen(new_nickname);
    if (new_nickname_size > 0 && new_nickname[new_nickname_size - 1] == '\n')
        new_nickname[--new_nickname_size] = '\0';
    if (!new_nickname_size)
    {
        snprintf(server->log_buffer, sizeof(server->log_buffer), "%s Nickname cannot be empty.\n", server->timestamp);
        if (append_to_send_buffer(server, client->fd, server->log_buffer, strlen(server->log_buffer)) == CLIENT_ERR_NONE)
            FD_SET(client->fd, &server->active_write);
        return ;
    }
    else if (new_nickname_size >= MAX_NICKNAME_SIZE)
    {
        snprintf(server->log_buffer, sizeof(server->log_buffer), "%s Nickname too long. Maximum length is %d characters.\n", server->timestamp, MAX_NICKNAME_SIZE - 1);
        if (append_to_send_buffer(server, client->fd, server->log_buffer, strlen(server->log_buffer)) == CLIENT_ERR_NONE)
            FD_SET(client->fd, &server->active_write);
        return ;
    }
    else if (is_nickname_taken(server, new_nickname))
    {
        snprintf(server->log_buffer, sizeof(server->log_buffer), "%s Nickname '%s' is already taken.\n", server->timestamp, new_nickname);
        if (append_to_send_buffer(server, client->fd, server->log_buffer, strlen(server->log_buffer)) == CLIENT_ERR_NONE)
            FD_SET(client->fd, &server->active_write);
        return ;
    }
    memcpy(client->nickname, new_nickname, new_nickname_size + 1);
    update_timestamp(server);
    snprintf(server->log_buffer, sizeof(server->log_buffer), "%s %s changed their nickname to %s\n", server->timestamp, old_nickname, client->nickname);
    broadcast(server, -1);
}

static bool is_nickname_taken(t_server *server, const char *nickname)
{
    t_client    *current = server->head;

    while (current)
    {
        if (!strcmp(current->nickname, nickname))
            return (true);
        current = current->next;
    }
    return (false);
}

static void handle_list_command(t_server *server, t_client *client)
{
    t_client    *current = server->head;
    char        list_buffer[1024] = "Connected clients:\n";
    size_t      offset = strlen(list_buffer);

    while (current)
    {
        offset += snprintf(list_buffer + offset, sizeof(list_buffer) - offset, "%s\n", current->nickname);
        current = current->next;
    }
    if (append_to_send_buffer(server, client->fd, list_buffer, strlen(list_buffer)) == CLIENT_ERR_NONE)
        FD_SET(client->fd, &server->active_write);
}

static void handle_quit_command(t_server *server, t_client *client)
{
    remove_client(server, client->fd);
}

static bool command_terminated(const char c, const char *end_characters)
{
    return (!(c ^ end_characters[0] && c ^ end_characters[1]));
}