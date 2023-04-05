#include <stdio.h>
#include <string.h>
#include "ubus_utils.h"
#include "utils.h"

enum {
    CONTROL_PORT,
    CONTROL_PIN,
    _CONTROL_MAX
};
/*ubus method arguments*/
static struct blobmsg_policy control_policy[] = {
    [CONTROL_PORT] = {.name = "port", .type = BLOBMSG_TYPE_STRING},
    [CONTROL_PIN] = {.name = "pin", .type = BLOBMSG_TYPE_INT32}
};

/*register ubus methods*/
static struct ubus_method control_methods []= {
    UBUS_METHOD_NOARG("devices", devices),
    UBUS_METHOD("on", on, control_policy),
    UBUS_METHOD("off", off, control_policy),
    
};

/*ubus object type*/
static struct ubus_object_type control_object_type = UBUS_OBJECT_TYPE("esp_control", control_methods);

/*create ubus object*/
static struct ubus_object control_object = {
    .name = "esp_control",
    .type = &control_object_type,
    .methods = control_methods,
    .n_methods = ARRAY_SIZE(control_methods),
};

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
	sprintf(message, "{\"action\": \"on\", \"pin\": %d}", pin);
	
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
	sprintf(message, "{\"action\": \"off\", \"pin\": %d}", pin);

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

	struct device devices[DEVICE_CAP];

	int ret = list_esp_devices(devices);
	
	if(ret <= 0){
		return UBUS_STATUS_NO_DATA;
	}

	form_devices_blob_buf(devices, ret, &b);
	ubus_send_reply(ctx, req, b.head);
	blob_buf_free(&b);
	fflush(stdout);

	return 0;
}




