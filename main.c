/*
** main.c for main.c in /home/cloare_q/PROJETS/PSU_2013_myscript
** 
** Made by Quentin Cloarec
** Login   <cloare_q@epitech.net>
** 
** Started on  Wed Feb 26 14:53:13 2014 Quentin Cloarec
** Last update Wed Feb 26 14:53:15 2014 Quentin Cloarec
*/

#include "main.h"

int	check_error(t_data *data)
{
	unsigned int n;

	n = 0;
	if ((data->fdm = open("/dev/ptmx", O_RDWR)) ==  -1)
		return (1);
	if (data->option == 0)
		data->fd = open(data->file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else
		data->fd = open(data->file, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (data->fdm < 0)
		return (1);
	data->rlen = ioctl(data->fdm, TIOCGPTN, &(data->num));
	if (data->rlen != 0)
		return (1);
	data->rlen = ioctl(data->fdm, TIOCSPTLCK, &n);
	if (data->rlen != 0)
		return (1);
	data->fds = open(ptsname(data->fdm), O_RDWR);
	return (0);
}

void	close_and_dup(t_data *data)
{
	close(0);
	close(1);
	close(2);
	dup(data->fds);
	dup(data->fds);
	dup(data->fds);
}

void	my_execlp(t_data *data, char *shell)
{
	struct termios slave_orig_term_settings;
	struct termios new_term_settings;
	close(data->fdm);
	data->rlen = tcgetattr(data->fds, &slave_orig_term_settings);
	new_term_settings = slave_orig_term_settings;
	cfmakeraw(&new_term_settings);
	tcsetattr(data->fds, TCSANOW, &new_term_settings);
	close_and_dup(data);
	setsid();
	shell = getenv("SHELL");
	if (shell != NULL)
		execlp(shell, shell, NULL);
	else
		execlp("/bin/sh", "/bin/sh", NULL);
}

void	check_option(t_data *data, char **av, int ac)
{
	if (ac == 2 && av[1][0] != '-')
		data->file = av[1];
	else if (ac == 2 && av[1][0] == '-'
		&& av[1][1] == 'a' && strlen(av[1]) == 2)
	{
		data->file = "typescript";
		data->option = 1;
	}
	else if (ac == 3)
	{
		if (av[1][0] != '-' && av[2][0] == '-'
			&& av[2][1] == 'a' && strlen(av[2]) == 2)
		{
			data->file = av[1];
			data->option = 1;
		}
	}
	else
		data->file = "typescript";
	data->num = 0;
}

int	main(int ac, char **av)
{
	time_t timestamp = time(NULL);
	t_data *data;
	char	*shell;
	char input[150];

	shell = NULL;
	if ((data = malloc(sizeof(t_data))) == 0)
		return (-1);
	strftime(data->date, sizeof(data->date), "%a %d %b %I:%M:%S %p %Z\n",
		localtime(&timestamp));
	data->option = 0;
	check_option(data, av, ac);
	if (check_error(data) == 1)
		return (-1);
	if (fork())
		my_fork(data, input);
	else
		my_execlp(data, shell);
	return 0;
}
