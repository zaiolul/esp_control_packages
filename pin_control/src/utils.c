#include <fcntl.h>           
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include "libserialport.h"
#include "ubus_utils.h"

int send_message(char* port, char* action, int pin, struct blob_buf *buf)
{
	struct termios options;
	char text[255];
	int fd = open(port, O_RDWR| O_NDELAY | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		blobmsg_add_string(buf, "Error", "Can't open serial port");
		return -1;
	}
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	/* Apply the settings */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	sprintf(text, "{\"action\": \"%s\", \"pin\": %d}", action, pin);
	
	int len = strlen(text);
	len = write(fd, text, len);
	sleep(2);

	memset(text, 0, 255);
	len = read(fd, text, 255);
	blobmsg_add_string(buf, "Received:", text);
	close(fd);

	return 0;
}

int list_esp_devices(struct blob_buf *buf)
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);

	int connected_devices = -1;

	if (result != SP_OK) {
		blobmsg_add_string(buf, "Error", "Can't read serial ports");
		return -1;
	}
	void* arr = blobmsg_open_array(buf, "devices");

	for (int i = 0; port_list[i] != NULL; i++) {
		struct sp_port *port = port_list[i];
		char *port_name = sp_get_port_name(port);
		enum sp_transport transport = sp_get_port_transport(port);
		if (transport == SP_TRANSPORT_USB) {
			int usb_vid, usb_pid;
			sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
			if(usb_vid == 0x10C4 && usb_pid == 0xEA60)
			{
				int usb_bus, usb_address;
				sp_get_port_usb_bus_address(port, &usb_bus, &usb_address);

				char name[100];
				sprintf(name, "ESP Device (%d, %d)", usb_bus, usb_address);

				void *tbl = blobmsg_open_table(buf, name);
				blobmsg_add_string(buf, "port", port_name);
				blobmsg_add_u32(buf, "vid", usb_vid);
				blobmsg_add_u32(buf, "pid", usb_pid);
				blobmsg_close_table(buf, tbl);
				connected_devices++;
			}
		}		
	}
	blobmsg_close_array(buf, arr);
	sp_free_port_list(port_list);
	return connected_devices;
}