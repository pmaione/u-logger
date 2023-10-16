/**
 *
 *
 *
 * Autor: Pedro Maione (maionep[at]tuta.io)
 */

#include "logger.h"

static uint8_t _log_level = LOG_LEVEL;
static uint8_t _log_timestamp = 1;
static uint8_t _log_distinct = 1;
static const char _log_distinct_msg[] = "L>";

static fnptr_millis_t _millis_fn;

char *past_last_slash(const char *str) {
  char *p0 = (char *)str;
  while (*str++ != '\0') {
    if (*str == '\\' || *str == '/')
      p0 = (char *)str;
  }
  return ++p0;
}

void Log_Print_Level() { printf("LOG LEVEL: %d\n", _log_level); }

void Log_Init(uint8_t log_level, fnptr_millis_t millis_fn) {
  /* log_husart = husart; */

  /* TODO: assert log_level */
  _log_level = log_level;

  /* Register millis function */
  _millis_fn = millis_fn;

  Log_Print_Level();

  printf("\n\n\n");
}

void Log(uint8_t level, const char *format, ...) {
#if (LOG_LEVEL != LOG_LEVEL_OFF)
  va_list args;
  va_start(args, format);

  if (_log_distinct) {
    printf("%s", _log_distinct_msg);
  }

  if (_log_timestamp) {
    printf("[%-10lu] ", (unsigned long)_millis_fn());
  }

  if ((level <= _log_level) && (_log_level > 0)) {
    switch (level) {
    case log_critical:
      printf(LOG_LEVEL_CRITICAL_PREFIX);
      break;
    case log_error:
      printf(LOG_LEVEL_ERROR_PREFIX);
      break;
    case log_warning:
      printf(LOG_LEVEL_WARNING_PREFIX);
      break;
    case log_info:
      printf(LOG_LEVEL_INFO_PREFIX);
      break;
    case log_debug:
      printf(LOG_LEVEL_DEBUG_PREFIX);
      break;
    }
    printf(" - ");
    vprintf(format, args);
  }
  va_end(args);
#endif
}
