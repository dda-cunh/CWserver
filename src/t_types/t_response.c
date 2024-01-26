#include "../../inc/cwserver.h"

void	send_response(int client_fd, t_response response)
{
	t_byte_array	bytes;
	char			*code_str;

	bytes = (t_byte_array){NULL, 0};
	append_str_to_bytes(&bytes, "HTTP/1.1 ");
	code_str = ut_itoa(response.status_code);
	append_str_to_bytes(&bytes, code_str);
	free(code_str);
	append_str_to_bytes(&bytes, " ");
	append_str_to_bytes(&bytes, response.status_message);
	append_str_to_bytes(&bytes, "\r\n");
	append_str_to_bytes(&bytes, "Content-Type: ");
	append_str_to_bytes(&bytes, response.content_type);
	append_str_to_bytes(&bytes, "\r\n");
	append_to_bytes(&bytes, response.body);
	write(client_fd, bytes.bytes, bytes.length);
	free(bytes.bytes);
}
