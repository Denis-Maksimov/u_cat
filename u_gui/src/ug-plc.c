#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glib-object.h>
/****************************
 * Уровень виджета
* ***************************/
#include <ug-plc.h>
// #include <snap7.h>
// #include <u_application/u_fuzzy.h>

struct _UgPlcPrivate
{
    GtkBox parent;
    // S7Object client;   
    GtkWidget *vbox;
    GtkWidget *btn_connect;
    GtkWidget *input_ip;
    GtkEntryBuffer *buffer_ip;
    GtkWidget *input_pass;
    GtkEntryBuffer *buffer_pass;
    //---------------
    char ip[16];
    char pass[8];
};



G_DEFINE_TYPE_WITH_PRIVATE(UgPlc, ug_plc, GTK_TYPE_BOX)


static void
ug_plc_init (UgPlc *self)
{
    
    g_message("ViewBox init");
    
    UgPlcPrivate* priv=ug_plc_get_instance_private(self);
    // priv -> client = Cli_Create();
    
    snprintf(priv ->ip,16,"192.168.0.1");
    priv ->ip[15]=0;

    // priv->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    priv->buffer_ip = gtk_entry_buffer_new("192.168.0.1",11);
    priv->buffer_pass = gtk_entry_buffer_new("microtec",8);
    
    priv->input_ip = gtk_entry_new_with_buffer(priv->buffer_ip);
    priv->input_pass = gtk_entry_new_with_buffer(priv->buffer_pass);
    priv->btn_connect = gtk_button_new_with_label("connect");




    // gtk_container_add (GTK_CONTAINER (self), GTK_W                 IDGET(priv->vbox));
    gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET(priv->input_ip));
    gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET(priv->input_pass));
    gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET(priv->btn_connect));
    gtk_widget_set_sensitive(priv->input_pass,FALSE);
    
    gtk_box_set_spacing(GTK_BOX(self),5);
    // g_signal_connect (self, "draw", G_CALLBACK (draw), priv);

}




typedef void(*f_func)(void*);
void ug_plc_finalize(GObject *object)
{
    UgPlcPrivate* priv=ug_plc_get_instance_private(UG_PLC(object));
    g_message("viewbox final");

    G_OBJECT_CLASS(ug_plc_parent_class)->finalize (object);
}

static void
default_write_signal_handler (UgPlc *self, gpointer data)
{
    g_message("default");
}


static void
ug_plc_class_init (UgPlcClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);
    obj_class->finalize = ug_plc_finalize;
}




UgPlc *
ug_plc_new (void)
{
    return g_object_new(UG_PLC_TYPE,"orientation",GTK_ORIENTATION_VERTICAL,NULL);
}
