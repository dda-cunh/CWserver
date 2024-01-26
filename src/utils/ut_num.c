#include "../../inc/cwserver.h"

char	*ut_itoa(int i)
{
	char	*str;
	int		len;
	int		neg;

	neg = (i < 0);
	len = 1;
	while (i /= 10)
		len++;
	str = (char *)malloc(sizeof(char) * (len + neg + 1));
	if (!str)
		return (NULL);
	str[len + neg] = '\0';
	while (len--)
	{
		str[len + neg] = '0' + (neg ? -(i % 10) : (i % 10));
		i /= 10;
	}
	if (neg)
		str[0] = '-';
	return (str);
}
