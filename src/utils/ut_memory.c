#include "../../inc/cwserver.h"

void	*ut_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*src_bytes;
	unsigned char	*dest_bytes;
	size_t			i;

	i = -1;
	src_bytes = (unsigned char *)src;
	dest_bytes = (unsigned char *)dest;
	if (dest_bytes >= src_bytes && dest_bytes <= src_bytes + n)
	{
		if (dest)
			while (n--)
				dest_bytes[n] = src_bytes[n];
	}
	else
		while (++i < n)
			dest_bytes[i] = src_bytes[i];
	return (dest);
}