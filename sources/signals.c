#include "signals.h"
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t	g_stop = 0;

void	handle_signal(int32_t signal)
{
	(void)signal;
	g_stop = 1;
}

void	setup_signals(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}
