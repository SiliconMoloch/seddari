#include <unistd.h>
#include "metrics.h"
#include "utils.h"

bool    metrics_updated(t_metrics last, t_metrics current)
{
    if (last.number_of_connections ^ current.number_of_connections)
        return (true);
    else if (last.number_of_disconnections ^ current.number_of_disconnections)
        return (true);
    else if (last.successful_recv_calls ^ current.successful_recv_calls)
        return (true);
    else if (last.bytes_received ^ current.bytes_received)
        return (true);
    else if (last.complete_messages_received ^ current.complete_messages_received)
        return (true);
    else if (last.successful_send_calls ^ current.successful_send_calls)
        return (true);
    else if (last.bytes_sent ^ current.bytes_sent)
        return (true);
    else if (last.command_help_count ^ current.command_help_count)
        return (true);
    else if (last.command_nick_count ^ current.command_nick_count)
        return (true);
    else if (last.command_list_count ^ current.command_list_count)
        return (true);
    else if (last.command_quit_count ^ current.command_quit_count)
        return (true);
    return (false);
}

uint64_t metrics_count;

void    print_metrics(t_metrics metrics)
{
    putstr("Print metrics "); putnbr(++metrics_count); putchar('\n');
    putstr("Connections: "); putnbr(metrics.number_of_connections); putchar('\n');
    putstr("Disconnections: "); putnbr(metrics.number_of_disconnections); putchar('\n');
    putstr("Successful recv() calls: "); putnbr(metrics.successful_recv_calls); putchar('\n');
    putstr("Bytes recevied: "); putnbr(metrics.bytes_received); putchar('\n');
    putstr("Complete messages received: "); putnbr(metrics.complete_messages_received); putchar('\n');
    putstr("Successful send() calls: "); putnbr(metrics.successful_send_calls); putchar('\n');
    putstr("Bytes sent: "); putnbr(metrics.bytes_sent); putchar('\n');
    putstr("Command help: "); putnbr(metrics.command_help_count); putchar('\n');
    putstr("Command nick: "); putnbr(metrics.command_nick_count); putchar('\n');
    putstr("Command list: "); putnbr(metrics.command_list_count); putchar('\n');
    putstr("Command quit: "); putnbr(metrics.command_quit_count); putchar('\n');
    putchar('\n');
}