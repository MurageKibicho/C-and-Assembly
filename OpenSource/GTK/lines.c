/*Run: gcc linetest.c -o linetest.o `pkg-config --cflags --libs gtk+-3.0`
*C program for basic drawing with GTK+ and cairo
*Working 2020 example if this got you stuck, http://zetcode.com/gfx/cairo/basicdrawing/
*Note, to run you need ``, it's right before 1 on your keyboard
*/
#include <cairo.h>
#include <gtk/gtk.h>



//function prototypes
static void do_drawing(cairo_t *);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static void do_drawing(cairo_t *cr);
static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
//end of function prototypes

/* Global variables for storing mouse coordinates,
* count is index of arrays, coordx and coordy are x and y coordinates of the mouse
*/
struct {
  int count;
  double coordx[100];
  double coordy[100];
} glob;


/* Function: on_draw_event
*Parameters: GtkWidget, cairo_t, gpointer
*Use: This is the function we attach to the main method when we want to draw. It calls the do_drawing method
*Example: g_signal_connect(G_OBJECT(darea), "draw",G_CALLBACK(on_draw_event), NULL); 
*/
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
  gpointer user_data)
{
  do_drawing(cr);

  return FALSE;
}


/* Function: do_drawing
*Parameters: cairo_t
*Use: It sets cairo canvas settings, and draws shapes with a for loop
*Settings: are commented
*Note: printf is used during debugging to find mouse click coordinates :)
*/
static void do_drawing(cairo_t *cr)
{
  cairo_set_source_rgb(cr, 0, 0, 0);//Line colour
  cairo_set_line_width(cr, 0.5);//Line width

  //i is starting point, i+1 is next mouse coordinate 
  int i;
  for (i = 0; i < glob.count - 1; i++ ) {
    cairo_move_to(cr, glob.coordx[i], glob.coordy[i]);
    cairo_line_to(cr, glob.coordx[i+1], glob.coordy[i+1]);
    //printf("from x:%f, y:%f\t to: x:%f, y:%f\n",glob.coordx[i],glob.coordy[i], glob.coordx[i+1], glob.coordy[i+1]);
    cairo_stroke(cr);    

  }
  //resets array so shape can be drawn again
  glob.count = 0;
}


/* Function: clicked
*Parameters: GtkWidget, GdkEventButton, gpointer
*Use: Registers mouse clicks, 1 is right, 3 is left on laptop. Clicks may be 1, 2 or 3 on a desktop
*Note: printf is used during debugging to find mouse click coordinates :)
*/
static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
  gpointer user_data)
{
  if (event->button == 1) {
       // printf("Right Click");
    glob.coordx[glob.count] = event->x;
    glob.coordy[glob.count++] = event->y;

        // int i;
        // for (i =0; i <= glob.count-1; i++) {
        //   printf("%f\n", glob.coordx[i]);
        // }
  }

  if (event->button == 3) {
        //printf("left Click");
    gtk_widget_queue_draw(widget);
  }

  return TRUE;
}
//Main method
int main(int argc, char *argv[])
{
  //widget variables, window and drawing area
  GtkWidget *window;
  GtkWidget *darea;
  
  //set global count 0, so array is at beginning whenver program starts
  glob.count = 0;

  //Always have this to start GTK
  gtk_init(&argc, &argv);

  //Set new window, set new drawing area
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  darea = gtk_drawing_area_new();

  //add the drawing area to the window
  gtk_container_add(GTK_CONTAINER(window), darea);

  //You need this to register mouse clicks
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  //Attaching draw function to the main method
  g_signal_connect(G_OBJECT(darea), "draw", 
    G_CALLBACK(on_draw_event), NULL); 

  //You can close window when you exit button
  g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);  

  //Register if left or right mouse click
  g_signal_connect(window, "button-press-event", 
    G_CALLBACK(clicked), NULL);

  //Set window position, default size, and title
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300); 
  gtk_window_set_title(GTK_WINDOW(window), "Lines");

  //show all widgets
  gtk_widget_show_all(window);

  //start window
  gtk_main();

  return 0;
}
