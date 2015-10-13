#ifndef DEBUG_H
#define DEBUG_H

#ifndef debug
#ifdef DEBUG
#define debug(format,...) 	Serial.printf(format, ##__VA_ARGS__)
#define debug_P(format,...) Serial.printf_P(format, ##__VA_ARGS__)
#else
#define debug(format,...)
#define debug_P(format,...)
#endif
#endif

#endif
