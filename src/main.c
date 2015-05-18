#include <pebble.h>
   
#define MAIN_WINDOW_ANIMATED   (true)
   
   
// Private variables   
static Window *MainWindow;
static TextLayer *TimeLayer;

static BitmapLayer *BackgroundLayer;
static GBitmap *BackgroundBmp;
 

// Private function prototypes
static void update_time(void);
static void tick_handler(struct tm *tick_tim, TimeUnits units_changed);
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();
static void deinit();

// MAIN function
int main(void){
   init();
   app_event_loop();
   deinit();
   
}

// Private functions
static void init(){
   // ---------- SERVICES TO SUBSCRIBE TO ----------
   // Register to the tick timer service
   tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
   
   // ---------- WINDOW CREATION ----------
   // Create the main window element and assign it to a pointer
   MainWindow = window_create();
   
   // Set handlers to manage elements inside the window
   window_set_window_handlers(MainWindow, (WindowHandlers){
                                 .load = main_window_load,
                                 .unload = main_window_unload   
   });
   
   // ---------- INIT THE WINDOW ----------
   // Show the window on the watch, with animated setting
   window_stack_push(MainWindow, MAIN_WINDOW_ANIMATED);

   // Display correct time
   update_time();
}

static void deinit(){
   // ---------- WINDOW DESTRUCTION ----------
   // Destroy the window
   window_destroy(MainWindow);
}

static void update_time(void){
   // Get a tm structure
   time_t tempTime = time(NULL);
   struct tm *tick_time = localtime(&tempTime);
   
   // Create a long lived buffer
   static char buffer[] = "00:00";
   
   // Write the current hours and minutes into the buffer
   if(clock_is_24h_style() == true){
      // Use 24 H format
      strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);      
   }
   else{      // Use 12H format
      strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);      
   }
   
   // Display this text on the text layer
   text_layer_set_text(TimeLayer, buffer);
}

static void tick_handler(struct tm *tick_tim, TimeUnits units_changed){
   update_time();
}
   
static void main_window_load(Window *window){
   // ----- Create the Bitmap Layer for background -----------
   // Include bitmap layer before the text layer so text layer
   // comes on top of bitmap layer
   BackgroundBmp = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
   BackgroundLayer = bitmap_layer_create(GRect(0,0,144,168));
   bitmap_layer_set_bitmap(BackgroundLayer, BackgroundBmp);
   layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(BackgroundLayer));
      
   // ----- Create the Text Layer for time -----------
   TimeLayer = text_layer_create(GRect(0,55,144,50));
   text_layer_set_background_color(TimeLayer, GColorClear);
   text_layer_set_text_color(TimeLayer, GColorBlack);
   //text_layer_set_text(s_time_layer, "00:00");
   
   // Improve the layout to be more like a watchface
   text_layer_set_font(TimeLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
   text_layer_set_text_alignment(TimeLayer, GTextAlignmentCenter);
   
   // Add the text layer as a child layer to the Window's root layer
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(TimeLayer));
}

static void main_window_unload(Window *window){
   // Destroy Bitmap layer for background
   bitmap_layer_destroy(BackgroundLayer);
   
   // Destroy Text layer for time
   text_layer_destroy(TimeLayer);
}

