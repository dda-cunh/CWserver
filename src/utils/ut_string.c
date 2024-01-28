#include "../../inc/cwserver.h"

char	*ut_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	size_t	i;
	char	*concat;

	total_len = strlen(s1) + strlen(s2);
	concat = (char *)malloc(total_len + 1);
	i = 0;
	if (!concat)
		return (NULL);
	while (*s1)
		concat[i++] = *s1++;
	while (*s2)
		concat[i++] = *s2++;
	concat[i] = '\0';
	return (concat);
}

char	*ut_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	i;
	char	*sub_str;

	s_len = strlen(s);
	if (start > s_len || len == 0)
		return (strdup(""));
	if (start + len > s_len)
		len = s_len - start;
	sub_str = (char *)calloc(len + 1, sizeof(char));
	if (!sub_str)
		return (NULL);
	i = -1;
	while (++i < len)
		sub_str[i] = s[i + start];
	return (sub_str);
}

static int	n_strings(char const *s, char c)
{
	int	start;
	int	i;
	int	n;

	n = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (s[start])
			n++;
	}
	return (n);
}

char	**ut_split(char const *s, char c)
{
	char	**sep;
	int		start;
	int		i;
	int		j;

	sep = (char **) calloc((n_strings(s, c) + 1), sizeof(char *));
	if (!sep)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (s[start])
			sep[j++] = ut_substr(s, start, i - start);
	}
	return (sep);
}