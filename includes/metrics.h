#ifndef METRICS_H
#define METRICS_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct s_metrics
{
    bool        *stop;
    time_t      start_time;
    uint64_t    number_of_connections;
    uint64_t    number_of_disconnections;
    uint64_t    successful_recv_calls;
    uint64_t    bytes_received;
    uint64_t    complete_messages_received;
    uint64_t    successful_send_calls;
    uint64_t    bytes_sent;
    uint64_t    command_help_count;
    uint64_t    command_nick_count;
    uint64_t    command_list_count;
    uint64_t    command_quit_count;
}   t_metrics;

bool    metrics_updated(t_metrics last, t_metrics current);
void    print_metrics(t_metrics metrics);

#endif