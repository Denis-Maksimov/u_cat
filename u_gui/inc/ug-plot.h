#ifndef __UG_PLOT_H__
#define __UG_PLOT_H__



/****************************
 * Уровень приложения
* ***************************/
#include <gtk/gtk.h>
#include <u_types/u_vector.h>

G_BEGIN_DECLS


#define UG_PLOT_TYPE (ug_plot_get_type ())

G_DECLARE_DERIVABLE_TYPE (UgPlot, ug_plot, UG, PLOT, GtkDrawingArea)

typedef struct _UgPlot UgPlot;
typedef struct _UgPlotClass UgPlotClass;
typedef struct _UgPlotPrivate UgPlotPrivate;


// void  UG_app_window_open (UGAppWindow *win, GFile *file);

struct _UgPlotClass
{
		//--родительский класс
		GtkDrawingAreaClass parent_class;

		// guint signal_test_id;
		// void (*_write) (UgPlot *self, gpointer data);
		// void(*UG_app_win_new)(GApplication *app, GFile *file);
		gpointer padding[20]; /* массив указателей; gpointer - переопределение void* */
};




UgPlot *
ug_plot_new(void);


//--(де)активировать построение графика для i-й функции
void ug_plot_axis_enable(UgPlot *self,size_t indx);
void ug_plot_axis_disable(UgPlot *self,size_t indx);

//--масштаб по oY--
void ug_plot_set_scale_Y(UgPlot *self,size_t indx,double scale);

//-- установить по иксу окно видимости indx-ного графика--
void ug_plot_set_view_window(UgPlot *self,size_t indx, size_t from,size_t to);

//--добавить график
size_t ug_plot_push_vector (UgPlot *self,u_vector_float* v);

u_vector_float* ug_plot_get_vector (UgPlot *self, size_t idx);

void ug_plot_set_X_param(UgPlot *self,size_t indx,double x0,double deltaX);

G_END_DECLS



#endif // __UG-PLOT_H__