/**
 *
 *
 *
 * Autor: Pedro Maione (maionep[at]tuta.io)
 */

#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdarg.h>
#include <stdio.h>

/* #include "usart.h" */
#include "stm32f1xx_ll_usart.h"

/* #ifndef USARTx_TRACE */
/* #define USARTx_TRACE USART2 */
/* #endif */

#ifndef TRACE_PREFIX_ADD_TIMESTAMP
#define TRACE_PREFIX_ADD_TIMESTAMP 1
#endif /* TRACE_PREFIX_ADD_TIMESTAMP */

#define LOG_LEVEL_OFF 0 /*! *!Logging is disabled */
#define LOG_LEVEL_CRITICAL                                                     \
  1 /*! Indicates the system is unusable, or an error that is unrecoverable */
#define LOG_LEVEL_ERROR 2   /*! Indicates an error condition */
#define LOG_LEVEL_WARNING 3 /*! Indicates a warning condition */
#define LOG_LEVEL_INFO 4    /*! Informational messages */
#define LOG_LEVEL_DEBUG 5   /*! Debug-level messages */
#define LOG_LEVEL_MAX                                                          \
  LOG_LEVEL_DEBUG /*! The maximum log level that can be set */
#define LOG_LEVEL_COUNT                                                        \
  (LOG_LEVEL_MAX + 1) /*! The number of possible log levels */

#define LOG_LEVEL_CRITICAL_PREFIX "<!>"
#define LOG_LEVEL_ERROR_PREFIX "<E>"
#define LOG_LEVEL_WARNING_PREFIX "<W>"
#define LOG_LEVEL_INFO_PREFIX "<I>"
#define LOG_LEVEL_DEBUG_PREFIX "<D>"

#define STRINGPASTE(x) #x
#define TOSTRING(x) STRINGPASTE(x)
#define STRCONCAT(x, y) x##y

#ifndef LOG_LEVEL_NAMES
/*! Users can override these default names with a compiler definition */
#define LOG_LEVEL_NAMES                                                        \
  { "off", "critical", "error", "warning", "info", "debug", }
#endif

#ifndef LOG_LEVEL_SHORT_NAMES
/*! Users can override these default short names with a compiler definition */
#define LOG_LEVEL_SHORT_NAMES                                                  \
  {                                                                            \
    "O", LOG_LEVEL_CRITICAL_PREFIX, LOG_LEVEL_ERROR_PREFIX,                    \
        LOG_LEVEL_WARNING_PREFIX, LOG_LEVEL_INFO_PREFIX,                       \
        LOG_LEVEL_DEBUG_PREFIX,                                                \
  }
#endif

/*  Supply a default log level */
#ifndef LOG_LEVEL
/** Default maximum log level.
 *
 * This is the maximum log level that will be compiled in.
 * To set a custom log level, define the LOG_LEVEL before including this header
 * (e.g., as a compiler definition)
 */
#define LOG_LEVEL LOG_LEVEL_OFF
#endif

#ifndef LOG_ECHO_EN_DEFAULT
/*! Indicates that log statements should be echoed to the console */
/*! If true, log statements will be echoed. */
/*! If false, log statements will only go to the log. */
#define LOG_ECHO_EN_DEFAULT 0
#endif

#define __SHORT_FILE__() past_last_slash(__FILE__ "\0")

#define TRACE_USE_SF 1

#if (TRACE_USE_SF == 1)
#define __FILENAME__ __SHORT_FILE__()
#else
#define __FILENAME__ __FILE__
#endif /* TRACE_USE_SF == 1 */

#define TRACE_USE_PRINTF 1

#define FUNC() __FUNCTION__
#define PRETTY_FUNC() __PRETTY_FUNCTION__

#if (TRACE_PREFIX_ADD_TIMESTAMP == 1)
#ifdef _UNITY_TEST_
#define _TRACE_TS()
#else
#define _TRACE_TS() printf("[%-10lu] ", _millis_fn())
#endif
#endif

#define _TRACE_NL() "\r\n"
#define _TRACE_PREFIX() "<TRACE> "
#define _TRACE_LINE_PREFIX(line) " (" TOSTRING(line) ") "

#define _DUMP_PREFIX() "<DUMP>  "
#define _DUMP_VAR_PREFIX(var) _DUMP_PREFIX() "" TOSTRING(var) " = "

#if (_DEBUG_ == 1)
#if (TRACE_USE_PRINTF == 1)
#define TRACE()                                                                \
  _TRACE_TS();                                                                 \
  printf("%s %s %s %s \n", _TRACE_PREFIX(), __FILENAME__,                      \
         _TRACE_LINE_PREFIX(__LINE__), __FUNCTION__);                          \
  /*  */
#define TRACE_MSG(...)                                                         \
  _TRACE_TS();                                                                 \
  printf("%s ", _TRACE_PREFIX());                                              \
  printf(__VA_ARGS__);                                                         \
  printf("\n")

#define DUMP_STR(var)                                                          \
  _TRACE_TS();                                                                 \
  printf("%s %s\n", _DUMP_VAR_PREFIX(var), var);

#define DUMP_INT(var)                                                          \
  _TRACE_TS();                                                                 \
  printf("%s %lu\n", _DUMP_VAR_PREFIX(var), (uint32_t)var);

#define DUMP_HEX(var)                                                          \
  _TRACE_TS();                                                                 \
  printf("%s %X\n", _DUMP_VAR_PREFIX(var), (uint32_t)var);

#define DUMP_INT_BUF(var, len)                                                 \
  _TRACE_TS();                                                                 \
  printf("%s", _DUMP_VAR_PREFIX(var));                                         \
  for (uint8_t i = 0; i < len; ++i) {                                          \
    printf(" %lu", (uint32_t)var[i]);                                          \
  }                                                                            \
  printf("\n")

#define DUMP_HEX_BUF(var, len)                                                 \
  _TRACE_TS();                                                                 \
  printf("%s", _DUMP_VAR_PREFIX(var));                                         \
  for (uint8_t i = 0; i < len; ++i) {                                          \
    printf(" %lX", (uint32_t)var[i]);                                          \
  }                                                                            \
  printf("\n")

#else

#define TRACE()                                                                \
  Usart_SendStr(USARTx_TRACE, _TRACE_PREFIX());                                \
  Usart_SendStr(USARTx_TRACE, __FILENAME__);                                   \
  Usart_SendStr(USARTx_TRACE, _TRACE_LINE_PREFIX(__LINE__));                   \
  Usart_SendStr(USARTx_TRACE, __FUNCTION__);                                   \
  Usart_SendStr(USARTx_TRACE, _TRACE_NL());

#define TRACE_MSG(...)
#endif /* TRACE_PRINTF */

#else
#define TRACE()
#define TRACE_MSG(...)
#define DUMP_STR(var)
#define DUMP_INT(var)
#define DUMP_HEX(var)
#define DUMP_INT_BUF(var, len)
#define DUMP_HEX_BUF(var, len)
#endif

enum log_level_e {
  log_off = LOG_LEVEL_OFF,
  log_critical = LOG_LEVEL_CRITICAL,
  log_error = LOG_LEVEL_ERROR,
  log_warning = LOG_LEVEL_WARNING,
  log_info = LOG_LEVEL_INFO,
  log_debug = LOG_LEVEL_DEBUG,
};

/*  */
typedef uint32_t (*fnptr_millis_t)(void);
extern fnptr_millis_t _millis_fn;
/*  */
char *past_last_slash(const char *str);
void Log_Print_Level();
void Log(uint8_t level, const char *format, ...);
/* void Log_Serial(uint8_t level, char * msg); */

#ifdef USE_HAL_DRIVER
void Log_Init(USART_TypeDef *husart, uint8_t log_level);
#else
void Log_Init(USART_TypeDef *husart, uint8_t log_level,
              fnptr_millis_t millis_fn);
#endif

#endif /* __LOGGER_H */
