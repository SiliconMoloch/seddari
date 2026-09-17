#include <unistd.h>
#include <string.h>
#include <stdint.h>

void    ft_putchar(const char c)
{
    write(1, &c, 1);
}

void    putnbr(uint64_t n)
{
    if (n > 9)
        putnbr(n / 10);
    
    const char digit = (n % 10) + '0';
    ft_putchar(digit);
}

void    putstr(const char *s)
{
    write(1, s, strlen(s));
}