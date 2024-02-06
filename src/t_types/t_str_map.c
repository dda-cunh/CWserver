#include "../../inc/cwserver.h"

static void	set(t_str_map *self, char *key, char *value)
{
	while (self)
	{
		if (strcmp(self->key, key) == 0)
		{
			free(self->value);
			self->value = strdup(value);
			break ;
		}
		self = self->next;
	}
}

static void	add(t_str_map *self, char *key, char *value)
{
	if (self)
	{
		while (self->next)
			self = self->next;
		self->next = t_str_map_new(key, value);
	}
	else
		self = t_str_map_new(key, value);
}

static char	*_get(t_str_map *self, char *key)
{
	while (self)
	{
		if (strcmp(self->key, key) == 0)
			return (self->value);
		self = self->next;
	}
	return (NULL);
}

void	dispose(t_str_map *self)
{
	t_str_map	*next;

	while (self)
	{
		next = self->next;
		if (self->key)
			free(self->key);
		if (self->value)
			free(self->value);
		free(self);
		self = next;
	}
}

t_str_map	*t_str_map_new(char *key, char *value)
{
	t_str_map	*map;

	map = calloc(1, sizeof(t_str_map));
	if (!map)
		return (NULL);
	map->next = NULL;
	map->key = strdup(key);
	map->value = strdup(value);
	map->set = set;
	map->add = add;
	map->get = _get;
	map->dispose = dispose;
	return (map);
}