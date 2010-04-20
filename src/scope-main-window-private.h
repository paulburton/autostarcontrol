#ifndef __SCOPE_MAIN_WINDOW_PRIVATE_H__
#define __SCOPE_MAIN_WINDOW_PRIVATE_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>

#define SCOPE_MAIN_WINDOW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), SCOPE_TYPE_MAIN_WINDOW, ScopeMainWindowPrivate))

struct _ScopeMainWindowPrivate
{
	GtkWidget *window;
	GtkWidget *webview;

	GIOChannel *serial;
	int iowatch;

	gchar *buf_in;

	gboolean mbed_ready;
};

enum
{
	PROP_0
};

static void cb_destroy (GtkWidget *widget, gpointer data);

static gboolean cb_prompt (WebKitWebView *web_view,
                           WebKitWebFrame *frame,
                           gchar *message,
                           gchar *def,
                           gchar **text,
                           gpointer user_data);

gchar *read_serial_in (ScopeMainWindow *self);
gboolean cb_serial_in (GIOChannel *ioch, GIOCondition cond, gpointer data);

#endif // __SCOPE_MAIN_WINDOW_PRIVATE_H__

