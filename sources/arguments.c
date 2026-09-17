#include "server.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void            handle_arguments(t_server *server, int argc, const char *argv[]);

static bool     help_option(const char *arg);
static bool     address_option(const char *arg);
static bool     port_option(const char *arg);
static bool     metrics_option(const char *arg);
static bool     print_metrics_option(const char *arg);

static void     print_help(const char *program_name);
static void     handle_address_option(t_server *server, const char *arg);
static uint16_t get_port(const char *arg);
static void     handle_unknown_option(const char *arg);

void    handle_arguments(t_server *server, int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (help_option(argv[i]))
            print_help(argv[0]);
        else if (address_option(argv[i]))
            handle_address_option(server, argv[++i]);
        else if (port_option(argv[i]))
            server->port = get_port(argv[++i]);
        else if (metrics_option(argv[i]))
            server->metrics_port = get_port(argv[++i]);
        else if (print_metrics_option(argv[i]))
            server->print_metrics = true;
        else
            handle_unknown_option(argv[i]);
    }
}

static bool help_option(const char *arg)
{
    return (!strcmp(arg, "-h") || !strcmp(arg, "--help"));
}

static bool address_option(const char *arg)
{
    return (!strcmp(arg, "-a") || !strcmp(arg, "--address"));
}

static bool port_option(const char *arg)
{
    return (!strcmp(arg, "-p") || !strcmp(arg, "--port"));
}

static bool metrics_option(const char *arg)
{
    return (!strcmp(arg, "-m") || !strcmp(arg, "--metrics"));
}

static bool print_metrics_option(const char *arg)
{
    return (!strcmp(arg, "-pm") || !strcmp(arg, "--print_metrics"));
}

static void print_help(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("    -h, --help                                Show this help message and exit\n");
    printf("    -a, --address           <address>         Set the IP address (default: 127.0.0.1)\n");
    printf("    -p, --port              <port>            Set the port (default: 8080)\n");
    printf("    -m, --metrics           <port>            Enables the metrics listening \n");
    printf("    -pm, --print_metrics                      Print metrics on the standard output (at each update)\n");
    exit(0);
}

static void handle_address_option(t_server *server, const char *arg)
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

static uint16_t get_port(const char *arg)
{
    if (!arg || !*arg)
    {
        fprintf(stderr, "Port number is required\n");
        exit(1);
    }

    char        *endptr;
    const long  port = strtol(arg, &endptr, 10);

    if (*endptr ^ '\0' || port < 1 || port > 65535)
    {
        fprintf(stderr, "Invalid port number: %s\n", arg);
        exit(1);
    }
    return ((uint16_t) port);
}

static void handle_unknown_option(const char *arg)
{
    fprintf(stderr, "Unknown option: %s\n", arg);
    exit(1);
}