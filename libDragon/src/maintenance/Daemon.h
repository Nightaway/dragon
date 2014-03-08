#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <sys/stat.h>
#include <fcntl.h>

#define BD_MAX_CLOSE 8192

int becameDaemon()
{
	switch (fork())
	{
		case -1: return -1;
		case  0: break;
		default : _exit(EXIT_SUCCESS);
	}

	if (setsid() == -1) {
		return -1;
	}

	switch (fork())
	{
		case -1: return -1;
		case  0: break;
		default : _exit(EXIT_SUCCESS);
	}

	umask(0);
	//chdir("/");


	//int maxfd = sysconf(_SC_OPEN_MAX);
	//if (maxfd == -1)
	//	maxfd = BD_MAX_CLOSE; 

	int fd;
	//for (fd = 0; fd < maxfd; fd++)
	//	close(fd);

	close(STDIN_FILENO); /* Reopen standard fd's to /dev/null */
	fd = open("/dev/null", O_RDWR);
	if (fd != STDIN_FILENO) /* 'fd' should be 0 */
		return -1;
	if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
		return -1;
	if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
		return -1;

	return 0;
}

#endif

