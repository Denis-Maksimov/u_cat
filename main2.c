
// #include <u_application/u_fsm.h>
// #include <u_application/s5_timers.h>
#include <u_cat.h>
void 
foo(fsm* f,void* data);
void 
bar(fsm* f,void* data);
void 
baz(fsm* f,void* data);

#include "ug-plot.h"
#include <u_application/u_ema.h>

void plotter(){
      gtk_init(0, 0);


//==================================================
    u_ema* ma=u_ema_new();
    u_ema_set_N(ma, 30);
    u_ema* ma30=u_ema_new();
    u_ema_set_N(ma30, 60);
    float ppir=0.;
    u_vector_float* vf=u_vector_float_new();
    
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t i = 0; i < 20; i++)
        {
            ppir=g_random_double_range(-1.,1.);
            u_vector_float_push_back(vf,ppir);
            u_ema_push( ma, ppir);
            u_ema_push( ma30, ppir);

        }
        for (size_t i = 0; i < 10; i++)
        {
            ppir=g_random_double_range(-1.,1.);
            u_vector_float_push_back(vf,ppir);
            u_ema_push( ma, ppir);
            u_ema_push( ma30, ppir);
        }
    }

    u_ivr* ivr=u_new_IVR(ma30->ema);

//==================================================
    UgPlot *pl=ug_plot_new();
    ug_plot_push_vector (pl,ma->ema);
    ug_plot_push_vector (pl,ma30->ema);
    // ug_plot_push_vector (pl,vf);
    ug_plot_push_vector (pl,ivr->IVR);
    ug_plot_push_vector (pl,ivr->F);
    ug_plot_push_vector (pl,ivr->source);

//==================================================
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *button = gtk_button_new_with_label("EXIT");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(vbox));

    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    

    // do_sidebar (0);
    gtk_widget_set_size_request (GTK_WIDGET(pl), 900, 400);
     gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(pl));
    gtk_container_add (GTK_CONTAINER (vbox), GTK_WIDGET(button));
    gtk_widget_show_all(GTK_WIDGET(window));



   

    gtk_main();
}


//=============================================
static
u_pid* pidr;

void 
foo(fsm* f,void* data)
{
   static int abc=0;
   static float x=0.;
    if(abc<5000)
    {
        // change_state(f, bar, &pidr);
        abc++;
         
        float y = proc_pid(pidr, x, 500.0);
        x=x+y;
        printf ("y=%f\n",x);
        // printf("ba %d\n",abc);
    }else
    // if(abc<6) {abc++;printf("baz %d\n",abc);}
    // else
    end_state(f);
}
void 
bar(fsm* f,void* data)
{

    // static float x=0.;
    // float y = proc_pid((u_pid*)data, x, 500.0);
    // x=x+y;
    // printf ("y=%f\n",x);

    static int as=0;
    as++;
    if(as==3)
    {
        
        
        change_state(f, baz, &as);
        printf("barracuda %d\n",f->stack.n_elem);
        
    }else{
    //   printf("barracuda end %d\n",*(int*)data);
      end_state(f);
    }

}
void 
baz(fsm* f,void* data)
{
    printf("bazalt %d\n",f->stack.n_elem);
    fflush(stdout);
    end_state(f);
}

#include <unistd.h>
#include <u_application/u_ema.h>
#include <math.h>
#include <u_application/u_fuzzy.h>

int main(int argc, char const *argv[])
{
  plotter();
  fsm* fv=fsm_new(foo);
  pidr = new_pid(0.005,0.005, 0.000, 1000);
  while(update(fv)){
    //   usleep(1000);
  };
  fsm_free(fv);
//   test_processing();

    u_ema* ma=u_ema_new();
    u_ema_set_N(ma, 5);
    float ppir=0.;
    for (size_t i = 0; i < 50; i++)
    {
        ppir=((float)(sinf((((float)i)/50.)*3.14)));
        u_ema_push( ma, ppir);
    }
    // for (size_t i = 0; i < 25; i++)
    // {
    //     ppir-=25.;
    //     u_ema_push( ma, ppir);
    //     // ma->value->mem[i];
    // }
    printf("-------------MMM----------\n");
    ema_print(ma);
    printf("Pirson: %f\n",u_r(ma->ema,ma->value));
    u_ivr* var= u_new_IVR(ma->value);
    printf("median: %f\n",u_median_IVR(var));
    printf("moda: %f\n",u_moda_IVR(var));
    printf("n: %i\n",var->n);
    for (size_t i = 0; i < var->IVR->n_elem; i++)
    {
      printf("%i: ",i+1);
      if(i)
        if(i==var->IVR->n_elem-1)
        {
           printf("AVR: [%f;inf] ", (var->min+(var->delta*(i-1))));
        }
        else
          printf("AVR: [%f;%f] ", var->min+var->delta*(i-1), var->min+(var->delta*i));
        else
          printf("AVR: [-inf;%f] ",(var->min+(var->delta*(i))));
      
      printf("IVR: %f  ",var->IVR->mem[i]);
      printf("F: %f\n",var->F->mem[i]);
    }
    
   u_fuzzy_test();
   
  return 0;
}

//=================================================================


