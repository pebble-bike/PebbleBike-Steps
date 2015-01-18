#include <pebble.h>

Window *my_window;
TextLayer *text_layer;

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
  
  static char s_buffer[32];
  snprintf(s_buffer, sizeof(s_buffer), "%d", data->data2);
  text_layer_set_text(text_layer, s_buffer);
  
  // Do something with the data
  APP_LOG(APP_LOG_LEVEL_INFO, "got data %i %i %i", data->data0, data->data1, data->data2);
}

void handle_init(void) {
  my_window = window_create();
  
  Layer *window_layer = window_get_root_layer(my_window);

  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  text_layer = text_layer_create(GRect(5, 5, bounds.size.w, 30));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  window_stack_push(my_window, true);
  
  // Subscribe to get AppWorkerMessages
  app_worker_message_subscribe(worker_message_handler);
  
  AppWorkerResult result = app_worker_launch();
  switch(result) {
    case APP_WORKER_RESULT_SUCCESS:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "Success");
      break;
    case APP_WORKER_RESULT_NOT_RUNNING:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "Not running");
      break;
    case APP_WORKER_RESULT_ALREADY_RUNNING:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "Already Running");
      break;
    case APP_WORKER_RESULT_NO_WORKER:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "No Worker");
      break;
    case APP_WORKER_RESULT_DIFFERENT_APP:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "Different App");
      break;
    case APP_WORKER_RESULT_ASKING_CONFIRMATION:
      APP_LOG(APP_LOG_LEVEL_INFO, "started watchface %s", "Asking Confirmation");
      break;
  }
  
  
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
