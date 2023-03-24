#ifndef UTILS_H
#define UTILS_H

/*sends a message to ESP through serial to change a given pin state*/
int send_message(char* port, char* action, int pin, struct blob_buf *buf);

/*finds and appends all esp device info to a blob buffer*/
int list_esp_devices(struct blob_buf *buf);
/*opens serial port for reading and sending*/
int open_port(char* port);
/*reads serial port to MESSAGE after a short delay*/
int read_serial_message(int fd, char* message);
/*sends MESSAGE to serial port*/
int send_serial_message(int fd, char* message);
/*sends data to an ESP device on given port*/
int send_to_esp(char* port, char* message, struct blob_buf *buf);
#endif