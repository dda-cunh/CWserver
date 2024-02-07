#include "../../inc/cwserver.h"


static t_str_map	*clone(t_str_map *self)
{
	t_str_map	*clone;

	clone = NULL;
	while (self)
	{
		if (!clone)
			clone = t_str_map_new(self->key, self->value);
		else
			clone->add(&clone, self->key, self->value);
		self = self->next;
	}
	return (clone);
}

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

static void	add(t_str_map **self, char *key, char *value)
{
	t_str_map	*current;

	if (self)
	{
		if (*self)
		{
			current = *self;
			while (current->next)
			{
				if (strcmp(current->key, key) == 0)
				{
					free(current->value);
					current->value = strdup(value);
					return ;
				}
				current = current->next;
			}
			if (strcmp(current->key, key) == 0)
			{
				free(current->value);
				current->value = strdup(value);
				return ;
			}
			current->next = t_str_map_new(key, value);
		}
		else
			*self = t_str_map_new(key, value);
	}
}

static void	_link(t_str_map **self, t_str_map *map)
{
	t_str_map	*current;

	if (self && map)
	{
		if (*self)
		{
			current = *self;
			while (current->next)
				current = current->next;
			current->next = map;
		}
		else
			*self = map;
	}
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

t_str_map	*map_from_form(char *str)
{
	t_str_map	*map;
	char		**split;
	char		**pair;
	int			i;

	split = ut_split(str, '&');
	if (!split)
		return (NULL);
	map = NULL;
	for (i = 0; split[i]; i++)
	{
		pair = ut_split(split[i], '=');
		if (!pair)
		{
			ut_puterror("Error: ", "ut_split failed in map_from_str");
			free_2d_str(split);
			if (map)
				map->dispose(map);
			return (NULL);
		}
		if (pair[0] && pair[1])
		{
			if (!map)
				map = t_str_map_new(pair[0], pair[1]);
			else
				map->add(&map, pair[0], pair[1]);
		}
		free_2d_str(pair);
	}
	free_2d_str(split);
	return (map);
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
	map->link = _link;
	map->get = _get;
	map->dispose = dispose;
	map->clone = clone;
	return (map);
}