/*
 * event.c
 *
 * Copyright (C) 2012, 2013 James Booth <boothj5@gmail.com>
 *
 * This file is part of Profanity.
 *
 * Profanity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Profanity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Profanity.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <glib.h>

#include "event.h"
#include "ui/ui.h"

GHashTable *listeners;

void
event_init(void)
{
    listeners = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)g_slist_free);
}

void
event_listen(char *name, EVENT_FUNC func)
{
    gboolean new;
    GSList *event_listeners = g_hash_table_lookup(listeners, name);
    if (event_listeners == NULL) {
        new = TRUE;
    }
    event_listeners = g_slist_append(event_listeners, func);
    if (new == TRUE) {
        g_hash_table_insert(listeners, name, event_listeners);
    }
}

void
event_trigger(const char * const name, void *arg, ...)
{
    va_list arg_list;
    va_start(arg_list, arg);
    GSList *event_listener = g_hash_table_lookup(listeners, name);
    while (event_listener != NULL) {
        EVENT_FUNC callback = event_listener->data;
        void *arg0 = arg;
        void *arg1 = va_arg(arg_list, void *);
        void *arg2 = va_arg(arg_list, void *);
        callback(arg0, arg1, arg2);
        event_listener = g_slist_next(event_listener);
    }
    va_end(arg_list);
}
