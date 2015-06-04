#ifndef TERMUTIL_H
#define TERMUTIL_H

#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#if defined(__TU_USE_CURSES__)
#  include <curses.h>
#elif defined(_MSC_VER)
#  include <stdlib.h>
#  include <conio.h>
#  ifdef NOMINMAX
#    define NOMONMAX_IS_NOT_DEFINED
#    define NOMINMAX
#  endif
#  ifdef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  ifdef NOMONMAX_IS_NOT_DEFINED
#    undef NOMINMAX
#  endif
#  ifdef WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WIN32_LEAN_AND_MEAN
#  endif
#else
#  include <fcntl.h>
#  include <termios.h>
#  include <unistd.h>
#  include <sys/ioctl.h>
#endif

#ifndef __TU_NOINLINE__
#  define __TU_STATIC__  static
#  if defined(_MSC_VER)
#    define __TU_INLINE__  __forceinline
#  elif defined(__GNUC__)
#    if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 0))
#      define __TU_INLINE__  __attribute__((always_inline)) inline
#    else
#      define __TU_INLINE__  inline
#    endif
#  elif __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#    define __TU_INLINE__  inline
#  else
#    define __TU_INLINE__
#  endif
#else
#  define __TU_STATIC__
#  define __TU_INLINE__
#endif


typedef struct {
  int row;
  int col;
} TuTermSize;

#if defined(__TU_USE_CURSES__)
typedef enum {
  TU_BLACK   = COLOR_BLACK,
  TU_RED     = COLOR_RED,
  TU_GREEN   = COLOR_GREEN,
  TU_YELLOW  = COLOR_YELLOW,
  TU_BLUE    = COLOR_BLUE,
  TU_MAGENTA = COLOR_MAGENTA,
  TU_CYAN    = COLOR_CYAN,
  TU_GRAY    = COLOR_WHITE,
  TU_DEFAULT_COLOR = COLOR_WHITE,
  TU_DEFAULT_FOREGROUND = 7,
  TU_DEFAULT_BACKGROUND = 0
} TuColor;

typedef enum {
  TU_ATTR_DEFAULT = A_NORMAL,
  TU_ATTR_BOLD = A_BOLD,
  TU_ATTR_UNDERLINE = A_UNDERLINE,
  TU_ATTR_REVERSE = A_REVERSE
} TuAttr;
#elif defined(_MSC_VER)
typedef enum {
  TU_BLACK   = 0,
  TU_BLUE    = 1,
  TU_GREEN   = 2,
  TU_CYAN    = 3,
  TU_RED     = 4,
  TU_MAGENTA = 5,
  TU_YELLOW  = 6,
  TU_GRAY    = 7,
  TU_DEFAULT_COLOR = -1
} TuColor;

typedef enum {
  TU_ATTR_DEFAULT,
  TU_ATTR_BOLD,
  TU_ATTR_UNDERLINE,
  TU_ATTR_REVERSE
} TuAttr;

#define __TU_BOLD_MASK  0x08

#else
typedef enum {
  TU_BLACK   = 30,
  TU_RED     = 31,
  TU_GREEN   = 32,
  TU_YELLOW  = 33,
  TU_BLUE    = 34,
  TU_MAGENTA = 35,
  TU_CYAN    = 36,
  TU_GRAY    = 37,
  TU_DEFAULT_COLOR = 39
} TuColor;

typedef enum {
  TU_ATTR_DEFAULT   = 0,
  TU_ATTR_BOLD      = 1,
  TU_ATTR_VERTLINE  = 2,
  TU_ATTR_UNDERLINE = 4,
  TU_ATTR_BLINK     = 5,
  TU_ATTR_REVERSE   = 7,
  TU_ATTR_HIDE      = 8
} TuAttr;
#endif

#define TU_FG_BLACK     "\x1b[30m"
#define TU_FG_RED       "\x1b[31m"
#define TU_FG_GREEN     "\x1b[32m"
#define TU_FG_YELLOW    "\x1b[33m"
#define TU_FG_BLUE      "\x1b[34m"
#define TU_FG_MAGENTA   "\x1b[35m"
#define TU_FG_CYAN      "\x1b[36m"
#define TU_FG_GRAY      "\x1b[37m"
#define TU_FG_DEFAULT   "\x1b[39m"

#define TU_FG_LBLACK    "\x1b[1;30m"
#define TU_FG_LRED      "\x1b[1;31m"
#define TU_FG_LGREEN    "\x1b[1;32m"
#define TU_FG_LYELLOW   "\x1b[1;33m"
#define TU_FG_LBLUE     "\x1b[1;34m"
#define TU_FG_LMAGENTA  "\x1b[1;35m"
#define TU_FG_LCYAN     "\x1b[1;36m"
#define TU_FG_LGRAY     "\x1b[1;37m"

#define TU_BG_BLACK     "\x1b[40m"
#define TU_BG_RED       "\x1b[41m"
#define TU_BG_GREEN     "\x1b[42m"
#define TU_BG_YELLOW    "\x1b[43m"
#define TU_BG_BLUE      "\x1b[44m"
#define TU_BG_MAGENTA   "\x1b[45m"
#define TU_BG_CYAN      "\x1b[46m"
#define TU_BG_GRAY      "\x1b[47m"
#define TU_BG_DEFAULT   "\x1b[49m"


#ifdef __cplusplus
extern "C" {
#endif
void
tu_init(void);

__TU_STATIC__ __TU_INLINE__ TuTermSize
tu_get_termsize(void);

__TU_STATIC__ __TU_INLINE__ void
tu_set_color(TuColor fc, TuColor bc);

__TU_STATIC__ __TU_INLINE__ void
tu_set_foreground(TuColor color);

__TU_STATIC__ __TU_INLINE__ void
tu_set_background(TuColor color);

__TU_STATIC__ __TU_INLINE__ void
tu_set_attribute(TuAttr attr);

__TU_STATIC__ __TU_INLINE__ void
tu_move(int y, int x);

__TU_STATIC__ __TU_INLINE__ void
_tu_move(int y, int x);

__TU_STATIC__ __TU_INLINE__ void
tu_move_up(int delta_y);

__TU_STATIC__ __TU_INLINE__ void
tu_move_down(int delta_y);

__TU_STATIC__ __TU_INLINE__ void
tu_move_right(int delta_x);

__TU_STATIC__ __TU_INLINE__ void
tu_move_left(int delta_x);

__TU_STATIC__ __TU_INLINE__ void
tu_clear(void);

__TU_STATIC__ __TU_INLINE__ void
_tu_clear(void);

__TU_STATIC__ __TU_INLINE__ void
tu_clear_right(void);

__TU_STATIC__ __TU_INLINE__ void
tu_clear_left(void);

__TU_STATIC__ __TU_INLINE__ void
tu_clear_line(void);

__TU_STATIC__ __TU_INLINE__ void
_tu_set_foreground(const char *color_str);

__TU_STATIC__ __TU_INLINE__ void
_tu_set_background(const char *color_str);

__TU_STATIC__ __TU_INLINE__ void
tu_addch(int c);

__TU_STATIC__ __TU_INLINE__ void
tu_mvaddch(int y, int x, int c);

__TU_STATIC__ __TU_INLINE__ void
tu_addstr(const char *str);

__TU_STATIC__ __TU_INLINE__ void
tu_mvaddstr(int y, int x, const char *str);

__TU_STATIC__ __TU_INLINE__ void
tu_echo(void);

__TU_STATIC__ __TU_INLINE__ void
tu_noecho(void);

__TU_STATIC__ __TU_INLINE__ void
tu_cbreak(void);

__TU_STATIC__ __TU_INLINE__ void
tu_nocbreak(void);

__TU_STATIC__ __TU_INLINE__ void
tu_nonblocking(void);

__TU_STATIC__ __TU_INLINE__ void
tu_blocking(void);

__TU_STATIC__ __TU_INLINE__ void
tu_refresh(void);

__TU_STATIC__ __TU_INLINE__ int
tu_getch(void);

__TU_STATIC__ __TU_INLINE__ int
tu_kbhit(void);

__TU_STATIC__ __TU_INLINE__ void
tu_setcur(int is_show);

__TU_STATIC__ __TU_INLINE__ void
tu_beep(void);

#ifdef __TU_NOINLINE__
void
tu_printw(const char *fmt, ...);

void
tu_mvprintw(int y, int x, const char *fmt, ...);
#endif
#ifdef __cplusplus
}
#endif


#ifdef _MSC_VER
struct __TuAttribute {
  int is_bold;
  int is_reverse;
};
#else
extern struct termios tu_init_tio;
#endif


#ifndef __TU_NOINLINE__
#  include "__termutil.h"
#endif



#endif  /* TERMUTIL_H */
