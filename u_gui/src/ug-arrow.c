#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glib-object.h>
/****************************
 * Уровень виджета
* ***************************/
#include <ug-arrow.h>
// #include <u_application/u_fuzzy.h>

struct _UgArrowPrivate
{
    GtkDrawingArea parent;
    double value;
    double min;
    double max;
    double size;
    // u_vector* vectors;
    
};






G_DEFINE_TYPE_WITH_PRIVATE(UgArrow, ug_arrow, GTK_TYPE_DRAWING_AREA)




//---------------------------------------------------------------------------------






/*
*      2 
*   1      3
        4
*   5       7
*       6
* */




#include <math.h>
static gboolean
draw (GtkWidget *da, cairo_t   *cr,  UgArrowPrivate*  priv)
{
  gint width, height;
   width = gtk_widget_get_allocated_width (da);
   height = gtk_widget_get_allocated_height (da);

    cairo_set_line_width(cr,2.);
    cairo_set_source_rgb(cr, 0.9,0.8,0.9);
    cairo_arc(cr,width/2,height/2,priv->size,0,2*3.15);
    cairo_fill_preserve(cr);

    cairo_set_source_rgb(cr, 0.,0.,0.);
    // cairo_arc(cr,priv->size,priv->size,priv->size,3.14-0.5,0.5);
    cairo_move_to(cr,width/2,height/2);

    double 
    norm_val=(priv->value-priv->min)/(priv->max-priv->min);
    norm_val=MAX(norm_val,0.);
    norm_val=MIN(norm_val,1.);

    cairo_rel_line_to(cr,priv->size*(0.9*cos(norm_val*4.4+2.5)),priv->size*0.9*sin(norm_val*4.4+2.5));
    cairo_stroke(cr);
    cairo_set_line_width(cr,0.7);
    for (size_t i = 0; i <= 10; i++)
    {
        cairo_move_to(cr,width/2,height/2);
        cairo_rel_move_to(cr,priv->size*(0.85*cos(i*0.44+2.5)),0.85*priv->size*sin(i*0.44+2.5));
        cairo_rel_line_to(cr,priv->size*(0.15*cos(i*0.44+2.5)),0.15*priv->size*sin(i*0.44+2.5));
    }
    cairo_stroke(cr);
    cairo_set_line_width(cr,0.5);
    for (size_t i = 1; i <= 20; i+=2)
    {
        cairo_move_to(cr,width/2,height/2);
        cairo_rel_move_to(cr,priv->size*(0.9*cos(i*0.22+2.5)),0.9*priv->size*sin(i*0.22+2.5));
        cairo_rel_line_to(cr,priv->size*(0.1*cos(i*0.22+2.5)),0.1*priv->size*sin(i*0.22+2.5));
    }
    cairo_stroke(cr);
    // cairo_move_to(cr,priv->size,priv->size);
    cairo_set_source_rgb(cr, 1.,0.,0.);
    cairo_arc(cr,width/2,height/2,priv->size*0.1,0,2*3.15);
    cairo_fill_preserve(cr);

  return TRUE;
}



void
ug_arrow_set_value(UgArrow *self, double digit)
{
    UgArrowPrivate* priv=ug_arrow_get_instance_private(self);
    priv->value=digit;
    gtk_widget_queue_draw(GTK_WIDGET(self));
    g_message("%f",priv->value);
}


static void
ug_arrow_init (UgArrow *self)
{
    
    g_message("ViewBox init");
    
    UgArrowPrivate* priv=ug_arrow_get_instance_private(self);
    priv->size=50.;
    priv->value=0;
    priv->min=0.;
    priv->max=1.;
    gtk_widget_set_size_request(GTK_WIDGET(self),priv->size*2,priv->size*2);
    g_signal_connect (self, "draw", G_CALLBACK (draw), priv);

}




typedef void(*f_func)(void*);
void ug_arrow_finalize(GObject *object)
{
    UgArrowPrivate* priv=ug_arrow_get_instance_private(UG_ARROW(object));
    g_message("viewbox final");

    G_OBJECT_CLASS(ug_arrow_parent_class)->finalize (object);
}

static void
default_write_signal_handler (UgArrow *self, gpointer data)
{
    g_message("default");
}


static void
ug_arrow_class_init (UgArrowClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);
    obj_class->finalize = ug_arrow_finalize;
}





UgArrow *
ug_arrow_new (void)
{
    return g_object_new(UG_ARROW_TYPE,NULL);
}
