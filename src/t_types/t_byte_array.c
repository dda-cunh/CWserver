#include "../../inc/cwserver.h"

void	append_str_to_bytes(t_byte_array *bytes, char *str)
{
	size_t	append_len;

	append_len = strlen(str);
	bytes->bytes = (unsigned char *)realloc(bytes->bytes,
					sizeof(unsigned char) * (bytes->length + append_len));
	ut_memmove(bytes->bytes + bytes->length, str, append_len);
	bytes->length += append_len;
}

void	append_to_bytes(t_byte_array *bytes, t_byte_array to_append)
{
	bytes->bytes = (unsigned char *)realloc(bytes->bytes,
					sizeof(unsigned char) * (bytes->length + to_append.length));
	ut_memmove(bytes->bytes + bytes->length, to_append.bytes, to_append.length);
	bytes->length += to_append.length;
}
