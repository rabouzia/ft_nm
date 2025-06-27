#include "ft_nm.h"


void *ft_memcpy(void *dest, const void *src, size_t n)
{
	char *d = (char *)dest;
	const char *s = (const char *)src;
	while (n--)
		*d++ = *s++;
	return dest;
}

char *ft_strdup(const char *src)
{
	size_t len = strlen(src);
	char *dup = malloc(len + 1);
	if (!dup)
		return NULL;
	memcpy(dup, src, len);
	dup[len] = '\0';
	return dup;
}