#ifndef __TERMUTIL_H
#define __TERMUTIL_H


/*!
 * @brief Get terminal size
 * @return Terminal size
 */
__TU_STATIC__ __TU_INLINE__ TuTermSize
tu_get_termsize(void)
{
  TuTermSize tts;
#if defined(__TU_USE_CURSES__)
  getmaxyx(stdscr, tts.row, tts.col);
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi);
  tts.row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  tts.col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
  struct winsize termSize;
  ioctl(fileno(stdout), TIOCGWINSZ, &termSize);
  tts.row = termSize.ws_row;
  tts.col = termSize.ws_col;
#endif
  return tts;
}


/*!
 * @brief Set foreground color and background color
 * @param [in] fc  Foreground color
 * @param [in] bc  Background color
 */
__TU_STATIC__ __TU_INLINE__ void
tu_set_color(TuColor fc, TuColor bc)
{
#if defined(__TU_USE_CURSES__)
  extern int tu_current_color;
  if (fc == TU_DEFAULT_COLOR && bc == TU_DEFAULT_COLOR) {
    tu_current_color = 0;
  } else if (fc == TU_DEFAULT_COLOR) {
    tu_current_color = (((bc << 3) & 0x38) | (TU_DEFAULT_FOREGROUND & 0x07));
  } else if (bc == TU_DEFAULT_COLOR) {
    tu_current_color = (((TU_DEFAULT_BACKGROUND << 3) & 0x38) | fc);
  } else {
    tu_current_color = (((bc << 3) & 0x38) | (fc & 0x07));
  }
  attron(COLOR_PAIR(tu_current_color + 1));
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  extern CONSOLE_SCREEN_BUFFER_INFO tu_init_state;
  extern struct __TuAttribute tu_attribute;
  extern WORD tu_current_color;
  if (fc == TU_DEFAULT_COLOR && bc == TU_DEFAULT_COLOR) {
    tu_current_color = tu_init_state.wAttributes;
  } else if (fc == TU_DEFAULT_COLOR) {
    tu_current_color = (((bc << 4) & 0xf0) | (tu_init_state.wAttributes & 0x0f));
  } else if (bc == TU_DEFAULT_COLOR) {
    tu_current_color = (WORD) ((tu_init_state.wAttributes & 0xf0) | fc);
  } else {
    tu_current_color = (WORD) (((bc << 4) & 0xf0) | fc);
  }
  if (tu_attribute.is_reverse) {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        ((tu_current_color << 4) & 0xf0) | (tu_current_color >> 4));
  } else {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        tu_current_color);
  }
#else
  assert((TU_BLACK <= fc && fc < TU_GRAY) || fc == TU_DEFAULT_COLOR);
  assert((TU_BLACK <= bc && bc < TU_GRAY) || bc == TU_DEFAULT_COLOR);
  printf("\x1b[%d;%dm", fc, bc + 10);
#endif
}


/*!
 * @brief Set foreground color
 * @param [in] color  Foreground color
 */
__TU_STATIC__ __TU_INLINE__ void
tu_set_foreground(TuColor color)
{
#if defined(__TU_USE_CURSES__)
  extern int tu_current_color;
  if (color == TU_DEFAULT_COLOR) {
    tu_current_color = ((tu_current_color & 0x38) | (TU_DEFAULT_FOREGROUND & 0x07));
  } else {
    tu_current_color = ((tu_current_color & 0x38) | (color & 0x07));
  }
  attron(COLOR_PAIR(tu_current_color + 1));
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  extern CONSOLE_SCREEN_BUFFER_INFO tu_init_state;
  extern struct __TuAttribute tu_attribute;
  extern WORD tu_current_color;
  assert(-1 <= color && color < 0xf0);
  if (color == TU_DEFAULT_COLOR) {
    tu_current_color = ((tu_current_color & 0xf0) | (tu_init_state.wAttributes & 0x0f));
  } else {
    tu_current_color = (WORD) ((tu_current_color & 0xf0) | color);
  }
  if (tu_attribute.is_reverse) {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        ((tu_current_color << 4) & 0xf0) | (tu_current_color >> 4));
  } else {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        tu_current_color);
  }
#else
  assert((TU_BLACK <= color && color < TU_GRAY) || color == TU_DEFAULT_COLOR);
  printf("\x1b[%dm", color);
#endif
}


/*!
 * @brief Set background color
 * @param [in] color  Background color
 */
__TU_STATIC__ __TU_INLINE__ void
tu_set_background(TuColor color)
{
#if defined(__TU_USE_CURSES__)
  extern int tu_current_color;
  if (color == TU_DEFAULT_COLOR) {
    tu_current_color = (((TU_DEFAULT_BACKGROUND << 3) & 0x38) | (tu_current_color & 0x07));
  } else {
    tu_current_color = (((color << 3) & 0x38) | (tu_current_color & 0x07));
  }
  attron(COLOR_PAIR(tu_current_color + 1));
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  extern CONSOLE_SCREEN_BUFFER_INFO tu_init_state;
  extern struct __TuAttribute tu_attribute;
  extern WORD tu_current_color;
  assert(-1 <= color && color < 0xf0);
  if (color == TU_DEFAULT_COLOR) {
    tu_current_color = ((tu_init_state.wAttributes & 0xf0) | (tu_current_color & 0x0f));
  } else {
    tu_current_color = (WORD) ((color << 4) | (tu_current_color & 0x0f));
  }
  if (tu_attribute.is_reverse) {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        ((tu_current_color << 4) & 0xf0) | (color & 0x0f));
  } else {
    SetConsoleTextAttribute(
        hConsoleStdOut,
        tu_current_color);
  }
#else
  assert(TU_BLACK <= color && color < TU_GRAY);
  printf("\x1b[%dm", color + 10);
#endif
}


/*!
 * @brief Set text attribute
 * @param [in] attr  Attribute value
 */
__TU_STATIC__ __TU_INLINE__ void
tu_set_attribute(TuAttr attr)
{
#if defined(__TU_USE_CURSES__)
  attron(attr);
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  extern struct __TuAttribute tu_attribute;
  extern WORD tu_current_color;
  WORD current_color = tu_current_color;
  switch (attr) {
    case TU_ATTR_BOLD:
      tu_attribute.is_bold = 1;
      SetConsoleTextAttribute(
          hConsoleStdOut,
          current_color | ((__TU_BOLD_MASK << 4) | __TU_BOLD_MASK));
      break;
    case TU_ATTR_REVERSE:
      tu_attribute.is_reverse = 1;
      SetConsoleTextAttribute(
          hConsoleStdOut,
          ((current_color << 4) & 0xf0) | (current_color >> 4));
      break;
    case TU_ATTR_DEFAULT:
      tu_attribute.is_bold = 0;
      tu_attribute.is_reverse = 0;
      break;
  }
#else
  printf("\x1b[%dm", attr);
#endif
}


/*!
 * @brief Move cursor in absolute coordinates
 * @param [in] y  Absolute Y-coordinate (0-based)
 * @param [in] x  Absolute X-coordinate (0-based)
 */
__TU_STATIC__ __TU_INLINE__ void
tu_move(int y, int x)
{
#if defined(__TU_USE_CURSES__)
  move(y, x);
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  COORD pos;
  pos.X = (SHORT) (x < 0 ? 0 : x);
  pos.Y = (SHORT) (y < 0 ? 0 : y);
  SetConsoleCursorPosition(hConsoleStdOut, pos);
#else
  printf("\x1b[%d;%dH", y + 1, x + 1);
#endif
}


/*!
 * @brief Move cursor in absolute coordinates
 * @param [in] y  Absolute Y-coordinate (1-based)
 * @param [in] x  Absolute X-coordinate (1-based)
 */
__TU_STATIC__ __TU_INLINE__ void
_tu_move(int y, int x)
{
#if defined(__TU_USE_CURSES__)
  move(y - 1, x - 1);
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  COORD pos;
  pos.X = (SHORT) (x < 1 ? 1 : x - 1);
  pos.Y = (SHORT) (y < 1 ? 1 : y - 1);
  SetConsoleCursorPosition(hConsoleStdOut, pos);
#else
  printf("\x1b[%d;%dH", y, x);
#endif
}


/*!
 * @brief Move cursor up
 * @param [in] delta_y  Amount of movement
 */
__TU_STATIC__ __TU_INLINE__ void
tu_move_up(int delta_y)
{
#if defined(__TU_USE_CURSES__)
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi)) {
    csbi.dwCursorPosition.Y -= (SHORT) delta_y;
    SetConsoleCursorPosition(hConsoleStdOut, csbi.dwCursorPosition);
  }
#else
  printf("\x1b[%dA", delta_y);
#endif
}


/*!
 * @brief Move cursor down
 * @param [in] delta_y  Amount of movement
 */
__TU_STATIC__ __TU_INLINE__ void
tu_move_down(int delta_y)
{
#if defined(__TU_USE_CURSES__)
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi)) {
    csbi.dwCursorPosition.Y += (SHORT) delta_y;
    SetConsoleCursorPosition(hConsoleStdOut, csbi.dwCursorPosition);
  }
#else
  printf("\x1b[%dB", delta_y);
#endif
}


/*!
 * @brief Move cursor right
 * @param [in] delta_y  Amount of movement
 */
__TU_STATIC__ __TU_INLINE__ void
tu_move_right(int delta_x)
{
#if defined(__TU_USE_CURSES__)
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi)) {
    csbi.dwCursorPosition.X += (SHORT) delta_x;
    SetConsoleCursorPosition(hConsoleStdOut, csbi.dwCursorPosition);
  }
#else
  printf("\x1b[%dC", delta_x);
#endif
}


/*!
 * @brief Move cursor left
 * @param [in] delta_y  Amount of movement
 */
__TU_STATIC__ __TU_INLINE__ void
tu_move_left(int delta_x)
{
#if defined(__TU_USE_CURSES__)
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi)) {
    csbi.dwCursorPosition.X -= (SHORT) delta_x;
    SetConsoleCursorPosition(hConsoleStdOut, csbi.dwCursorPosition);
  }
#else
  printf("\x1b[%dD", delta_x);
#endif
}


/*!
 * @brief Clear screen
 */
__TU_STATIC__ __TU_INLINE__ void
tu_clear(void)
{
#if defined(__TU_USE_CURSES__)
  clear();
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  extern CONSOLE_SCREEN_BUFFER_INFO tu_init_state;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwNumberOfCharsWritten;
  COORD coord = {0, 0};
  if (GetConsoleScreenBufferInfo(hConsoleStdOut, &csbi)) {
    FillConsoleOutputAttribute(
        hConsoleStdOut,
        tu_init_state.wAttributes,
        csbi.dwSize.X * csbi.dwSize.Y,
        coord,
        &dwNumberOfCharsWritten);
    FillConsoleOutputCharacter(
        hConsoleStdOut,
        ' ',
        csbi.dwSize.X * csbi.dwSize.Y,
        coord, &dwNumberOfCharsWritten);
  }
#else
  printf("\x1b[2J");
  tu_move(0, 0);
#endif
}


/*!
 * @brief Clear screen using system command
 */
__TU_STATIC__ __TU_INLINE__ void
_tu_clear(void)
{
#if defined(__TU_USE_CURSES__)
  clear();
#elif defined(_MSC_VER)
  system("cls");
#elif defined(__cygwin__)
  printf("\x1bc\x1b[3J");
#else
  system("clear");
#endif

}


/*!
 * @brief Clear right from cursor
 */
__TU_STATIC__ __TU_INLINE__ void
tu_clear_right(void)
{
  fputs("\x1b[0K", stdout);
}


/*!
 * @brief Clear left from cursor
 */
__TU_STATIC__ __TU_INLINE__ void
tu_clear_left(void)
{
  fputs("\x1b[1K", stdout);
}


/*!
 * @brief Clear the line where the cursor is
 */
__TU_STATIC__ __TU_INLINE__ void
tu_clear_line(void)
{
  fputs("\x1b[2K", stdout);
}


/*!
 * @brief Set foreground directly
 */
__TU_STATIC__ __TU_INLINE__ void
_tu_set_foreground(const char *color_str)
{
  fputs(color_str, stdout);
}


/*!
 * @brief Set background directly
 */
__TU_STATIC__ __TU_INLINE__ void
_tu_set_background(const char *color_str)
{
  fputs(color_str, stdout);
}


/*!
 * @brief Print character
 * @param [in] c  Character
 */
__TU_STATIC__ __TU_INLINE__ void
tu_addch(int c)
{
#ifdef __TU_USE_CURSES__
  addch(c);
#else
  putchar(c);
#endif
}


/*!
 * @brief Move cursor and print character
 * @param [in] y  Y-coordinate
 * @param [in] x  X-coordinate
 * @param [in] c  Character
 */
__TU_STATIC__ __TU_INLINE__ void
tu_mvaddch(int y, int x, int c)
{
#ifdef __TU_USE_CURSES__
  mvaddch(y, x, c);
#else
  tu_move(y, x);
  putchar(c);
#endif
}


/*!
 * @brief Print string
 * @param [in] str  String
 */
__TU_STATIC__ __TU_INLINE__ void
tu_addstr(const char *str)
{
#ifdef __TU_USE_CURSES__
  addstr(str);
#else
  fputs(str, stdout);
#endif
}


/*!
 * @brief Move cursor and print string
 * @param [in] y    Y-coordinate
 * @param [in] x    X-coordinate
 * @param [in] str  String
 */
__TU_STATIC__ __TU_INLINE__ void
tu_mvaddstr(int y, int x, const char *str)
{
#ifdef __TU_USE_CURSES__
  mvaddstr(y, x, str);
#else
  tu_move(y, x);
  fputs(str, stdout);
#endif
}


#if defined(__TU_USE_CURSES__)
#  define tu_printw    printw
#  define tu_mvprintw  mvprintw
#elif !defined(__TU_NOINLINE__)
#  define tu_printw(fmt, ...) \
  printf(fmt, ##__VA_ARGS__)
#  define tu_mvprintw(y, x, fmt, ...) \
  (tu_move(y, x), printf(fmt, ##__VA_ARGS__))
#endif


/*!
 * @brief Enable echo back when input characters
 */
__TU_STATIC__ __TU_INLINE__ void
tu_echo(void)
{
#if defined(__TU_USE_CURSES__)
  echo();
#elif !defined(_MSC_VER)
  struct termios tio;

  tcgetattr(STDIN_FILENO, &tio);
  tio.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
#endif
}


/*!
 * @brief Disable echo back when input characters
 */
__TU_STATIC__ __TU_INLINE__ void
tu_noecho(void)
{
#if defined(__TU_USE_CURSES__)
  noecho();
#elif !defined(_MSC_VER)
  struct termios tio;

  tcgetattr(STDIN_FILENO, &tio);
  tio.c_lflag &=  (~(unsigned int) ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
#endif
}


/*!
 * @brief Enable to input characters without detect return
 */
__TU_STATIC__ __TU_INLINE__ void
tu_cbreak(void)
{
#if defined(__TU_USE_CURSES__)
  cbreak();
#elif !defined(_MSC_VER)
  struct termios tio;

  tcgetattr(STDIN_FILENO, &tio);
  tio.c_lflag &= (~(unsigned int) ICANON);
  tio.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
#endif
}


/*!
 * @brief Disnable to input characters without detect return
 */
__TU_STATIC__ __TU_INLINE__ void
tu_nocbreak(void)
{
#if defined(__TU_USE_CURSES__)
  nocbreak();
#elif !defined(_MSC_VER)
  struct termios tio;

  tcgetattr(STDIN_FILENO, &tio);
  tio.c_lflag |= ICANON;
  tio.c_cc[VTIME] = tu_init_tio.c_cc[VTIME];
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
#endif
}


/*!
 * @brief Enable non-blocking input from stdin
 */
__TU_STATIC__ __TU_INLINE__ void
tu_nonblocking(void)
{
#if defined(__TU_USE_CURSES__)
  nodelay(stdscr, 1);
#elif defined(_MSC_VER)
  extern int is_nonblocking;
  is_nonblocking = 1;
#else
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
#endif
}


/*!
 * @brief Disable non-blocking input from stdin
 */
__TU_STATIC__ __TU_INLINE__ void
tu_blocking(void)
{
#if defined(__TU_USE_CURSES__)
  nodelay(stdscr, 0);
#elif defined(_MSC_VER)
  extern int is_nonblocking;
  is_nonblocking = 0;
#else
  int ret = fcntl(STDIN_FILENO, F_GETFL, 0);
  ret &= ~(ret & O_NONBLOCK);
  fcntl(STDIN_FILENO, F_SETFL, ret);
#endif
}


/*!
 * @brief Refresh screen
 */
__TU_STATIC__ __TU_INLINE__ void
tu_refresh(void)
{
#ifdef __TU_USE_CURSES__
  refresh();
#else
  fflush(stdout);
#endif
}


/*!
 * @brief Input character
 */
__TU_STATIC__ __TU_INLINE__ int
tu_getch(void)
{
#if defined(__TU_USE_CURSES__)
  return getch();
#elif defined(_MSC_VER)
  extern int is_nonblocking;
#  if _MSC_VER >= 1400
  return is_nonblocking ? (_kbhit() ? _getch() : EOF) : _getch();
#  else
  return is_nonblocking ? (kbhit() ? getch() : EOF) : getch();
#  endif
#else
  return getchar();
#endif
}


/*!
 * @brief Inform whether key is pushed or not
 * @return  Return 1 when key is pushed, otherwise return 0
 */
__TU_STATIC__ __TU_INLINE__ int
tu_kbhit(void)
{
#if defined(_MSC_VER)
#  if _MSC_VER >= 1400
  return _kbhit();
#  else
  return kbhit();
#  endif
#else
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= (unsigned int) (~(ICANON | ECHO));
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  return 0;
#endif
}


/*!
 * @brief Show/hide cursor
 * @param [in] is_show  Hide cursor if is_show is 0, otherwise show cursor
 */
__TU_STATIC__ __TU_INLINE__ void
tu_setcur(int is_show)
{
#if defined(__TU_USE_CURSES__)
  curs_set(is_show);
#elif defined(_MSC_VER)
  extern HANDLE hConsoleStdOut;
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(hConsoleStdOut, &cci);
  cci.bVisible = is_show ? 1 : 0;
  SetConsoleCursorInfo(hConsoleStdOut, &cci);
#else
  fputs(is_show ? "\x1b[>5l" : "\x1b[>5h", stdout);
#endif
}


/*!
 * @brief Sound a beep
 */
__TU_STATIC__ __TU_INLINE__ void
tu_beep(void)
{
#ifdef __TU_USE_CURSES__
  beep();
#else
  putchar('\a');
#endif
}


#endif  /* __TERMUTIL_H */
