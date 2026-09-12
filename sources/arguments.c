#include "server.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void        handle_arguments(t_server *server, int argc, const char *argv[]);
static bool help_flag(const char *arg);
static void print_help(const char *program_name);
static bool address_flag(const char *arg);
static void handle_address_flag(t_server *server, const char *arg);
static bool port_flag(const char *arg);
static void handle_port_flag(t_server *server, const char *arg);
static void handle_unknown_flag(const char *arg);

void    handle_arguments(t_server *server, int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (help_flag(argv[i]))
            print_help(argv[0]);
        else if (address_flag(argv[i]))
            handle_address_flag(server, argv[++i]);
        else if (port_flag(argv[i]))
            handle_port_flag(server, argv[++i]);
        else
            handle_unknown_flag(argv[i]);
    }
}

static bool help_flag(const char *arg)
{
    return (!strcmp(arg, "-h") || !strcmp(arg, "--help"));
}

static void print_help(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message and exit\n");
    printf("  -a, --address  Set the IP address (default: 127.0.0.1)\n");
    printf("  -p, --port     Set the port (default: 8080)\n");
    exit(0);
}

static bool address_flag(const char *arg)
{
    return (!strcmp(arg, "-a") || !strcmp(arg, "--address"));
}

static void handle_address_flag(t_server *server, const char *arg)
{
    if (!arg || !*arg)
    {
        fprintf(stderr, "IP address is required after -a or --address\n");
        exit(1);
    }
    if (!strcmp(arg, "localhost"))
    {
        server->ip = htonl(0x7F000001);
        return ;
    }
    if (!inet_pton(AF_INET, arg, (void *)&server->ip))
    {
        fprintf(stderr, "Invalid IP address: %s\n", arg);
        exit(1);
    }
}

static bool port_flag(const char *arg)
{
    return (!strcmp(arg, "-p") || !strcmp(arg, "--port"));
}

static void handle_port_flag(t_server *server, const char *arg)
{
    if (!arg || !*arg)
    {
        fprintf(stderr, "Port number is required after -p or --port\n");
        exit(1);
    }

    char        *endptr;
    const long  port = strtol(arg, &endptr, 10);

    if (*endptr ^ '\0' || port < 1 || port > 65535)
    {
        fprintf(stderr, "Invalid port number: %s\n", arg);
        exit(1);
    }
    server->port = (uint16_t)port;
}

static void handle_unknown_flag(const char *arg)
{
    fprintf(stderr, "Unknown option: %s\n", arg);
    exit(1);
}