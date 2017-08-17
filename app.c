#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef BUILD_APPLICATIONS
/* 
   Copyright (C) 2009 - 2010
   
   Artem Makhutov <artem@makhutov.org>
   http://www.makhutov.org
   
   Dmitry Vagin <dmitry2004@yandex.ru>

   bg <bg_one@mail.ru>
*/

#include <stdbool.h>
#include <asterisk.h>
#include <asterisk/app.h>    /* AST_DECLARE_APP_ARGS() ... */
#include <asterisk/pbx.h>    /* pbx_builtin_setvar_helper() */
#include <asterisk/module.h>    /* ast_register_application2() ast_unregister_application() */
#include <asterisk/ast_version.h>    /* ASTERISK_VERSION_NUM */

#include "app.h"        /* app_register() app_unregister() */
#include "chan_dongle.h"    /* struct pvt */
#include "helpers.h"        /* send_sms() ITEMS_OF() */

struct ast_channel;

static int app_status_exec(struct ast_channel *channel, const char *data) {
	struct pvt *pvt;
	char *parse;
	int stat;
	char status[2];
	int exists = 0;

	AST_DECLARE_APP_ARGS (args,	AST_APP_ARG(resource);
								AST_APP_ARG(variable);
	);

	if (ast_strlen_zero(data)) {
		return -1;
	}

	parse = ast_strdupa (data);

	AST_STANDARD_APP_ARGS (args, parse);

	if (ast_strlen_zero(args.resource) || ast_strlen_zero(args.variable)) {
		return -1;
	}

	if (args.argc != 2) {
		ast_log(LOG_ERROR, "\nInvalid options number.\n" 
				"Two input parameters are required: the device identifier and the variable for the result\n");
		return -1;
	}

	pvt = find_device_by_resource(args.resource, 0, NULL, &exists);
	if (pvt) {
		/* ready for outgoing call */
		ast_mutex_unlock (&pvt->lock);
		stat = 2;
	} else {
		stat = exists ? 3 : 1;
	}

	snprintf(status, sizeof(status), "%d", stat);
	pbx_builtin_setvar_helper(channel, args.variable, status);

	return 0;
}

static const char * pvt_state_text(const struct pvt * pvt)
{
	const char * state = NULL;
// length is "AAAAAAAAAA"
	if(pvt->current_state == DEV_STATE_STOPPED && pvt->desired_state == DEV_STATE_STOPPED)
		state = "Stopped";
	else if(!pvt->connected)
		state = "Not connected";
	else if(!pvt->initialized)
		state = "Not initialized";
	else if(!pvt->gsm_registered)
		state = "GSM not registered";
	else if(pvt->ring || PVT_STATE(pvt, chan_count[CALL_STATE_INCOMING]))
		state = "Ring";
	else if(pvt->cwaiting || PVT_STATE(pvt, chan_count[CALL_STATE_WAITING]))
		state = "Waiting";
	else if(pvt->dialing ||
			(PVT_STATE(pvt, chan_count[CALL_STATE_INIT])
			 +
			 PVT_STATE(pvt, chan_count[CALL_STATE_DIALING])
			 +
			 PVT_STATE(pvt, chan_count[CALL_STATE_ALERTING])) > 0)
		state = "Dialing";
	else if(PVT_STATE(pvt, chan_count[CALL_STATE_ACTIVE]) > 0)
//			state = "Active";
		state = pvt_call_dir(pvt);
	else if(PVT_STATE(pvt, chan_count[CALL_STATE_ONHOLD]) > 0)
		state = "Held";
	else if(pvt->outgoing_sms || pvt->incoming_sms)
		state = "SMS";
	else
		state = "Free";
	return state;
}

static int app_gsm_status_exec(struct ast_channel *channel, const char *data) {
	struct pvt *pvt;
	char *parse;
	int stat;
	const char *status;
	int exists = 0;

	AST_DECLARE_APP_ARGS (args,	AST_APP_ARG(resource);
								AST_APP_ARG(variable);
	);

	if (ast_strlen_zero(data)) {
		return -1;
	}
	parse = ast_strdupa (data);

	AST_STANDARD_APP_ARGS (args, parse);

	if (ast_strlen_zero(args.resource) || ast_strlen_zero(args.variable)) {
		ast_log(LOG_ERROR, "\nInvalid options number.\n"
				"Two input parameters are required: the device identifier and the variable for the result\n");
		return -1;
	}
	
	pvt = find_device_by_resource(args.resource, 0, NULL, &exists);
	if (pvt) {
		/* ready for outgoing call */
		ast_mutex_unlock (&pvt->lock);
		status = pvt_state_text(pvt);
	} else {
		status = "Not found";
	}

	pbx_builtin_setvar_helper(channel, args.variable, status);
	return 0;
}

static int app_get_name_device_exec(struct ast_channel *channel, const char *data) {
	struct pvt *pvt;
	char *parse;
	//int stat;
	const char *name;
	int exists = 0;

	AST_DECLARE_APP_ARGS (args,	AST_APP_ARG(resource);
			                    AST_APP_ARG(variable);
	);

	if (ast_strlen_zero(data)) {
		return -1;
	}
	parse = ast_strdupa (data);

	AST_STANDARD_APP_ARGS (args, parse);

	if (ast_strlen_zero(args.resource) || ast_strlen_zero(args.variable)) {
		ast_log(LOG_ERROR, "\nInvalid options number.\n"
				"Two input parameters are required: the device identifier and the variable for the result\n\n");
		return -1;
	}

	pvt = find_device_by_resource(args.resource, 0, NULL, &exists);
	if (pvt) {
		/* ready for outgoing call */
		ast_mutex_unlock (&pvt->lock);
		name = PVT_ID(pvt);
	} else {
		name = "Not found";
	}

	pbx_builtin_setvar_helper(channel, args.variable, name);
	return 0;
}

static int app_get_number_device_exec(struct ast_channel *channel, const char *data) {
	struct pvt *pvt;
	char *parse;
	//int stat;
	const char *number;
	int exists = 0;

	AST_DECLARE_APP_ARGS (args,	AST_APP_ARG(resource);
								AST_APP_ARG(variable);
	);

	if (ast_strlen_zero(data)) {
		return -1;
	}
	parse = ast_strdupa (data);

	AST_STANDARD_APP_ARGS (args, parse);

	if (ast_strlen_zero(args.resource) || ast_strlen_zero(args.variable)) {
		ast_log(LOG_ERROR, "\nInvalid options number.\n"
				"Two input parameters are required: the device identifier and the variable for the result\n");
		return -1;
	}

	pvt = find_device_by_resource(args.resource, 0, NULL, &exists);
	if (pvt) {
		/* ready for outgoing call */
		ast_mutex_unlock (&pvt->lock);
		number = pvt->subscriber_number;
	} else {
		number = "Not found";
	}

	pbx_builtin_setvar_helper(channel, args.variable, number);
	return 0;
}

static int app_send_sms_exec(attribute_unused struct ast_channel *channel, const char *data) {
	char *parse;
	const char *msg;
	int status;
	void *msgid;

	AST_DECLARE_APP_ARGS (args, AST_APP_ARG(device);
								AST_APP_ARG(number);
								AST_APP_ARG(message);
								AST_APP_ARG(validity);
								AST_APP_ARG(report);
	);

	if (ast_strlen_zero(data)) {
		return -1;
	}

	parse = ast_strdupa (data);

	AST_STANDARD_APP_ARGS (args, parse);

	if (args.argc != 5) {
		ast_log(LOG_ERROR, "Invalid options number.\n Five input parameters are required.\n");
		return -1;
	}

	if (ast_strlen_zero(args.device)) {
		ast_log(LOG_ERROR, "NULL device for message -- SMS will not be sent\n");
		return -1;
	}

	if (ast_strlen_zero(args.number)) {
		ast_log(LOG_ERROR, "NULL destination for message -- SMS will not be sent\n");
		return -1;
	}

	msg = send_sms(args.device, args.number, args.message, args.validity, args.report, &status, &msgid);
	if (!status)
		ast_log(LOG_ERROR, "[%s] %s with id %p\n", args.device, msg, msgid);
	return !status;
}


static const struct dongle_application {
	const char *name;

	int (*func)(struct ast_channel *channel, const char *data);

	const char *synopsis;
	const char *desc;
} dca[] =
		{
				{
					"DongleStatus",
						app_status_exec,
						"DongleStatus(Resource,Variable)",
						"DongleStatus(Resource,Variable)\n"
							"  Resource - Resource string as for Dial()\n"
							"  Variable - Variable to store status in will be 1-3.\n"
							"             In order, Disconnected, Connected & Free, Connected & Busy.\n"
				},
				{
					"DongleSendSMS",
						app_send_sms_exec,
						"DongleSendSMS(Device,Dest,Message,Validity,Report)",
						"DongleSendSMS(Device,Dest,Message,Validity,Report)\n"
							"  Device   - Id of device from dongle.conf\n"
							"  Dest     - destination\n"
							"  Message  - text of the message\n"
							"  Validity - Validity period in minutes\n"
							"  Report   - Boolean flag for report request\n"
				},
				{
					"DongleGSMStatus",
						app_gsm_status_exec,
						"DongleGSMStatus(Resource,Variable)",
						"DongleGSMStatus(Resource,Variable)\n"
							"  Resource - Resource string as for Dial()\n"
							"  Variable - Variable to store status GSM Modem.\n"
							"             Stopped, Not connected, Not initialized, GSM not registered, Not found.\n"
				},
				{
					"DongleGetNameDevice",
						app_get_name_device_exec,
						"DongleGetNameDevice(Resource,Variable)",
						"DongleGetNameDevice(Resource,Variable)\n"
							"  Resource - Resource string as for Dial()\n"
							"  Variable - Variable to store name device\n"
				},
				{
					"DongleGetNumberDevice",
						app_get_number_device_exec,
						"DongleGetNumberDevice(Resource,Variable)",
						"DongleGetNumberDevice(Resource,Variable)\n"
							"  Resource - Resource string as for Dial()\n"
							"  Variable - Variable to store number device\n"
				},
		};

#if ASTERISK_VERSION_NUM >= 10800

typedef int        (*app_func_t)(struct ast_channel *channel, const char *data);

#else
typedef int		(*app_func_t)(struct ast_channel* channel, void * data);
#endif

#/* */

EXPORT_DEF void app_register() {
	unsigned i;
	for (i = 0; i < ITEMS_OF(dca); i++) {
		ast_register_application2(dca[i].name, (app_func_t) (dca[i].func), dca[i].synopsis, dca[i].desc, self_module());
	}
}

#/* */


EXPORT_DEF void app_unregister() {
	int i;
	for (i = ITEMS_OF(dca) - 1; i >= 0; i--) {
		ast_unregister_application(dca[i].name);
	}
}

#endif /* BUILD_APPLICATIONS */
