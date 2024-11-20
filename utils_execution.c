#include "minishell.h"

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;
    unsigned char *x;
    unsigned char *y;

    if (!s1 || !s2)
        return -1;

    x = (unsigned char *)s1;
    y = (unsigned char *)s2;
    i = 0;
    
    if (n == (size_t)-1)
    {
        while (x[i] && y[i] && x[i] == y[i])
            i++;
        return x[i] - y[i];
    }
    
    while (i < n && (x[i] || y[i]))
    {
        if (x[i] != y[i])
            return x[i] - y[i];
        i++;
    }
    return 0;
}

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}


char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char    *ft_strncpy(char *dest,  char *src, unsigned int n)
{
        unsigned int i;
        i = 0;
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n) 
    {
        dest[i] = '\0';
        i++;
    }
        
    return dest;    
}
void	*ft_memset(void *big, int to_find, size_t len)
{
	size_t	i;
	char	*str;

	str = (char *)big;
	i = 0;
	while (i < len)
	{
		str[i] = to_find;
		i++;
	}
	return (big);
}