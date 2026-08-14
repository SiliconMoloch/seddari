#include "signals.h"

volatile sig_atomic_t	g_stop = 0;

void	handle_signal(int32_t signal)
{
	(void)signal;
	g_stop = 1;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
}
