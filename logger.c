/**
 *
 * 
 * 
 * Autor: Pedro Maione (maionep[at]tuta.io)
*/

#include "logger.h"

static uint8_t _log_timestamp = 1;
static uint8_t _log_level = LOG_LEVEL_OFF;

fnptr_millis_t _millis_fn;

char *past_last_slash(const char *str) {
  char *p0 = (char *)str;
  while (*str++ != '\0') {
    if (*str == '\\' || *str == '/')
      p0 = (char *)str;
  }
  return ++p0;
}

#ifdef USE_HAL_DRIVER
#pragma message "Using HAL DRIVER! \r\n"

#include "stm32f1xx_hal.h"

static UART_HandleTypeDef *log_husart;

uint32_t log_millis(void) { return HAL_GetTick(); }

void Log_Init(UART_HandleTypeDef *husart, uint8_t log_level) {
  log_husart = husart;
  _log_level = log_level;

  printf("\n\n\n");
}

#else
#pragma message "Using LOW LEVEL DRIVER! \r\n"

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_utils.h"

static USART_TypeDef *log_husart;

void Log_Init(USART_TypeDef *husart, uint8_t log_level,
              fnptr_millis_t millis_fn) {
  log_husart = husart;
  _log_level = log_level;
  _millis_fn = millis_fn;

  printf("\n\n\n");
}

#endif

void Log_Print_Level() { printf("LOG LEVEL: %d\n", _log_level); }

void Log(uint8_t level, const char *format, ...) {
#if (LOG_LEVEL != LOG_LEVEL_OFF)
  va_list args;
  va_start(args, format);

  if (_log_timestamp) {
    _TRACE_TS();
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
    printf(" ");
    vprintf(format, args);
  }
  va_end(args);
#endif
}
