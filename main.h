/*
** main.h for main in /home/cloare_q/PROJETS/PSU_2013_myscript
** 
** Made by Quentin Cloarec
** Login   <cloare_q@epitech.net>
** 
** Started on  Fri Feb 28 16:00:12 2014 Quentin Cloarec
** Last update Fri Feb 28 16:00:14 2014 Quentin Cloarec
*/

#ifndef MAIN_H__
#define MAIN_H__

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_BSD
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

typedef struct s_data
{
	int fdm;
	int fds;
	int rlen;
	int fd;
	unsigned int num;
	fd_set fd_in;
	char *file;
	int option;
	char date[256];
} t_data;

void	my_fork(t_data *data, char input[150]);

#endif
