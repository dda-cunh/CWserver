#include "../../inc/cwserver.h"
#include <stddef.h>

void	append_str_to_bytes(t_byte_array *bytes, char *str)
{
	size_t	append_len;

	append_len = strlen(str);
	if (append_len+ bytes->length > bytes->capacity)
		bytes->bytes = (unsigned char *)realloc(bytes->bytes,
						sizeof(unsigned char) * (bytes->length + append_len));
	ut_memmove(bytes->bytes + bytes->length, str, append_len);
	bytes->length += append_len;
}

void	append_to_bytes(t_byte_array *bytes, t_byte_array to_append)
{
	if (to_append.length + bytes->length > bytes->capacity)
		bytes->bytes = (unsigned char *)realloc(bytes->bytes,
						sizeof(unsigned char) * (bytes->length + to_append.length));
	ut_memmove(bytes->bytes + bytes->length, to_append.bytes, to_append.length);
	bytes->length += to_append.length;
}

static void	t_byte_array_dispose(t_byte_array *self)
{
	if (self)
	{
		if (self->bytes)
			free(self->bytes);
		free(self);
	}
}

t_byte_array	*t_byte_array_new()
{
	const size_t	initial_capacity = 16;
	t_byte_array	*byte_array;

	byte_array = (t_byte_array *)malloc(sizeof(t_byte_array));
	if (!byte_array)
		return (NULL);
	byte_array->bytes = malloc(initial_capacity);
	if (!byte_array->bytes)
	{
		free(byte_array);
		return (NULL);
	}
	byte_array->length = 0;
	byte_array->capacity = initial_capacity;
	byte_array->dispose = t_byte_array_dispose;
	return (byte_array);
}
