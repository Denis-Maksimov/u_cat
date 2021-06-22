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
    u_msg("Hello, world!");
    u_msg("Hello, world!");
    u_msg("Hello, world!");
//==================================================
    u_ema* ma=u_ema_new();
    u_ema_set_N(ma, 30);
    u_ema* ma30=u_ema_new();
    u_ema_set_N(ma30, 60);
    float ppir=0.;
    u_vector_float* vf=u_vector_float_new();
    
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t i = 0; i < 20; i++)
        {
            ppir=g_random_double_range(-1.,1.);
            u_vector_float_push_back(vf,ppir);
            u_ema_push( ma, ppir);
            u_ema_push( ma30, ppir);

        }
        for (size_t i = 0; i < 10; i++)
        {
            ppir=g_random_double_range(-1.,1.);
            u_vector_float_push_back(vf,ppir);
            u_ema_push( ma, ppir);
            u_ema_push( ma30, ppir);
        }
    }

    u_ivr* ivr=u_new_IVR(ma30->ema);

//==================================================
    UgPlot *pl=ug_plot_new();
    ug_plot_push_vector (pl,ma->ema);
    ug_plot_push_vector (pl,ma30->ema);
    // ug_plot_push_vector (pl,vf);
    ug_plot_push_vector (pl,ivr->IVR);
    ug_plot_push_vector (pl,ivr->F);
    ug_plot_push_vector (pl,ivr->source);

//==================================================
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button = gtk_button_new_with_label("EXIT");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
    ug_plot_axis_enable(pl,1);
    ug_plot_set_X_param(pl,1,-50.,1.);

     u_vector_float* v = ug_plot_get_vector (pl, 1);
    g_message("%d",v->n_elem);
    g_timeout_add(10,(GSourceFunc)to_Func,pl);
    // g_timeout_add_seconds(1,(GSourceFunc)to_Func,pl);
    // do_sidebar (0);
    gtk_widget_set_size_request (GTK_WIDGET(pl), 900, 400);
     gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(pl));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    gtk_widget_show_all(GTK_WIDGET(window));



   

    gtk_main();
}









