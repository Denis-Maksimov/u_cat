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



static void 
draw_digit(cairo_t *cr,uint8_t d, double size)
{
    //123456789
    switch (d)
    {
    case 0:
    case 4:
    case 5:
    case 6:
    case 8:    
    case 9:
        cairo_rel_line_to(cr,0.,size);
        break;
    default:
        cairo_rel_move_to(cr,0.,size);
        break;
    }

    switch (d)
    {
    case 1:
    case 0:
    case 7:
        cairo_rel_move_to(cr,size,0.);
        break;
    default:
        cairo_rel_line_to(cr,size,0.);
        break;
    }

    switch (d)
    {
    case 5:
    case 6:
        cairo_rel_move_to(cr,0.,-size);
        break;
    default:
        cairo_rel_line_to(cr,0.,-size);
        break;
    }

    switch (d)
    {
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 0:
    case 9:
        
        cairo_rel_line_to(cr,-size,0.);
        break;
    default:
        cairo_rel_move_to(cr,-size,0.);
        break;
    }
//
    cairo_rel_move_to(cr,0.,size); 
    switch (d)
    {
    case 2:
    case 6:
    case 8:
    case 0:
        cairo_rel_line_to(cr,0.,size);
        break;
    default:
        cairo_rel_move_to(cr,0.,size);
        break;
    }
    switch (d)
    {
    case 1:
    case 4:
    case 7:
        cairo_rel_move_to(cr,size,0.);
        break;
    default:
        cairo_rel_line_to(cr,size,0.);
        break;
    }
    switch (d)
    {
    case 2:
        cairo_rel_move_to(cr,0.,-size);
        break;
    default:
        cairo_rel_line_to(cr,0.,-size);
        break;
    }


    // cairo_rel_line_to(cr,0.,size);      //вниз 45689
    // cairo_rel_line_to(cr,size,0.);      //вправо 2345689
    // cairo_rel_line_to(cr,0.,-size);     //вверх 12346789 (not 5)
    // cairo_rel_line_to(cr,-size,0.);     //влево 2356789
    
    // cairo_rel_move_to(cr,0.,size); 
    // cairo_rel_line_to(cr,0.,size);      //вниз 268 
    // cairo_rel_line_to(cr,size,0.);      //вправо 235689 
    // cairo_rel_line_to(cr,0.,-size);     //вверх 13456789 (not 2)

}


#include <math.h>
static gboolean
draw (GtkWidget *da, cairo_t   *cr,  UgIndicatorPrivate*  data)
{
  gint width, height;
   width = gtk_widget_get_allocated_width (da);
   height = gtk_widget_get_allocated_height (da);

    cairo_set_line_width(cr,1);
    cairo_set_source_rgb(cr, 0, 0, 0);

    // cairo_move_to(cr,0.,0.);
    // draw_digit(cr, 1, 10);

    for (uint8_t i = 0; i < 10; i++)
    {
        cairo_move_to(cr,i*15.,3.);
        draw_digit(cr, i, 10);
    }
    
    // cairo_rel_line_to(cr,0.,F_SIZE);
    // cairo_rel_line_to(cr,F_SIZE,0.);
    // cairo_rel_line_to(cr,0.,-F_SIZE);
    // cairo_rel_line_to(cr,-F_SIZE,0.);
    
    // cairo_rel_move_to(cr,0.,F_SIZE);
    // cairo_rel_line_to(cr,0.,F_SIZE);
    // cairo_rel_line_to(cr,F_SIZE,0.);
    // cairo_rel_line_to(cr,0.,-F_SIZE);
    // cairo_rel_line_to(cr,-F_SIZE,0.);


    cairo_stroke(cr);

    

  return TRUE;
}

static void
ug_indicator_init (UgIndicator *self)
{
    
    g_message("ViewBox init");
    UgIndicatorPrivate* priv=ug_indicator_get_instance_private(self);
  
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
