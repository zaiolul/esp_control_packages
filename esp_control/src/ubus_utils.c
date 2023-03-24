#include <stdio.h>
#include <string.h>
#include "ubus_utils.h"
#include "utils.h"

int on(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[_CONTROL_MAX];
	struct blob_buf b = {};
	blob_buf_init(&b, 0);
	
	blobmsg_parse(control_policy, _CONTROL_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[CONTROL_PORT] || !tb[CONTROL_PIN])
		return UBUS_STATUS_INVALID_ARGUMENT;

	char* port = blobmsg_get_string(tb[CONTROL_PORT]);
	int pin = blobmsg_get_u32(tb[CONTROL_PIN]);

	char message[255];
	sprintf(message, "{\"action\": \"%s\", \"pin\": %d}", "on", pin);
	
	int ret = send_to_esp(port, message, &b);
	if(ret < 0){
		return UBUS_STATUS_INVALID_ARGUMENT;
	}
	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);
}

int off(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
    struct blob_attr *tb[_CONTROL_MAX];
	struct blob_buf b = {};
	blob_buf_init(&b, 0);
	
	blobmsg_parse(control_policy, _CONTROL_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[CONTROL_PORT] || !tb[CONTROL_PIN])
		return UBUS_STATUS_INVALID_ARGUMENT;

	char* port = blobmsg_get_string(tb[CONTROL_PORT]);
	int pin = blobmsg_get_u32(tb[CONTROL_PIN]);
	char message[255];
	sprintf(message, "{\"action\": \"%s\", \"pin\": %d}", "off", pin);

	int ret = send_to_esp(port, message, &b);
	if(ret < 0){
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);
}

int devices(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
    struct blob_buf b = {};
	blob_buf_init(&b, 0);

	int ret = list_esp_devices(&b);
	if(ret < 0){
		//blobmsg_add_string(&b, "Error", "No devices found");
		return UBUS_STATUS_NOT_FOUND;
	}

	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);

	return 0;
}
