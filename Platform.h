#ifdef ARDUINO_ARCH_AVR
#endif

#ifdef ARDUINO_ARCH_ESP32
#define analogWrite ledcWrite
#endif