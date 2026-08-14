#include <signal.h>
#include <stdint.h>

extern volatile sig_atomic_t	g_stop;

void	setup_signals(void);
