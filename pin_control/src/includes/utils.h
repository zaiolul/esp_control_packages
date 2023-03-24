#ifndef UTILS_H
#define UTILS_H

/*sends a message to ESP through serial to change a given pin state*/
int send_message(char* port, char* action, int pin, struct blob_buf *buf);

/*finds and appends all esp device info to a blob buffer*/
int list_esp_devices(struct blob_buf *buf);
#endif