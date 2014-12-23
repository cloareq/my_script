/*
** myscript.c for myscript in /home/cloare_q/PROJETS/PSU_2013_myscript
** 
** Made by Quentin Cloarec
** Login   <cloare_q@epitech.net>
** 
** Started on  Wed Feb 26 16:20:48 2014 Quentin Cloarec
** Last update Wed Feb 26 16:20:50 2014 Quentin Cloarec
*/

#include "main.h"

void	init_fd(t_data *data)
{
	FD_ZERO(&(data->fd_in));
	FD_SET(0, &(data->fd_in));
	FD_SET(data->fdm, &(data->fd_in));
}

void	verif_isset(t_data *data, char input[150])
{
	data->rlen = read(data->fdm, input, sizeof(input));
	if (data->rlen > 0)
	{
		write(1, input, data->rlen);
		write(data->fd, input, data->rlen);
	}
	else
	{
		if (data->rlen < 0)
		{
			if (errno == 5)
			{
				write(1, input, data->rlen);
				printf("Script done, file is %s\n", data->file);
				write(data->fd, "\nScript done on ",
				strlen("\nScript done on "));
				write(data->fd, data->date, strlen(data->date));
			}
			exit(1);
		}
	}
}

void	isset_zero(t_data *data, char input[150])
{
	data->rlen = read(0, input, sizeof(input));
	if (data->rlen > 0)
	{
		write(data->fdm, input, data->rlen);
		write(data->fd, input, data->rlen);
	}
	else
	{
		if (data->rlen <= 0)
		{
			printf("exit\nscript done file is %s\n", data->file);
			write(data->fd, "exit\n", 6);
			write(data->fd, "\nScript done on ", strlen("\nScript done on "));
			write(data->fd, data->date, strlen(data->date));
			exit(1);
		}
	}
}

void	my_fork(t_data *data, char input[150])
{
	setsid();
	ioctl(data->fds, TIOCSCTTY, NULL);
	close(data->fds);
	printf("Script started, file is %s\n", data->file);
	write(data->fd, "Script started on ", strlen("Script started on "));
	write(data->fd, data->date, strlen(data->date));
	while (42)
	{
		init_fd(data);
		data->rlen = select(data->fdm + 1, &(data->fd_in), NULL, NULL, NULL);
		if (data->rlen)
		{
			if (data->rlen == -1)
				exit(1);
			else
			{
				if (FD_ISSET(0, &(data->fd_in)))
					isset_zero(data, input);
				if (FD_ISSET(data->fdm, &(data->fd_in)))
					verif_isset(data, input);
			}
		}
	}
}