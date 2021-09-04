#include <gtk/gtk.h>

#include <ug-plot.h>
#include <ug-indicator.h>
#include <ug-arrow.h>
#include <ug-plc.h>
#include <u_cat.h>
#include <math.h>

u_ema* ma;
u_pid_d* Pid;
u_timer_double T2={0};
u16 sets=1;
double temp= 0.;
UgArrow *arrow;
double setting=36.6;
double machine(double x, double job)
{
    return (job*0.01)+x-0.0001;
}
double heater_job;


gboolean to_Func(UgPlot *self){


    heater_job=proc_pid_d(Pid, temp, setting);

    temp = machine(temp,heater_job);
    // printf("temperature= %f\n",temp);
    u_ema_fwd(ma,temp);


    ug_arrow_set_value(arrow, temp/100.);
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
    ug_plot_set_scale_Y(pl,0,2.);

//==================================================
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button = gtk_button_new_with_label("EXIT");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
    ug_plot_axis_enable(pl,0);

    g_timeout_add(80,(GSourceFunc)to_Func,pl);

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
void tst_add1(GtkWidget*btn, UgArrow *self)
{
    // ug_arrow_set_value(self, cq);
    setting+=5.;
}

void
tst_sub(GtkWidget*btn, UgArrow *self)
{
    // ug_arrow_set_value(self, cq);
    setting-=5.;
}


void gui_test2()
{

    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget *button = gtk_button_new_with_label("EXIT");
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    
    GtkWidget *label = gtk_label_new("Hello,World!");

    
    


    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    

    GtkWidget *button_plus = gtk_button_new_with_label("+");
    GtkWidget *button_minus = gtk_button_new_with_label("-");
    UgIndicator *ind=ug_indicator_new();
    // UgArrow *
    arrow=ug_arrow_new();
    UgPlc* plc=ug_plc_new();

    g_signal_connect (button_plus, "clicked", G_CALLBACK (tst_add), ind);
    g_signal_connect (button_plus, "clicked", G_CALLBACK (tst_add1), arrow);
    g_signal_connect (button_minus, "clicked", G_CALLBACK (tst_sub), arrow);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(label));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button_plus));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button_minus));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    //  gtk_widget_set_size_request (GTK_WIDGET(ind), 90, 40);
    // gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(ind));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(arrow));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(plc));
    gtk_widget_show_all(GTK_WIDGET(window));


}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    u_msg("Hello, world!");
    Pid=new_pid_d( 3.8, 5. ,0.5,0.5);

    gui_test();
    gui_test2();
    gtk_main();
}




