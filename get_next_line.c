/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assempas <assempas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 17:49:41 by assempas          #+#    #+#             */
/*   Updated: 2026/01/31 17:49:41 by assempas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	fill_stash(int fd, char **stash)
{
	char	*buffer;
	ssize_t	bytes;
	char	*tmp;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (-1);
	bytes = 1;
	while (bytes > 0 && !gnl_strchr(*stash, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free(buffer), -1);
		if (bytes == 0)
			break;
		buffer[bytes] = '\0';
		tmp = gnl_strjoin(*stash, buffer);
		if (!tmp)
			return (free(buffer), -1);
		*stash = tmp;
	}
	return (free(buffer), 0);
}

static char	*extract_line(const char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	if (!stash || stash[0] == '\0')
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = (char *)malloc(i + 2);
	else
		line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		line[j] = stash[j];
		j++;
	}
	if (stash[i] == '\n')
		line[j++] = '\n';
	line[j] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	char	*new_stash;
	size_t	i;
	size_t	j;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] != '\n')
		return (free(stash), NULL);
	i++;
	if (stash[i] == '\0')
		return (free(stash), NULL);
	new_stash = (char *)malloc(gnl_strlen(stash + i) + 1);
	if (!new_stash)
		return (free(stash), NULL);
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (fill_stash(fd, &stash) < 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(stash);
	stash = update_stash(stash);
	return (line);
}
