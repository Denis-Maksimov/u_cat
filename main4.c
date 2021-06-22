#include <gtk/gtk.h>

#include "ug-plot.h"
#include <u_application/u_ema.h>
#include <math.h>
#include <u_cat.h>


gboolean to_Func(UgPlot *self){
    u_vector_float* v = ug_plot_get_vector (self, 4);
    g_message("%d",v->n_elem);
    u_vector_fifo_forward(v, 0.1*sinf(0.00008*clock()));
    gtk_widget_queue_draw(GTK_WIDGET(self));
    return TRUE;
}



int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

//==================================================
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button = gtk_button_new_with_label("EXIT");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
 
    g_timeout_add(10,(GSourceFunc)to_Func,pl);
    // g_timeout_add_seconds(1,(GSourceFunc)to_Func,pl);
    // do_sidebar (0);
    gtk_widget_set_size_request (GTK_WIDGET(pl), 900, 400);
     gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(pl));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    gtk_widget_show_all(GTK_WIDGET(window));



   

    gtk_main();
}









