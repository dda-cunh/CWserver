#ifndef	_CWSERVER_H_
# define _CWSERVER_H_

# include <netinet/in.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>

# define STD_IN			0
# define STD_OUT		1
# define STD_ERR		2

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	35000
# endif

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct sockaddr_in	t_sockaddr_in;

typedef struct s_byte_array
{
	unsigned char	*bytes;
	size_t			length;
}	t_byte_array;

typedef struct s_server
{
	unsigned long		interface;
	int					protocol;	
	int					backlog;
	int					service;	
	int					domain;
	int					socket;
	int					port;
	t_sockaddr_in		address;

	void				(*up)(struct s_server *server);
}	t_server;

typedef struct s_request
{
	t_byte_array	body;
	char			*method;
	char			*path;
	char			*query_string;
	char			*content_type;
}	t_request;

typedef struct s_response
{
	t_byte_array	body;
	char			*status_message;
	char			*content_type;
	int				status_code;
}	t_response ;

t_server		*t_server_new(unsigned long interface, int domain, int protocol,
								int service, int port, int backlog);
void			t_server_del(t_server *server);
void			send_response(int client_fd, t_response response);

/* ************************************************************************** */
/*                              t_byte_array                                  */
/* ************************************************************************** */
void			append_to_bytes(t_byte_array *bytes, t_byte_array byte_arr);
void			append_str_to_bytes(t_byte_array *bytes, char *str);

/* ************************************************************************** */
/*                                    UTILS                                   */
/* ************************************************************************** */
void			ut_putendl_fd(int fd, const char *str);
void			ut_putstr_fd(int fd, const char *str);
void			ut_puterror(const char *hearder, const char *str);
void			*ut_memmove(void *dest, const void *src, size_t n);
void			ut_puchar_fd(int fd, char c);
char			*ut_itoa(int i);


#endif