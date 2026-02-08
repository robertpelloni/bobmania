#include "global.h"
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
=======
#include "LoadingWindow_GtkModule.h"
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
#include "RageUtil.h"
#include "RageSurface.h"
#include "RageSurfaceUtils.h"
#include "RageSurface_Load.h"
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
#include "LoadingWindow_Gtk.h"

#include <cstdint>
=======
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp

#include <gtk/gtk.h>

static GtkWidget *label;
static GtkWidget *window;
static GtkWidget *splash;
static GtkWidget *progressBar;

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
LoadingWindow_Gtk::LoadingWindow_Gtk()
{
}

RString LoadingWindow_Gtk::Init()
=======
extern "C" const char *Init( int *argc, char ***argv )
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	// Need to use external library to load this image. Native loader seems broken :/
	const gchar *splash_image_path = "Data/splash.png";
	GtkWidget *vbox;

	gtk_disable_setlocale();
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
	if( !gtk_init_check(&g_argc, &g_argv) )
		return "Couldn't initialize gtk (cannot open display)";

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS );
	gtk_widget_set_size_request(window,-1,-1);
=======
	if( !gtk_init_check(argc,argv) )
		return "Couldn't initialize gtk (cannot open display)";

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER );
	gtk_widget_set_size_request(window,468,-1);
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
=======
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS );
	gtk_widget_set_size_request(window,-1,-1);
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
	gtk_window_set_deletable( GTK_WINDOW(window), FALSE );
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_role( GTK_WINDOW(window), "sm-startup" );
	//gtk_window_set_icon( GTK_WINDOW(window), );
	gtk_widget_realize(window);

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
	splash = gtk_image_new_from_pixbuf(
		gdk_pixbuf_new_from_file (splash_image_path, nullptr)
	);
=======
	splash = gtk_image_new_from_file(splash_image_path);
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
=======
	splash = gtk_image_new_from_pixbuf(
		gdk_pixbuf_new_from_file (splash_image_path, nullptr)
	);
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp

	label = gtk_label_new(nullptr);
	gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_CENTER);
	gtk_label_set_ellipsize(GTK_LABEL(label),PANGO_ELLIPSIZE_END);
	gtk_label_set_line_wrap(GTK_LABEL(label),FALSE);

	progressBar = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR(progressBar), 0.0 );

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
=======
	vbox = gtk_vbox_new(FALSE,0);
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
=======
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
>>>>>>> origin/unified-ui-features-13937230807013224518:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_box_pack_start(GTK_BOX(vbox),splash,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox),progressBar,FALSE,FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox),label,TRUE,TRUE,0);

	gtk_widget_show_all(window);
	gtk_main_iteration_do(FALSE);
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
	return "";
}

LoadingWindow_Gtk::~LoadingWindow_Gtk()
=======
	return nullptr;
}

extern "C" void Shutdown()
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	gtk_widget_hide(window);
	g_signal_emit_by_name (G_OBJECT (window), "destroy");
	while( gtk_events_pending() )
		gtk_main_iteration_do(FALSE);
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
void LoadingWindow_Gtk::SetText( RString s )
{
	gtk_label_set_text(GTK_LABEL(label), s.c_str());
=======
extern "C" void SetText( const char *s )
{
	gtk_label_set_text(GTK_LABEL(label), s);
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
	gtk_widget_show(label);
	gtk_main_iteration_do(FALSE);
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
static void DeletePixels( guchar *pixels, gpointer data )
=======
void DeletePixels( guchar *pixels, gpointer data )
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	delete[] (uint8_t *)pixels;
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
static GdkPixbuf *MakePixbuf( const RageSurface *pSrc )
=======
GdkPixbuf *MakePixbuf( const RageSurface *pSrc )
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	RageSurface *pSurface = CreateSurface( pSrc->w, pSrc->h, 32,
		0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
	RageSurfaceUtils::Blit( pSrc, pSurface , -1, -1 );

	GdkPixbuf *pBuf = gdk_pixbuf_new_from_data( pSurface->pixels, GDK_COLORSPACE_RGB,
		true, 8, pSurface->w, pSurface->h , pSurface->pitch, DeletePixels, nullptr);

	if( pBuf != nullptr )
		pSurface->pixels_owned = false;

	delete pSurface;
	return pBuf;
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
void LoadingWindow_Gtk::SetIcon( const RageSurface *pIcon )
{
	GdkPixbuf *pBuf = MakePixbuf( pIcon );
=======
extern "C" void SetIcon( const RageSurface *pSrcImg )
{
	GdkPixbuf *pBuf = MakePixbuf( pSrcImg );
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
	if( pBuf != nullptr )
	{
		gtk_window_set_icon( GTK_WINDOW(window), pBuf );
		g_object_unref(pBuf);
	}
	gtk_main_iteration_do(FALSE);
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
void LoadingWindow_Gtk::SetSplash( const RageSurface *pSplash )
=======
extern "C" void SetSplash( const RageSurface *pSplash )
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	GdkPixbuf *pBuf = MakePixbuf( pSplash );
	if( pBuf != nullptr )
	{
		gtk_image_set_from_pixbuf(GTK_IMAGE(splash), pBuf);
		g_object_unref(pBuf);
	}
	gtk_main_iteration_do(FALSE);
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
static void UpdateProgress( int progress, int totalWork )
=======
extern "C" void SetProgress( int progress, int totalWork )
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
{
	gdouble fraction = ( totalWork > 0 ? progress / (gdouble)totalWork : 0 );
	if( fraction > 1.0 ) fraction = 1.0;
	if( fraction < 0.0 ) fraction = 0.0;
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR(progressBar), fraction );
	gtk_main_iteration_do(FALSE);
}

<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
void LoadingWindow_Gtk::SetProgress( const int progress )
{
	LoadingWindow::SetProgress( progress );
	UpdateProgress( m_progress, m_totalWork );
}

void LoadingWindow_Gtk::SetTotalWork( const int totalWork )
{
	LoadingWindow::SetTotalWork( totalWork );
	UpdateProgress( m_progress, m_totalWork );

}

void LoadingWindow_Gtk::SetIndeterminate( bool indeterminate )
{
	LoadingWindow::SetIndeterminate( indeterminate );

=======
extern "C" void SetIndeterminate( bool indeterminate )
{
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressBar));
	gtk_main_iteration_do(FALSE);
}

/*
 * (c) 2003-2004 Glenn Maynard, Sean Burke
 * All rights reserved.
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
 *
=======
 * 
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
<<<<<<< HEAD:itgmania/src/arch/LoadingWindow/LoadingWindow_Gtk.cpp
 *
=======
 * 
>>>>>>> origin/c++11:src/arch/LoadingWindow/LoadingWindow_GtkModule.cpp
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
