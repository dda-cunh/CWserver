#include "../../inc/cwserver.h"

static unsigned char	n_digs(long n)
{
	if (n == 0)
		return (0);
	if (n < 0)
		return (2 + n_digs(-n / 10));
	return (1 + n_digs(n / 10));
}

char	*ut_itoa(int n)
{
	unsigned char	len;
	unsigned char	neg;
	char			*a;

	if (n == 0)
		return (strdup("0"));
	len = n_digs(n);
	neg = 0;
	if (n < 0)
		neg = 1;
	a = (char *) calloc(len + 1, sizeof(char));
	if (!a)
		return (NULL);
	while (n)
	{
		a[--len] = abs(n % 10) + '0';
		n /= 10;
	}
	if (neg)
		a[--len] = '-';
	return (a);
}
