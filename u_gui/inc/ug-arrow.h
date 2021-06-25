#ifndef __UG_ARROW_H__
#define __UG_ARROW_H__



/****************************
 * Уровень приложения
* ***************************/
#include <gtk/gtk.h>
#include <u_types/u_vector.h>

G_BEGIN_DECLS


#define UG_ARROW_TYPE (ug_arrow_get_type ())

G_DECLARE_DERIVABLE_TYPE (UgArrow, ug_arrow, UG, ARROW, GtkDrawingArea)

typedef struct _UgArrow UgArrow;
typedef struct _UgArrowClass UgArrowClass;
typedef struct _UgArrowPrivate UgArrowPrivate;


// void  UG_app_window_open (UGAppWindow *win, GFile *file);

struct _UgArrowClass
{
		//--родительский класс
		GtkDrawingAreaClass parent_class;

		// guint signal_test_id;
		// void (*_write) (UgArrow *self, gpointer data);
		// void(*UG_app_win_new)(GApplication *app, GFile *file);
		gpointer padding[20]; /* массив указателей; gpointer - переопределение void* */
};




UgArrow* ug_arrow_new(void);
void ug_arrow_set_value(UgArrow *self, double digit);

G_END_DECLS



#endif // 