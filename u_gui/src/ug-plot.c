#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glib-object.h>
/****************************
 * Уровень виджета
* ***************************/
#include "ug-plot.h"
#include <u_application/u_fuzzy.h>

struct _UgPlotPrivate
{
    GtkDrawingArea parent;
    
    u_vector* vectors;
    
};

// union g{
//     uint8_t a;
//     struct{
//         int b:1;
//         int c:1;
//         int d:1;
//         int e:1;
//         int f:1;
//         int g:1;
//         int h:1;
//         int j:1;
//     };
// };


typedef struct 
{
    
    //---параметры данных---
    struct{
        //указатель на данные
        u_vector_float* v;

        float max,min;          //минимальные и максимальные значения в выбранном диапазоне
        float x0;               //начало отсчёта по оХ
        float delta_x;          //шаг в единицах измерений
    };
    
     //---экран---
     struct{
        double width;
        double height;
     };

     //---масштабирование области построения---
     struct{
        //шаг по oX
        double step_x;
        
        //массштаб по оУ
        double scale_y;

        //окно построения
        size_t from;
        size_t to;
     };
     
    
    //---флаги доп.функционала---
    struct{
        gboolean 
            autoscale:1, //автоподгон массштаба
            axis:1;      //рисовать оси к графику
    };
   
    
}plot_float;



G_DEFINE_TYPE_WITH_PRIVATE(UgPlot, ug_plot, GTK_TYPE_DRAWING_AREA)


//--пересчитывает минимум и максимум в выбранном диапазоне
static void 
autoscale(plot_float*p)
{
    p->autoscale=TRUE;
    float max,min,k;
    max =min=u_vector_float_at(p->v,p->from);
    for (size_t i = p->from; i < p->to; i++)
    {
        k=u_vector_float_at(p->v,i);
       max=U_MAX(max, k);
       min=U_MIN(min,k);
    }
    p->max=max;
    p->min=min;
}


//---------------------------------------------------------------------------------
void
ug_plot_set_view_window(UgPlot *self,size_t indx, size_t from,size_t to)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);

    plot_float* p=u_vector_at(priv->vectors,indx);
    p->from=from;
    p->to=to;
    if(p->autoscale){
        autoscale(p);
    }
}

void
ug_plot_set_scale_Y(UgPlot *self,size_t indx,double scale)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);

    plot_float* p=u_vector_at(priv->vectors,indx);
    p->autoscale=FALSE;
    p->scale_y=scale;
}


void
ug_plot_set_X_param(UgPlot *self,size_t indx,double x0,double deltaX)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);

    plot_float* p=u_vector_at(priv->vectors,indx);
    p->delta_x=deltaX;
    p->x0=x0;
    
}

void
ug_plot_axis_enable(UgPlot *self,size_t indx)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);

    plot_float* p=u_vector_at(priv->vectors,indx);
    p->axis=TRUE;
}

void
ug_plot_axis_disable(UgPlot *self,size_t indx)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);

    plot_float* p=u_vector_at(priv->vectors,indx);
    p->axis=FALSE;
}
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
draw_axis_Y(cairo_t   *cr,double width, double offY)
{
    cairo_move_to(cr, 0, offY);
    cairo_line_to(cr,width,offY);
    // cairo_stroke(cr);
}

static void 
draw_axis_X(cairo_t   *cr,double width,double height, double from, double to)
{
    double step=width/(to-from);
    from*step*(-1.);
    cairo_move_to(cr, from*step*(-1.), height/2.);
    cairo_line_to(cr, from*step*(-1.), -height/2.);
    
    // cairo_stroke(cr)    
}
static void
draw_axis(cairo_t   *cr,double width, double height,double offY, plot_float *data)
{
    cairo_move_to(cr, 0, offY);
    cairo_line_to(cr,width,offY);

// draw_axis_X(cr,width,height,data->delta_x*(data->from+data->x0),data->delta_x*(data->to+data->x0));

    if(((data->delta_x*(data->from+data->x0))<0.)
        &&(data->delta_x*(data->to+data->x0)>0.))
    {
        double from =data->delta_x*(data->from+data->x0);
        double to= data->delta_x*(data->to+data->x0);
        double step=width/(to-from);

        cairo_move_to(cr, (from)*step*(-1.), height/2.);
        cairo_line_to(cr, from*step*(-1.), -height/2.);
        cairo_stroke(cr) ;
    }
}

static void
plot(cairo_t *cr, double width, double height, plot_float* data)
{
    cairo_set_line_width(cr,1);
    if(!data)return;
    size_t n_el=(data->to-data->from);
    data->step_x=(double)width/(double)(n_el-1);
    double offY=0.;
    if(data->autoscale){

        data->scale_y=height/((data->max-data->min));
        offY=(-data->min*data->scale_y-height/2.);
    }
    cairo_move_to(cr, 0, offY+data->scale_y*u_vector_float_at(data->v,data->from));
    for (size_t i = data->from+1; i < data->to; i++)
    {
        cairo_line_to(cr, (i)*data->step_x, offY+data->scale_y*u_vector_float_at(data->v,i));
    }

    // cairo_move_to(cr, 0, offY);
    // cairo_line_to(cr,width,offY);
    cairo_stroke(cr);
    if(data->axis) {
        cairo_set_line_width(cr,1.5);
        draw_axis(cr, width,  height, offY, data);
    }
    
}

#include <math.h>
static gboolean
plotter_draw (GtkWidget *da,
                   cairo_t   *cr,
                   UgPlotPrivate*  data)
{
  gint width, height;
   width = gtk_widget_get_allocated_width (da);
  height = gtk_widget_get_allocated_height (da);

    dekart(cr,height);
    // ve.step_x=(double)width/(double)data->n_elem;

    
    cairo_set_line_width(cr,1);
    cairo_set_source_rgb(cr, 0, 0, 0);

     double _r,_g,_b;
     double r=(double)(data->vectors->n_elem);
    for (size_t i = 0; i < data->vectors->n_elem; i++)
    {
        
        _r=(sin((((double)i/r)*G_PI)+G_PI/3.)<0.)  ? 0. : sin((((double)i/r)*G_PI)+G_PI/3.);
        _g=(sin((((double)i/r)*G_PI)+0.)<0.)?0.:sin((((double)i/r)*G_PI)+0.);
        _b=(sin((((double)i/r)*G_PI)+2.*G_PI/3.)<0.)?0.:(sin((((double)i/r)*G_PI)+2.*G_PI/3.));

        cairo_set_source_rgb(cr,_r,_g,_b);

        plot(cr,width, height,(plot_float*)u_vector_at(data->vectors,i));
        
    }
    

    cairo_stroke(cr);

    

  return TRUE;
}

static void
ug_plot_init (UgPlot *self)
{
    
    g_message("ViewBox init");
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);
    priv->vectors=u_vector_new();
    g_signal_connect (self, "draw", G_CALLBACK (plotter_draw), priv);

}


size_t
ug_plot_push_vector (UgPlot *self,u_vector_float* v)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);
    
    plot_float* p=malloc(sizeof(plot_float));
    p->v=v;
    p->from=0;
    p->to=v->n_elem;
    p->scale_y=1.;
    p->axis=FALSE;
    p->delta_x=1.;
    p->x0=(-10.);
    autoscale(p);
    u_vector_push_back(priv->vectors,p);
    
    return priv->vectors->n_elem-1;
}

u_vector_float*
ug_plot_get_vector (UgPlot *self, size_t idx)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(self);
    plot_float* p =u_vector_at(priv->vectors,idx);
    return p->v;
}


static void
free_f(plot_float* p)
{
    u_vector_float_free(p->v);
    free(p);
}


typedef void(*f_func)(void*);
void ug_plot_finalize(GObject *object)
{
    UgPlotPrivate* priv=ug_plot_get_instance_private(UG_PLOT(object));
    g_message("viewbox final");
    u_vector_free(priv->vectors,(f_func)free_f);
    G_OBJECT_CLASS(ug_plot_parent_class)->finalize (object);
}

static void
default_write_signal_handler (UgPlot *self, gpointer data)
{
    g_message("default");
}


static void
ug_plot_class_init (UgPlotClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);
    obj_class->finalize = ug_plot_finalize;
}





UgPlot *
ug_plot_new (void)
{
    
    return g_object_new(UG_PLOT_TYPE,NULL);
}
