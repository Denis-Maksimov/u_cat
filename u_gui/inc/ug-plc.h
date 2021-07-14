#ifndef __UG_PLC_H__
#define __UG_PLC_H__



/****************************
 * Уровень приложения
* ***************************/
#include <gtk/gtk.h>
#include <u_types/u_vector.h>

G_BEGIN_DECLS


#define UG_PLC_TYPE (ug_plc_get_type ())

G_DECLARE_DERIVABLE_TYPE (UgPlc, ug_plc, UG, PLC, GtkBox)

typedef struct _UgPlc UgPlc;
typedef struct _UgPlcClass UgPlcClass;
typedef struct _UgPlcPrivate UgPlcPrivate;


// void  UG_app_window_open (UGAppWindow *win, GFile *file);

struct _UgPlcClass
{
		//--родительский класс
		GtkBoxClass parent_class;

		// guint signal_test_id;
		// void (*_write) (UgPlc *self, gpointer data);
		// void(*UG_app_win_new)(GApplication *app, GFile *file);
		gpointer padding[40]; /* массив указателей; gpointer - переопределение void* */
};




UgPlc* ug_plc_new(void);
// void ug_plc_set_value(UgPlc *self, double digit);

G_END_DECLS



#endif // 