#ifndef UBUS_UTILS_H
#define UBUS_UTILS_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>

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
#endif