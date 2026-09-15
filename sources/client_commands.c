#include "server.h"
#include <stdio.h>
#include <string.h>

t_command_status    handle_commands(t_server *server, t_client *client);
static void         handle_help_command(t_server *server, t_client *client);
static void         handle_nick_command(t_server *server, t_client *client);
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
    snprintf(server->buffer, sizeof(server->buffer),
        "Available commands:\n"
        "/help - Show this help message\n"
        "/nick <nickname> - Change your nickname\n"
        "/list - List all connected clients\n"
        "/quit - Disconnect from the server\n");
    send(client->fd, server->buffer, strlen(server->buffer), 0);
}

static void handle_nick_command(t_server *server, t_client *client)
{
    char        old_nickname[strlen(client->nickname) + 1];
    char        *new_nickname;
    size_t      new_nickname_len;

    memcpy(old_nickname, client->nickname, sizeof(old_nickname));
    new_nickname = client->message + 6;
    new_nickname_len = strlen(new_nickname);
    if (new_nickname_len >= sizeof(client->nickname))
    {
        snprintf(server->buffer, sizeof(server->buffer), "Nickname too long. Max length is %zu characters.\n", sizeof(client->nickname) - 1);
        send(client->fd, server->buffer, strlen(server->buffer), 0);
        return ;
    }
    memcpy(client->nickname, new_nickname, new_nickname_len);
    client->nickname[new_nickname_len - 1] = '\0';
    update_timestamp(server);
    snprintf(server->buffer, sizeof(server->buffer), "%s %s changed their nickname to %s", server->timestamp, old_nickname, new_nickname);
    broadcast(server, -1);
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
    send(client->fd, list_buffer, strlen(list_buffer), 0);
}

static void handle_quit_command(t_server *server, t_client *client)
{
    remove_client(server, client->fd);
}

static bool command_terminated(const char c, const char *end_characters)
{
    return (!(c ^ end_characters[0] && c ^ end_characters[1]));
}