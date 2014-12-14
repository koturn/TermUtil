#include <stdlib.h>
#if defined(__TU_USE_CURSES__)
#  include <curses.h>
#elif defined(_MSC_VER)
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
#  include <termios.h>
#endif
#include "../include/termutil.h"


static void
tu_cleanup(void);

static void
sigint_handler(int sig);


#if defined(__TU_USE_CURSES__)
int tu_current_color = 0;
#elif defined(_MSC_VER)
struct __TuAttribute tu_attribute = {0, 0};
CONSOLE_SCREEN_BUFFER_INFO tu_init_state;
WORD tu_current_color;
HANDLE hConsoleStdOut;
static CONSOLE_CURSOR_INFO tu_init_cci;
#else
struct termios tu_init_tio;
#endif


/*!
 * @brief Initialize and save initial state
 */
void
tu_init(void)
{
#if defined(__TU_USE_CURSES__)
  int i, j;
  initscr();
  start_color();
  use_default_colors();
  clear();
  assume_default_colors(COLOR_BLACK, COLOR_WHITE);
  for (i = COLOR_BLACK; i <= COLOR_WHITE; i++) {
    for (j = COLOR_BLACK; j <= COLOR_WHITE; j++) {
      init_pair(((j << 3) | i) + 1, i, j);
    }
  }
  bkgd(COLOR_PAIR(0));
#elif defined(_MSC_VER)
  hConsoleStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsoleStdOut, &tu_init_state);
  GetConsoleCursorInfo(hConsoleStdOut, &tu_init_cci);
  tu_current_color = tu_init_state.wAttributes;
#else
  tcgetattr(STDIN_FILENO, &tu_init_tio);
#endif
  signal(SIGINT, sigint_handler);
  atexit(tu_cleanup);
}


/*!
 * @brief Restore initial state
 */
static void
tu_cleanup(void)
{
#if defined(__TU_USE_CURSES__)
  endwin();
#elif defined(_MSC_VER)
  SetConsoleTextAttribute(hConsoleStdOut, tu_init_state.wAttributes);
  SetConsoleCursorInfo(hConsoleStdOut, &tu_init_cci);
#else
  tcsetattr(STDIN_FILENO, TCSANOW, &tu_init_tio);
#endif
}


/*!
 * @brief Signal handler
 * @param [in] sig  signal
 */
static void
sigint_handler(int sig)
{
  tu_cleanup();
  exit(0);
}


#if defined(__TU_NOINLINE__) && !defined(__TU_USE_CURSES__)
#  include "../include/__termutil.h"
/*!
 * @brief Print formatted text
 * @param [in] fmt  Format string
 * @param [in] ...  Printf-arguments
 */
void
tu_printw(const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}


/*!
 * @brief Move cursor and  print formatted text
 *
 * @param [in] y    Y-coordinate
 * @param [in] x    X-coordinate
 * @param [in] fmt  Format string
 * @param [in] ...  Printf-arguments
 */
void
tu_mvprintw(int y, int x, const char *fmt, ...)
{
  va_list args;

  tu_move(y, x);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
#endif
