#ifndef __SERIAL_H__
#define __SERIAL_H__
	/*
	 *function_name: uart_set
	 *function: configure uart arguments
	 *arguments:
	 *	int fd: open serial device return the file descriptor
	 *	int speed: baud rate
	 *	int flow_ctrl: flow control
	 *	int databits: data bits
	 *	int stopbits: stop bits
	 *	int parity: parity
	 *return: 0 is success, -1 is error.
	 */
	int uart_set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);

	/*
	 *function_name: init_uart
	 *function: init uart(open the serial device and configure uart arguments)
	 *arguments: void
	 *return: serial device file descriptor is success, -1 is error. 
	 */
	int init_uart();

	/*
	 *function_name: writecmd
	 *function: send str by serial port
	 *arguments:
	 *	int fd: serial device file descriptor
	 *	const char *str: control command
	 *return: sucess return write data len, -1 is error.
	 */
	int writecmd(int fd, const unsigned char *str);

	/*
	 *function_name: recvmsg
	 *function: recv message form serial ports.
	 *arguments:
	 *	int fd: serial device file descriptor
	 *	char *buf: send buf
	 *	int data_len: want to send buf len
	 *return: success is len(>0), -1 is error.
	 */
	int myrecvmsg(int fd, char *buf, int data_len);
#endif
