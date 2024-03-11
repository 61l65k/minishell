/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 23:55:43 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/28 13:01:25 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifdef __linux__
#  include <linux/limits.h>
#  include <stdint.h>
# endif
# include "./ft_printf/ft_printf.h"
# include "./get_next_line/get_next_line_bonus.h"
# include "./vec/vec.h"
# include <limits.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	char			*content;
	bool			is_quoted_redirector;
	bool			ambigious_redirect;
	struct s_list	*next;
}					t_list;

int					ft_isupper(int c);
size_t				ft_envlen(char *env);
char				*ft_strndup(const char *s1, size_t n);
char				*ft_strncat(char *dest, const char *src, size_t n);
void				*ft_realloc(void *old_ptr, size_t old_size,
						size_t new_size);
int					ft_isalnum(int c);
int					ft_isenv_var(int c);
int					ft_strcmp(const char *s1, const char *s2);
double				ft_atof(const char *str);
long				ft_atol(const char *str);
int					ft_isenv_var(int c);
int					ft_isprint(int c);
int					ft_isdigit(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);
size_t				ft_strlen(const char *s);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
void				*ft_memset(void *b, int c, size_t len);
int					ft_atoi(const char *str);
void				ft_bzero(void *s, size_t n);
char				*ft_strdup(const char *s1);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
size_t				ft_strlcpy(char *dest, const char *src, size_t destsize);
size_t				ft_strlcat(char *dst, const char *src, size_t destsize);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_putnbr_fd(int n, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putchar_fd(char c, int fd);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new_node);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new_node);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
#endif
