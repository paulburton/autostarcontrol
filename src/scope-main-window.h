#ifndef __SCOPE_MAIN_WINDOW_H__
#define __SCOPE_MAIN_WINDOW_H__

#include <glib-object.h>

#define SCOPE_TYPE_MAIN_WINDOW                  (scope_main_window_get_type ())
#define SCOPE_MAIN_WINDOW(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCOPE_TYPE_MAIN_WINDOW, ScopeMainWindow))
#define SCOPE_IS_MAIN_WINDOW(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCOPE_TYPE_MAIN_WINDOW))
#define SCOPE_MAIN_WINDOW_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), SCOPE_TYPE_MAIN_WINDOW, ScopeMainWindowClass))
#define SCOPE_IS_MAIN_WINDOW_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), SCOPE_TYPE_MAIN_WINDOW))
#define SCOPE_MAIN_WINDOW_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), SCOPE_TYPE_MAIN_WINDOW, ScopeMainWindowClass))

typedef struct _ScopeMainWindow               ScopeMainWindow;
typedef struct _ScopeMainWindowClass          ScopeMainWindowClass;
typedef struct _ScopeMainWindowPrivate        ScopeMainWindowPrivate;

struct _ScopeMainWindow
{
	GObject parent_instance;
	
	// instance members
	
	// private
	ScopeMainWindowPrivate *priv;
};

struct _ScopeMainWindowClass
{
	GObjectClass parent_class;
	
	// class members
};

GType scope_main_window_get_type ();

ScopeMainWindow *scope_main_window_new ();

#endif // __SCOPE_MAIN_WINDOW_H__

