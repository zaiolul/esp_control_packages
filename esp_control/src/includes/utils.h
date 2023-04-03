#ifndef UTILS_H
#define UTILS_H

#define CP210x_VID 0x10C4
#define CP210x_PID 0xEA60
#define DEVICE_CAP 16

struct device{
    char port[30];
    char pid[30];
    char vid[30];
};

/*sends a message to ESP through serial to change a given pin state*/
int send_message(char* port, char* action, int pin, struct blob_buf *buf);

/*finds all ESP devices*/
int list_esp_devices(struct device* devices);
/*opens serial port for reading and sending*/
int open_port(char* port);
/*reads serial port to MESSAGE after a short delay*/
int read_serial_message(int fd, char* message);
/*sends MESSAGE to serial port*/
int send_serial_message(int fd, char* message);
/*sends data to an ESP device on given port*/
int send_to_esp(char* port, char* message, struct blob_buf *buf);
/**/
int form_devices_blob_buf(struct device *devices, int count, struct blob_buf *buf);
#endif