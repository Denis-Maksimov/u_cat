#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glib-object.h>
/****************************
 * Уровень виджета
* ***************************/
#include <ug-indicator.h>
// #include <u_application/u_fuzzy.h>

struct _UgIndicatorPrivate
{
    GtkDrawingArea parent;
    long value;
    double size;
    // u_vector* vectors;
    
};






G_DEFINE_TYPE_WITH_PRIVATE(UgIndicator, ug_indicator, GTK_TYPE_DRAWING_AREA)




//---------------------------------------------------------------------------------


static void 
dekart(cairo_t *cr,gint height){

    cairo_matrix_t x_reflection_matrix; 
    cairo_matrix_init_identity(&x_reflection_matrix); // could not find a oneliner
    /* reflection through the x axis equals the identity matrix with the bottom 
    left value negated  */
    x_reflection_matrix.xx=1.;
    x_reflection_matrix.yy = -1.0;
    cairo_set_matrix(cr, &x_reflection_matrix);
    // This would result in your drawing being done on top of the destination 
    // surface, so we translate the surface down the full height
    cairo_translate(cr, 0., -(double)height/2.); // replace SURFACE_HEIGHT
    // ... do your drawing

}





/*
*      2 
*   1      3
        4
*   5       7
*       6
* */


enum
{
    _9=0b01101111,
    _8=0b01111111,
    _7=0b01000110,
    _6=0b01111011,
    _5=0b01101011,
    _4=0b01001101,
    _3=0b01101110,
    _2=0b00111110,
    _1=0b01000100,
    _0=0b01110111,
_minus=0b00001000,
};

const uint8_t 
table[]={_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_minus};



static void 
draw_digit(cairo_t *cr,uint8_t d, double size)
{
    if(d>10)return;
    cairo_rel_move_to(cr,size*1.5,size*1.5);

    if (table[d] & 0b1)
        cairo_rel_line_to(cr, 0., -size);
    else
        cairo_rel_move_to(cr, 0., -size);
    if (table[d] & 0b10)
        cairo_rel_line_to(cr, size, 0.);
    else
        cairo_rel_move_to(cr, size, 0.);
    if (table[d] & 0b100)
        cairo_rel_line_to(cr, 0., size);
    else
        cairo_rel_move_to(cr, 0., size);
    if (table[d] & 0b1000)
        cairo_rel_line_to(cr, -size, 0.);
    else
        cairo_rel_move_to(cr, -size, 0.);
    if (table[d] & 0b10000)
        cairo_rel_line_to(cr, 0., size);
    else
        cairo_rel_move_to(cr, 0., size);
    if (table[d] & 0b100000)
        cairo_rel_line_to(cr, size, 0.);
    else
        cairo_rel_move_to(cr, size, 0.);
    if (table[d] & 0b1000000)
        cairo_rel_line_to(cr, 0., -size);
    else
        cairo_rel_move_to(cr, 0., -size);

}

static void 
draw_digit_ch(cairo_t *cr,char d, double size){

    if(d>='0'&&d<='9')
    draw_digit(cr, d-'0', size);
    else if(d=='-')
    draw_digit(cr, 10, size);
}



static void 
draw_int(cairo_t *cr, long d, double size)
{
    char b[21];
    sprintf(b,"%ld",d);
    size_t s=strlen(b);
    size_t max=20;
    size_t begin=max-s;

    cairo_set_source_rgb(cr,0.15,0.15,0.15);
    for (size_t i = 0; i < max; i++)
    {
        cairo_move_to(cr,(i)*size*1.5,3.);
        draw_digit(cr, 8, size);
    }
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,1.,0,0);
    for (size_t i = 0; i < s; i++)
    {       
        cairo_move_to(cr,(i+begin)*size*1.5,3.);
        draw_digit_ch(cr, b[i], size);   
    }
    cairo_stroke(cr);
}




#include <math.h>
static gboolean
draw (GtkWidget *da, cairo_t   *cr,  UgIndicatorPrivate*  priv)
{
  gint width, height;
   width = gtk_widget_get_allocated_width (da);
   height = gtk_widget_get_allocated_height (da);

    
    cairo_set_source_rgb(cr,0.,0.3,0.2);
    cairo_rectangle(cr,0,0, 330,40);
    cairo_fill(cr);

    cairo_set_line_width(cr,1);
    cairo_set_source_rgb(cr,0.,0.,0.);
    cairo_rectangle(cr,0,0, 330,40);
    
    cairo_stroke(cr);
    

    draw_int(cr,priv->value, priv->size);


    // cairo_stroke(cr);

    

  return TRUE;
}



void
ug_indicator_set_value(UgIndicator *self, long digit)
{
    UgIndicatorPrivate* priv=ug_indicator_get_instance_private(self);
    priv->value=digit;
    gtk_widget_queue_draw(GTK_WIDGET(self));
}


static void
ug_indicator_init (UgIndicator *self)
{
    
    g_message("ViewBox init");
    gtk_widget_set_size_request(GTK_WIDGET(self),330,40);
    UgIndicatorPrivate* priv=ug_indicator_get_instance_private(self);
    priv->size=10.;
    priv->value=0;
    g_signal_connect (self, "draw", G_CALLBACK (draw), priv);

}




typedef void(*f_func)(void*);
void ug_indicator_finalize(GObject *object)
{
    UgIndicatorPrivate* priv=ug_indicator_get_instance_private(UG_INDICATOR(object));
    g_message("viewbox final");

    G_OBJECT_CLASS(ug_indicator_parent_class)->finalize (object);
}

static void
default_write_signal_handler (UgIndicator *self, gpointer data)
{
    g_message("default");
}


static void
ug_indicator_class_init (UgIndicatorClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);
    obj_class->finalize = ug_indicator_finalize;
}





UgIndicator *
ug_indicator_new (void)
{
    return g_object_new(UG_INDICATOR_TYPE,NULL);
}
