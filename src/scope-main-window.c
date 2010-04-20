#include "scope-main-window.h"
#include "scope-main-window-private.h"

#include <stdlib.h>
#include <string.h>

#define BUF_IN_SIZE 256

G_DEFINE_TYPE (ScopeMainWindow, scope_main_window, G_TYPE_OBJECT);

static void scope_main_window_set_property (GObject      *object,
                                             guint         property_id,
                                             const GValue *value,
                                             GParamSpec   *pspec)
{
	//ScopeMainWindow *self = SCOPE_MAIN_WINDOW (object);
	
	switch (property_id)
	{
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void scope_main_window_get_property (GObject      *object,
                                             guint         property_id,
                                             GValue     *value,
                                             GParamSpec   *pspec)
{
	//ScopeMainWindow *self = SCOPE_MAIN_WINDOW (object);
	
	switch (property_id)
	{
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void scope_main_window_dispose (GObject *object)
{
	//ScopeMainWindow *self = SCOPE_MAIN_WINDOW (object);
	
	G_OBJECT_CLASS (scope_main_window_parent_class)->dispose (object);
}

static void scope_main_window_finalize (GObject *object)
{
	//ScopeMainWindow *self = SCOPE_MAIN_WINDOW (object);
	
	G_OBJECT_CLASS (scope_main_window_parent_class)->finalize (object);
}

static void scope_main_window_class_init (ScopeMainWindowClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	
	gobject_class->dispose = scope_main_window_dispose;
	gobject_class->finalize = scope_main_window_finalize;
	gobject_class->get_property = scope_main_window_get_property;
	gobject_class->set_property = scope_main_window_set_property;
	
	g_type_class_add_private (klass, sizeof (ScopeMainWindowPrivate));
}

static void scope_main_window_init (ScopeMainWindow *self)
{
	GtkWidget *scrolledwindow;
	WebKitWebSettings *settings = webkit_web_settings_new ();
	
	self->priv = SCOPE_MAIN_WINDOW_GET_PRIVATE (self);

	self->priv->buf_in = g_malloc0 (BUF_IN_SIZE);
	self->priv->mbed_ready = false;
	
	self->priv->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position (GTK_WINDOW (self->priv->window), GTK_WIN_POS_CENTER);
	
	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow),
	                                GTK_POLICY_NEVER, GTK_POLICY_NEVER);
	gtk_container_add (GTK_CONTAINER (self->priv->window),
	                   GTK_WIDGET (scrolledwindow));
	
	self->priv->webview = webkit_web_view_new ();
	gtk_widget_set_size_request (GTK_WIDGET (self->priv->webview), 640, 480);
	gtk_container_add (GTK_CONTAINER (scrolledwindow),
	                   GTK_WIDGET (self->priv->webview));
	
	g_object_set (G_OBJECT (settings), "enable-universal-access-from-file-uris", TRUE, NULL);
	webkit_web_view_set_settings (WEBKIT_WEB_VIEW (self->priv->webview), settings);
	
	g_signal_connect (G_OBJECT (self->priv->window), "destroy", G_CALLBACK (cb_destroy), self);
	
	g_signal_connect (G_OBJECT (self->priv->webview), "script-prompt", G_CALLBACK (cb_prompt), self);
	
	webkit_web_view_load_uri (WEBKIT_WEB_VIEW (self->priv->webview),
	                          "file://src/html/index.html");
	
	gtk_widget_show_all (self->priv->window);
}

ScopeMainWindow *scope_main_window_new (GIOChannel *serial)
{
	ScopeMainWindow *ret = g_object_new (SCOPE_TYPE_MAIN_WINDOW, NULL);
	
	ret->priv->serial = serial;
	ret->priv->iowatch = g_io_add_watch (serial, G_IO_IN, cb_serial_in, ret);
	
	return ret;
}

static void cb_destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

static gboolean cb_prompt (WebKitWebView *web_view,
                           WebKitWebFrame *frame,
                           gchar *message,
                           gchar *def,
                           gchar **text,
                           gpointer user_data)
{
	ScopeMainWindow *self = (ScopeMainWindow *)user_data;
	gchar buf[128];
	gsize written;
	
	*text = NULL;
	
	if (g_strcmp0 (message, "SendToMBED") == 0)
	{
		g_stpcpy (buf, def);
		g_strlcat (buf, "\r", 128);

		g_print (">> %s\n", buf);
		
		g_io_channel_write_chars (self->priv->serial,
		                          buf, -1,
		                          &written, NULL);
		g_io_channel_flush (self->priv->serial, NULL);
	}
	else if (g_strcmp0 (message, "PollMBED") == 0)
	{
		*text = read_serial_in (self);

		if (*text)
			g_print ("<< %s\n", *text);
	}
	
	return true;
}

gchar *read_serial_in (ScopeMainWindow *self)
{
	gsize read, linelen;
	gchar buf[128], str[129];
	gchar *line, *nline, *newstart;
	
	do
	{
		g_io_channel_read_chars (self->priv->serial, buf, 128, &read, NULL);
		
		if (read > 0)
		{
			g_memmove (str, buf, read);
			str[read] = 0;
			
			g_strlcat (self->priv->buf_in, str, BUF_IN_SIZE);
			
			while ((nline = g_strrstr (self->priv->buf_in, "\r\n")) != NULL)
			{
				linelen = (gsize)(nline - self->priv->buf_in);
				newstart = nline + 2;

				line = g_malloc (linelen + 1);
				g_memmove (line, self->priv->buf_in, linelen);
				line[linelen] = 0;
				
				g_memmove (self->priv->buf_in, newstart, BUF_IN_SIZE - (linelen + 2));

				return line;
			}
		}
	} while (read > 0);
	
	return NULL;
}

gboolean cb_serial_in (GIOChannel *ioch, GIOCondition cond, gpointer data)
{
	ScopeMainWindow *self = SCOPE_MAIN_WINDOW (data);
	gchar *line, *b64, *jscode;

	while ((line = read_serial_in (self)) != NULL)
	{
		if (g_strcmp0 (line, "READY") == 0)
		{
			g_print ("MBED Ready!\n");

			self->priv->mbed_ready = true;
			
			webkit_web_view_execute_script (
			    WEBKIT_WEB_VIEW (self->priv->webview),
			    "scope.ready();");
		}
		else if (self->priv->mbed_ready)
		{
			g_print ("<< %s\n", line);

			if (strlen (line) > 0)
				b64 = g_base64_encode ((guchar *)line, strlen (line));
			else
				b64 = g_strdup ("");
			
			jscode = g_strconcat ("scope.receive('",
			                      b64, "');", NULL);
			webkit_web_view_execute_script (
			    WEBKIT_WEB_VIEW (self->priv->webview),
			    jscode);
			g_free (b64);
			g_free (jscode);
		}
		else
			g_print ("%s\n", line);

		g_free (line);
	}
	
	return true;
}

