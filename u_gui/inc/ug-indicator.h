#ifndef __UG_INDICATOR_H__
#define __UG_INDICATOR_H__



/****************************
 * Уровень приложения
* ***************************/
#include <gtk/gtk.h>
#include <u_types/u_vector.h>

G_BEGIN_DECLS


#define UG_INDICATOR_TYPE (ug_indicator_get_type ())

G_DECLARE_DERIVABLE_TYPE (UgIndicator, ug_indicator, UG, INDICATOR, GtkDrawingArea)

typedef struct _UgIndicator UgIndicator;
typedef struct _UgIndicatorClass UgIndicatorClass;
typedef struct _UgIndicatorPrivate UgIndicatorPrivate;


// void  UG_app_window_open (UGAppWindow *win, GFile *file);

struct _UgIndicatorClass
{
		//--родительский класс
		GtkDrawingAreaClass parent_class;

		// guint signal_test_id;
		// void (*_write) (UgIndicator *self, gpointer data);
		// void(*UG_app_win_new)(GApplication *app, GFile *file);
		gpointer padding[20]; /* массив указателей; gpointer - переопределение void* */
};




UgIndicator* ug_indicator_new(void);


G_END_DECLS



#endif // 