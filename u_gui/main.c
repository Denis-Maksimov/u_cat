#include <gtk/gtk.h>

#include <ug-plot.h>
#include <ug-indicator.h>
#include <u_cat.h>
#include <math.h>

u_ema* ma;

gboolean to_Func(UgPlot *self){
    // u_vector_float* v = ug_plot_get_vector (self, 0);
    // g_message("%d",v->n_elem);
    // u_vector_fifo_forward(v, 0.08*sinf(0.0008*clock()));
    static float t=0.0;
    u_ema_fwd(ma, 30.*sinf(1.*t)*cosf(1.57+3.*t));
    t+=0.1;
    gtk_widget_queue_draw(GTK_WIDGET(self));
    return TRUE;
}



void gui_test()
{
    //Новая скользящая средняя
    ma=u_ema_new();
    u_ema_set_N(ma, 30);
    float ppir=0.;
    for (size_t i = 0; i < 40; i++)
    {
        ppir=g_random_double_range(-1.,1.);
        // u_vector_float_push_back(vf,ppir);
        u_ema_push( ma, ppir);
    }
    
//==================================================
    UgPlot *pl=ug_plot_new();
    ug_plot_push_vector (pl,ma->ema);
    ug_plot_set_scale_Y(pl,0,10.);

//==================================================
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button = gtk_button_new_with_label("EXIT");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
    ug_plot_axis_enable(pl,0);

    g_timeout_add(30,(GSourceFunc)to_Func,pl);

    gtk_widget_set_size_request (GTK_WIDGET(pl), 900, 400);
     gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(pl));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    gtk_widget_show_all(GTK_WIDGET(window));


}

long cnt=0;

void tst_add(GtkWidget*btn, UgIndicator *self)
{
    cnt++;
    ug_indicator_set_value(self, cnt);
    

}

void gui_test2()
{

    UgIndicator *ind=ug_indicator_new();
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget *button = gtk_button_new_with_label("EXIT");
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
    GtkWidget *label = gtk_label_new("Hello,World!");

    GtkWidget *button_plus = gtk_button_new_with_label("+");
    g_signal_connect (button_plus, "clicked", G_CALLBACK (tst_add), ind);
    gtk_label_set_text(GTK_LABEL(label),"beam");

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    


    
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(label));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button_plus));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    //  gtk_widget_set_size_request (GTK_WIDGET(ind), 90, 40);
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(ind));
    gtk_widget_show_all(GTK_WIDGET(window));


}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    u_msg("Hello, world!");
    gui_test();
    gui_test2();
    gtk_main();
}




