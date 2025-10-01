#ifndef PORTABLE_H
#define PORTABLE_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "util.h"

#ifdef _WIN32
  #include <windows.h>
  #include <io.h>
  #define write(fd, buf, nbyte) _write(fd, buf, nbyte)
  #define read(fd, buf, nbyte) _read(fd, buf, nbyte)
  #define ftruncate(fd, len) ftruncate_win(fd, len)
#else
  #include <unistd.h>
  #define write(fd, buf, nbyte) write(fd, buf, nbyte)
  #define read(fd, buf, nbyte) read(fd, buf, nbyte)
#endif /*_WIN32*/

#ifdef _WIN32
static inline int ftruncate_win(int fd, long long length) {
    HANDLE h = (HANDLE)_get_osfhandle(fd);
    if (h == INVALID_HANDLE_VALUE) {
        return -1;
    }

    LARGE_INTEGER li;
    li.QuadPart = length;

    if (SetFilePointerEx(h, li, NULL, FILE_BEGIN) == 0) {
        return -1;
    }
    if (SetEndOfFile(h) == 0) {
        return -1;
    }
    return 0;
}
#endif /*_WIN32*/

#ifdef EXPERIMENTAL_p
static inline i64_t getline_p(char **lineptr, i64_t *n, FILE *fp) {
  if (!lineptr || !n || !fp) {
    errno = EINVAL;
    return -1;
  }

  char *buf = *lineptr;
  i64_t cap = *n;

  if (buf == NULL || cap == 0) {
    cap = 128;
    buf = malloc(cap);
    if (!buf) {
      errno = ENOMEM;
      return -1;
    }
  }

  i64_t len = 0;

  for (;;) {
    int ch = fgetc(fp);
    if (ch == EOF) {
      if (ferror(fp) || len == 0) {
        free(buf);
        *lineptr = NULL;
        return -1;
      }
      break;
    }
    if (ch == '\r') {
      // Peek one more to detect CRLF. Handle errors carefully.
      int next = fgetc(fp);
      if (next == EOF) {
        if (ferror(fp)) {
          // Read error while peeking; report failure.
          return -1;
        }
        // EOF after CR -> treat CR as newline and finish line.
        ch = '\n';
      } else {
        if (next != '\n') {
          // Put it back if it's not LF. ungetc can fail only on error or EOF,
          // but here next != EOF, so check its return value just in case.
          if (ungetc(next, fp) == EOF) {
            // Stream is in a bad state; report error.
            return -1;
          }
        }
        ch = '\n';
      }
    }
    //if (ch == '\r') {
    //  int next = fgetc(fp);
    //  if (next != '\n' && next != EOF) {
    //    ungetc(next, fp);
    //  }
    //  ch = '\n';
    //}
    if (len + 1 >= cap) {
      i64_t newcap = (cap < 256)? (cap * 2) : (cap + cap / 2);
      char *newbuf = realloc(buf, newcap);
      if (!newbuf) {
        errno = ENOMEM;
        buf[len] = '\0';
        *lineptr = buf;
        *n = cap;
        return -1;
      }
      buf = newbuf;
      cap = newcap;
    }

    buf[len++] = (char)ch;
    if (ch == '\n') break;
  }

  buf[len] = '\0';
  *lineptr = buf;
  *n = cap;
  return len;
}
#endif /*EXPERIMENTAL_p*/
#endif /*PORTABLE_H*/