// SPDX-License-Identifier: MPL-2.0
// Copyright (c) Yuxuan Shui <yshuiv7@gmail.com>

#include <inttypes.h>

#include <xsui-picom/api.h>
#include <xsui-picom/backend.h>

#include <uthash.h>

#include "compiler.h"
#include "log.h"
#include "utils/list.h"
#include "utils/misc.h"

struct backend_plugins {
	UT_hash_handle hh;
	const char *backend_name;
	struct list_node plugins;
} *backend_plugins;

struct backend_plugin {
	const char *backend_name;
	xsui_picom_backend_plugin_entrypoint entrypoint;
	void *user_data;
	struct list_node siblings;
};

static bool add_backend_plugin(const char *backend_name, uint64_t major, uint64_t minor,
                               xsui_picom_backend_plugin_entrypoint entrypoint, void *user_data) {
	if (major != XSUI_PICOM_BACKEND_MAJOR || minor > XSUI_PICOM_BACKEND_MINOR) {
		log_error("Cannot add plugin for backend %s, because the requested "
		          "version %" PRIu64 ".%" PRIu64 " is incompatible with the our "
		          "%lu.%lu",
		          backend_name, major, minor, XSUI_PICOM_BACKEND_MAJOR,
		          XSUI_PICOM_BACKEND_MINOR);
		return false;
	}

	auto plugin = ccalloc(1, struct backend_plugin);
	plugin->backend_name = backend_name;
	plugin->entrypoint = entrypoint;
	plugin->user_data = user_data;

	struct backend_plugins *plugins = NULL;
	HASH_FIND_STR(backend_plugins, backend_name, plugins);
	if (!plugins) {
		plugins = ccalloc(1, struct backend_plugins);
		plugins->backend_name = strdup(backend_name);
		list_init_head(&plugins->plugins);
		HASH_ADD_STR(backend_plugins, backend_name, plugins);
	}
	list_insert_after(&plugins->plugins, &plugin->siblings);
	return true;
}

void api_backend_plugins_invoke(const char *backend_name, struct backend_base *backend) {
	struct backend_plugins *plugins = NULL;
	HASH_FIND_STR(backend_plugins, backend_name, plugins);
	if (!plugins) {
		return;
	}

	list_foreach(struct backend_plugin, plugin, &plugins->plugins, siblings) {
		plugin->entrypoint(backend, plugin->user_data);
	}
}

static struct xsui_picom_api xsui_picom_api = {
    .add_backend_plugin = add_backend_plugin,
};

XSUI_PICOM_PUBLIC_API const struct xsui_picom_api *
xsui_picom_api_get_interfaces(uint64_t major, uint64_t minor, const char *context) {
	if (major != XSUI_PICOM_API_MAJOR || minor > XSUI_PICOM_API_MINOR) {
		log_error("Cannot provide API interfaces to %s, because the requested"
		          "version %" PRIu64 ".%" PRIu64 " is incompatible with our "
		          "%lu.%lu",
		          context, major, minor, XSUI_PICOM_API_MAJOR, XSUI_PICOM_API_MINOR);
		return NULL;
	}
	return &xsui_picom_api;
}
