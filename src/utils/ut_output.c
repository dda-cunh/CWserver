#include "../../inc/cwserver.h"

void	ut_putchar_fd(int fd, char c)
{
	write(fd, &c, 1);
}

void	ut_putendl_fd(int fd, const char *str)
{
	if (!str || !*str)
		return ;
	write(fd, str, strlen(str));
	ut_putchar_fd(fd, '\n');
}

void	ut_puterror(const char *header, const char *str)
{
	if (!str || !*str)
		return ;
	ut_putstr_fd(STD_ERR, ANSI_COLOR_RED);
	if (header)
	{
		ut_putstr_fd(STD_ERR, header);
		ut_putstr_fd(STD_ERR, ": ");
	}
	ut_putendl_fd(STD_ERR, str);
	ut_putstr_fd(STD_ERR, ANSI_COLOR_RESET);
}

void	ut_putstr_fd(int fd, const char *str)
{
	if (!str || !*str)
		return ;
	write(fd, str, strlen(str));
}
