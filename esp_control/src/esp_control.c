#include <stdio.h>
#include <stdlib.h>
#include "ubus_utils.h"

int main()
{
    struct ubus_context *ctx;

	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &control_object);
	uloop_run();

	ubus_free(ctx);
	uloop_done();
    return 0; 
}
