#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "scope-main-window.h"

int main (int argc, char *argv[])
{
	int serial;
	//struct termios tio;
	GIOChannel *schan;
	gtk_init (&argc, &argv);
	
	// WebKitGtk requires g_thread
	if (!g_thread_supported ())
		g_thread_init (NULL);

	if (argc > 1)
		serial = open (argv[1], O_RDWR|O_NOCTTY|O_NONBLOCK);
	else
		serial = open ("/dev/ttyACM0", O_RDWR|O_NOCTTY|O_NONBLOCK);

	if (serial < 0)
	{
		g_print ("Unable to open serial device\n");
		return EXIT_FAILURE;
	}
	
	/*tcgetattr (serial, &tio);
	tio.c_cflag |= CREAD | CLOCAL | CS8;
	tio.c_iflag = IGNPAR | IGNBRK;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	tio.c_cc[VTIME] = 0;
	tio.c_cc[VMIN]  = 1;
	tcsetattr (serial, TCSANOW, &tio);
	tcflush (serial, TCIFLUSH);
	tcflush (serial, TCOFLUSH);*/
	
	schan = g_io_channel_unix_new (serial);
	
	scope_main_window_new (schan);
	gtk_main ();

	close (serial);

	return EXIT_SUCCESS;
}

