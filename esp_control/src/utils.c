#include <fcntl.h>           
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include "libserialport.h"
#include "ubus_utils.h"
#include "utils.h"


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
		//blobmsg_add_string(buf, "Error", "Can't open serial port");	
		return -1;
	}
	int len = send_serial_message(fd, message);
	len = read_serial_message(fd, message);
	blobmsg_add_json_from_string(buf, message);
	close(fd);
	return 0;
}

int list_esp_devices(struct device *devices)
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);

	int count = 0;

	if (result != SP_OK) {
		return -1;
	}

	for (int i = 0; port_list[i] != NULL; i++) {
		struct sp_port *port = port_list[i];
		char *port_name = sp_get_port_name(port);
		enum sp_transport transport = sp_get_port_transport(port);
		int usb_vid, usb_pid;
		if (transport != SP_TRANSPORT_USB) {
			continue; 
		}
		sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);

		if(usb_vid == CP210x_VID && usb_pid == CP210x_PID && count < DEVICE_CAP){
	
			int usb_bus, usb_address;
			sp_get_port_usb_bus_address(port, &usb_bus, &usb_address);

			char vid[30];
			char pid[30];

			sprintf(vid, "%04X", usb_vid);
			sprintf(pid, "%04X", usb_pid);
			strncpy(devices[count].port, port_name, sizeof(devices->port));
			strncpy(devices[count].vid, vid, sizeof(devices->vid));
			strncpy(devices[count].pid, pid, sizeof(devices->pid));
			count ++;
		}
	}
	sp_free_port_list(port_list);
	return count;
}
int form_devices_blob_buf(struct device *devices, int count, struct blob_buf *buf)
{
	void* arr = NULL;
	arr = blobmsg_open_array(buf, "devices");
	for(int i = 0; i < count; i ++){
		void *tbl = blobmsg_open_table(buf, devices[i].port);
		blobmsg_add_string(buf, "port", devices[i].port);
		blobmsg_add_string(buf, "vid", devices[i].vid);
		blobmsg_add_string(buf, "pid", devices[i].pid);
		blobmsg_close_table(buf, tbl);
	}
	blobmsg_close_array(buf, arr);
	return 0;
}