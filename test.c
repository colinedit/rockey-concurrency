#include <stdlib.h>
#include "rockey.h"

#define	R4ND_P1		0xffff		// Password 1 
#define	R4ND_P2		0xffff		// Password 2 

#define	LOG_COLOR_PREFIX(x) printf("\x1b[%dm%d: ", 30 + ((x) % 7), (x))

void			print_buf_hex(const char *buf)
{
	int		i;
	char	c;

	i = -1;
	printf("Buffer content on next line:\n");
	while (++i < 9)
	{
		c = buf[i];
		if (isprint(c))
			write(1, &c, 1);
		else
			printf("%x", c);
	}
	printf("\n");
}

unsigned int	dongle_get_hardware_id(int pid)
{
	unsigned short	result;
	unsigned short	function,handle,p1,p2,p3,p4;
	unsigned int	lp1,lp2;
	unsigned char	buffer[1024];

	p1 = R4ND_P1;
	p2 = R4ND_P2;
	p3 = 0; //R4ND_P3;
	p4 = 0; //R4ND_P4;
	handle=0;
	lp1=0;
	lp2=0;
	function=RY_FIND;
	result=Rockey(function,&handle,&lp1,&lp2,&p1,&p2,&p3,&p4,(unsigned char*)buffer);
	LOG_COLOR_PREFIX(pid);
	printf("RY_FIND:\t%d\n", result);
	if (result != ERR_SUCCESS)
	{
		LOG_COLOR_PREFIX(pid);
		printf("Error with RY_FIND. Quitting.\n");
		exit(1);
	}
	else
	{
		LOG_COLOR_PREFIX(pid);
		printf("Hardware ID:\t%u\n", lp1);
	}
	return (lp1);
}

unsigned short	dongle_get_handle(int pid, unsigned int *lp1)
{
	unsigned short	result;
	unsigned short	function,handle,p1,p2,p3,p4;
	unsigned int	lp2;
	unsigned char	buffer[1024];

	p1 = R4ND_P1;
	p2 = R4ND_P2;
	p3 = 0; //R4ND_P3;
	p4 = 0; //R4ND_P4;
	handle=0;
	lp2=0;
	function=RY_OPEN;
	result=Rockey(function,&handle,lp1,&lp2,&p1,&p2,&p3,&p4,(unsigned char*)buffer);
	LOG_COLOR_PREFIX(pid);
	printf("RY_OPEN:\t%d\n", result);
	if (result != ERR_SUCCESS)
	{
		LOG_COLOR_PREFIX(pid);
		printf("Error with RY_OPEN. Quitting.\n");
		exit(1);
	}
	else
	{
		LOG_COLOR_PREFIX(pid);
		printf("Handle:\t%u\n", handle);
	}
	return (handle);
}

int				dongle_close(int pid, unsigned short *handle, unsigned int *hardware_id)
{
	unsigned short	result;
	unsigned short	function,p1,p2,p3,p4;
	unsigned int	lp2;
	unsigned char	buffer[1024];

	p1 = R4ND_P1;
	p2 = R4ND_P2;
	p3 = 0; //R4ND_P3;
	p4 = 0; //R4ND_P4;
	lp2=0;
	function=RY_CLOSE;
	result=Rockey(function,handle,hardware_id,&lp2,&p1,&p2,&p3,&p4,(unsigned char*)buffer);
	//result=0;
	LOG_COLOR_PREFIX(pid);
	printf("RY_CLOSE:\t%d\n", result);
	if (result != ERR_SUCCESS)
	{
		LOG_COLOR_PREFIX(pid);
		printf("Error with RY_CLOSE. Quitting.\n");
		exit(1);
	}
	else
	{
		LOG_COLOR_PREFIX(pid);
		printf("Closing dongle.\n");
	}
	return (result);
}

void			dongle_read(int pid, unsigned short *handle, unsigned int *hardware_id)
{
	unsigned short	result;
	unsigned short	function,p1,p2,p3,p4;
	unsigned int	lp2;
	unsigned char	buffer[1024];

	p1 = 0;
	p2 = 9; // 9 bytes for dongle serial
	p3 = 0; //R4ND_P3;
	p4 = 0; //R4ND_P4;
	lp2=0;
	function=RY_READ;
	result=Rockey(function,handle,hardware_id,&lp2,&p1,&p2,&p3,&p4,(unsigned char*)buffer);
	LOG_COLOR_PREFIX(pid);
	printf("RY_READ:\t%d\n", result);
	if (result != ERR_SUCCESS)
	{
		LOG_COLOR_PREFIX(pid);
		printf("Error with RY_READ. Closing dongle.\n");
		dongle_close(pid, handle, hardware_id);
		exit(1);
	}
	else
	{
		LOG_COLOR_PREFIX(pid);
		print_buf_hex((const char*)buffer);
	}
}

int				main(void)
{
	int				pid;
	unsigned int	hardware_id;
	unsigned short	handle;

	pid = getpid();
	hardware_id = dongle_get_hardware_id(pid);
	handle = dongle_get_handle(pid, &hardware_id);
	dongle_read(pid, &handle, &hardware_id);
	dongle_close(pid, &handle, &hardware_id);
}
