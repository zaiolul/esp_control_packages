#ifndef UBUS_UTILS_H
#define UBUS_UTILS_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>

enum {
    CONTROL_PORT,
    CONTROL_PIN,
    _CONTROL_MAX
};

/*ubus method to list all connected ESP devices*/
int devices(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

/*ubus method to turn on a pin on an ESP device*/
int on(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

/*ubus method to turn off a pin on an ESP device*/
int off(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg);

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
static struct ubus_object_type control_object_type = UBUS_OBJECT_TYPE("pin_control", control_methods);

/*create ubus object*/
static struct ubus_object control_object = {
    .name = "pin_control",
    .type = &control_object_type,
    .methods = control_methods,
    .n_methods = ARRAY_SIZE(control_methods),
};


#endif