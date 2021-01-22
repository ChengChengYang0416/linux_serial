#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <inttypes.h>
#include <string.h>

#define LENGTH_OF_MESSAGE 100
#define LENGTH_OF_LOOP 5
#define SEND_FREQUENCY 10

int serial_init(char *port_name)
{
	//open the port
	int serial_fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(serial_fd == -1) {
		return -1;
	}

	//config the port
	struct termios options;

	tcgetattr(serial_fd, &options);

	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	tcflush(serial_fd, TCIFLUSH);
	tcsetattr(serial_fd, TCSANOW, &options);

	return serial_fd;
}

void serial_puts(int serial_fd, char *s, size_t size)
{
	write(serial_fd, s, size);
}

char serial_getc(int serial_fd)
{
	char c;
	int received_len = 0;

	do {
		received_len = read(serial_fd, (void *)&c, 1);
	} while(received_len <= 0);

	return c;
}

int main(void)
{
	int serial_fd = serial_init("/dev/ttyUSB1");
	if(serial_fd == -1) {
		printf("failed to open the serial port.\n");
		return 0;
	}

	// produce messages
	char msg[LENGTH_OF_MESSAGE];
	for (int i = 0; i < LENGTH_OF_MESSAGE; i++) {
		msg[i] = '1';
	}
	msg[LENGTH_OF_MESSAGE-1] = '\n';

	int delay_time_us = 1000000/SEND_FREQUENCY;
	for (int i = 0; i < LENGTH_OF_LOOP; i++) {
		usleep(delay_time_us);
		serial_puts(serial_fd, msg, strlen(msg));
	}
	printf("I sent %d bytes of messages.\n", LENGTH_OF_MESSAGE*LENGTH_OF_LOOP);

	close(serial_fd);

	return 0;
}
