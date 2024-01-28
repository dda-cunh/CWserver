#ifndef	_CWSERVER_H_
# define _CWSERVER_H_

# include <netinet/in.h>
#include <stddef.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>

# define ROOT_PATH			"www"

# define STD_IN				0
# define STD_OUT			1
# define STD_ERR			2

# ifndef SOCK_BUFFER_SIZE
#  define SOCK_BUFFER_SIZE	35000
# endif

# ifndef FILE_BUFFER_SIZE
#  define FILE_BUFFER_SIZE	4096
# endif

# define HTTP_BAD_REQ		400
# define HTTP_OK			200
# define HTTP_NOT_FOUND		404

# define ANSI_COLOR_GREEN	"\x1b[32m"
# define ANSI_COLOR_RED		"\x1b[31m"
# define ANSI_COLOR_RESET	"\x1b[0m"

typedef struct sockaddr_in	t_sockaddr_in;

typedef enum e_http_method
{
	HTTP_GET,
	HTTP_POST,
	HTTP_UNKNOWN
}	t_http_method;

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_byte_array
{
	unsigned char		*bytes;
	size_t				capacity;
	size_t				length;

	void				(*dispose)(struct s_byte_array *self);
}	t_byte_array;

// typedef struct s_str_map
// {
// 	char				*key;
// 	char				*value;

// 	void				(*dispose)(struct s_str_map *self);
// }	t_str_map;

typedef struct s_server
{
	t_sockaddr_in		address;
	unsigned long		interface;
	int					logger_fd;
	int					protocol;	
	int					backlog;
	int					service;	
	int					domain;
	int					socket;
	int					port;

	void				(*dispose)(struct s_server *self);
	void				(*up)(struct s_server *self);
}	t_server;

typedef struct s_request
{
	t_http_method		method;
	t_byte_array		*body;
	// t_str_map			*query_strings;
	char				*path;

	void				(*dispose)(struct s_request *self);
}	t_request;

typedef struct s_response
{
	t_byte_array		*body;
	char				*status_message;
	char				*content_type;
	int					status_code;

	void				(*dispose)(struct s_response *self);
}	t_response ;

/* ************************************************************************** */
/*                                 t_server                                   */
/* ************************************************************************** */
t_server				*t_server_new(unsigned long interface, int domain,
										int protocol, int service, int port,
										int backlog);

/* ************************************************************************** */
/*                                 t_request                                  */
/* ************************************************************************** */
t_request				*parse_request(t_byte_array *req_bytes);
t_request				*read_request(int client_fd);


/* ************************************************************************** */
/*                                t_response                                  */
/* ************************************************************************** */
t_response				*t_response_new(t_byte_array *body, char *stat_message,
										char *content_type, int status_code);
void					dump_response(int client_fd, t_response *response);
t_response				*parse_response(t_request request);


/* ************************************************************************** */
/*                               t_byte_array                                 */
/* ************************************************************************** */
t_byte_array			*t_byte_array_new();
void					append_to_bytes(t_byte_array *bytes,
										t_byte_array byte_arr);
void					append_str_to_bytes(t_byte_array *bytes, char *str);

/* ************************************************************************** */
/*                                    UTILS                                   */
/* ************************************************************************** */
t_byte_array			*read_all_from_file(int fd);
void					ut_putendl_fd(int fd, const char *str);
void					ut_putstr_fd(int fd, const char *str);
void					ut_puterror(const char *hearder, const char *str);
void					*ut_memmove(void *dest, const void *src, size_t n);
void					*ut_memset(void *s, int c, size_t n);
void					ut_puchar_fd(int fd, char c);
char					*ut_substr(char const *s, unsigned int start,
									size_t len);
char					*ut_strjoin(char const *s1, char const *s2);
char					**ut_split(char const *s, char c);
char					*ut_itoa(int i);
int						open_server_file(char *path);

#endif