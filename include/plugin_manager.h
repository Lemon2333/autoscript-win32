#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "plugin.h"

void plugin_manager_init(void);
int plugin_manager_register(const char* name, PluginFunction func);
PluginFunction plugin_manager_find(const char* name);

#endif