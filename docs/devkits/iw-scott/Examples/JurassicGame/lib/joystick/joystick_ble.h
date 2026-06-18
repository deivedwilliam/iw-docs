#include <Arduino.h>
#include <NimBLEDevice.h>

enum JOY_BUTTONS {
  JOY_A,
  JOY_B,
  JOY_X,
  JOY_Y,
  JOY_R1,
  JOY_L1,
  JOY_SELECT,
  JOY_START,
};

typedef void (*button_callback_t)(bool);
typedef void (*movement_callback_t)(int, int);
typedef void (*connect_callback_t)(bool);

class BLE_Client_Joystick {
public:
  BLE_Client_Joystick() {
    movement_function = NULL;
    connection_function = NULL;
    memset(button_functions, 0, sizeof(button_functions));
  }

  ~BLE_Client_Joystick() {}
  void begin();
  void end() {}
  void loop();
  void set_connect_callback(connect_callback_t f) { connection_function = f; }
  connect_callback_t get_connect_callback() { return connection_function; }

  void set_button_A_callback(button_callback_t f) {
    button_functions[JOY_A] = f;
  }
  void set_button_B_callback(button_callback_t f) {
    button_functions[JOY_B] = f;
  }
  void set_button_X_callback(button_callback_t f) {
    button_functions[JOY_X] = f;
  }
  void set_button_Y_callback(button_callback_t f) {
    button_functions[JOY_Y] = f;
  }
  void set_button_select_callback(button_callback_t f) {
    button_functions[JOY_SELECT] = f;
  }
  void set_button_start_callback(button_callback_t f) {
    button_functions[JOY_START] = f;
  }
    void set_button_R1_callback(button_callback_t f) {
    button_functions[JOY_L1] = f;
  }
  void set_button_L1_callback(button_callback_t f) {
    button_functions[JOY_R1] = f;
  }

  button_callback_t get_button_callback(size_t button) {
    if (button > sizeof(button_functions)) return NULL;
    return button_functions[button];
  }
  void set_movement_callback(movement_callback_t f) { movement_function = f; }
  movement_callback_t get_movement_callback() { return movement_function; }

private:
  button_callback_t button_functions[16];
  movement_callback_t movement_function;
  connect_callback_t connection_function;
};

typedef struct __attribute__((__packed__)) {
  uint8_t x;
  uint8_t y;
  uint8_t z;
  uint8_t rz;
  uint8_t buttons;
  uint8_t user;
  uint8_t blank1;
  uint8_t blank2;
} joystick_t;