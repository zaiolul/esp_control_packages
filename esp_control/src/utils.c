#include <fcntl.h>           
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include "libserialport.h"
#include "ubus_utils.h"

int send_serial_message(int fd, char* message)
{
	int len = strlen(message);
	len = write(fd, message, len);
	return len;
}
int read_serial_message(int fd, char* message)
{
	sleep(2); // give time to device to send and receive data;
	memset(message, 0, 255);
	int len = read(fd, message, 255);
	return len;
}
int open_port(char* port)
{
	struct termios options;
	char text[255];
	int fd = open(port, O_RDWR| O_NDELAY | O_NOCTTY | O_SYNC);
	if (fd < 0){
		return -1;
	}
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	/* Apply the settings */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	return fd;
}
int send_to_esp(char* port, char* message, struct blob_buf *buf)
{
	int fd = open_port(port);
	if(fd < 0){
		blobmsg_add_string(buf, "Error", "Can't open serial port");
		return -1;
	}
	int len = send_serial_message(fd, message);
	len = read_serial_message(fd, message);
	blobmsg_add_json_from_string(buf, message);
	close(fd);
	return 0;
}
int list_esp_devices(struct blob_buf *buf)
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);

	int connected_devices = -1;

	if (result != SP_OK) {
		//blobmsg_add_string(buf, "Error", "Can't read serial ports");
		return -1;
	}
	void* arr = NULL;

	for (int i = 0; port_list[i] != NULL; i++) {
		struct sp_port *port = port_list[i];
		char *port_name = sp_get_port_name(port);
		enum sp_transport transport = sp_get_port_transport(port);
		int usb_vid, usb_pid;
		if (transport != SP_TRANSPORT_USB) {
			continue; 
		}
		sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
		if(usb_vid == 0x10C4 && usb_pid == 0xEA60){
			if(arr == NULL){
				arr = blobmsg_open_array(buf, "devices");
			}
			int usb_bus, usb_address;
			sp_get_port_usb_bus_address(port, &usb_bus, &usb_address);

			char name[100];
			char vid[30];
			char pid[30];

			sprintf(name, "ESP Device (%d, %d)", usb_bus, usb_address);
			sprintf(vid, "%04X", usb_vid);
			sprintf(pid, "%04X", usb_pid);

			void *tbl = blobmsg_open_table(buf, name);
			blobmsg_add_string(buf, "port", port_name);
			blobmsg_add_string(buf, "vid", vid);
			blobmsg_add_string(buf, "pid", pid);
			blobmsg_close_table(buf, tbl);
			connected_devices++;
		}
	}
	if(arr != NULL){
		blobmsg_close_array(buf, arr);
	}
	sp_free_port_list(port_list);
	return connected_devices;
}
