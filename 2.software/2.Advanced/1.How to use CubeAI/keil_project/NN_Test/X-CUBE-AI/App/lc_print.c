/**
 ******************************************************************************
 * @file    lc_print.c
 * @author  MCD/AIS Team
 * @brief   Low cost print implementation
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019,2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * history
 *  v1.0 - initial version
 */

#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "lc_print.h"

#define FLAG_ZERO_EXTEND    (1 << 0)
#define FLAG_NEG_PAD        (1 << 1)
#define FLAG_IS_SIGNED      (1 << 2)
#define FLAG_CAPS           (1 << 3)
#define FLAG_HEX            (1 << 4)

struct specifier
{
  int    width;
  int    precision;
  int    flags;
  void (*putc_fct)(void*, const char);
  void * user;
};

#define _WRITE_C(_c) \
		specifier->putc_fct(specifier->user, _c); \
		count++;

static int _write_string(const char *str, struct specifier *specifier)
{
  int len = strlen(str);
  int count = 0;

  if (specifier->precision) {
    len = (len > specifier->precision)?specifier->precision:len;
  }

  if (!(specifier->flags & FLAG_NEG_PAD)) {
    for(int i = len; i < specifier->width; i++) {
      _WRITE_C(' ');
    }
  }

  for (int i = 0; i < len; i++) {
    _WRITE_C(*str++);
  }

  if (specifier->flags & FLAG_NEG_PAD) {
    for(int i = len; i < specifier->width; i++) {
      _WRITE_C(' ');
    }
  }

  return count;
}

static int _write_number(uint64_t number, struct specifier *specifier)
{
  char buf[64];
  uint32_t idx = sizeof(buf) - 1;
  int count = 0;
  char sign_neg = 0;
  char chr;

  buf[idx--] = 0;

  if (specifier->flags & FLAG_IS_SIGNED) {
    if ((int64_t)number < 0) {
      number = -number;
      sign_neg = '-';
      specifier->width -= 1;
    }
  }

  do {
    uint64_t t;
    if (specifier->flags & FLAG_HEX) {
      chr = number & 0x0F;
      t = number >> 4;
      chr = (chr >= 10) ? (chr + (specifier->flags & FLAG_CAPS ? 'A' : 'a') - 10) : (chr + '0');
    } else {
      t = number / 10;
      chr = (number - t * 10) + '0';
    }
    number = t;
    buf[idx--] = chr;
    count++;
  } while (number);

  if ((sign_neg) && (!(specifier->flags & FLAG_ZERO_EXTEND))) {
    buf[idx--] = sign_neg;
    count++;
  }

  while ((count < specifier->width) && (!(specifier->flags & FLAG_NEG_PAD))) {
    if (specifier->flags & FLAG_ZERO_EXTEND)
      buf[idx--] = '0';
    else
      buf[idx--] = ' ';
    count++;
  }

  if ((sign_neg) && (count < (specifier->width + 1))) {
    buf[idx--] = sign_neg;
    count++;
  }

  idx++;


  for(int i = 0; i < count; i++) {
    specifier->putc_fct(specifier->user, (buf + idx)[i]);
  }

  if (specifier->flags & FLAG_NEG_PAD) {
    while (count < specifier->width) {
      _WRITE_C(' ');
    }
  }

  return count;
}

#if defined(LC_PRINT_FLOAT_SUPPORT) && LC_PRINT_FLOAT_SUPPORT == 1
static inline int _write_double(double number, struct specifier *specifier)
{
  char buf[64];
  uint64_t val;
  uint32_t idx = sizeof(buf) - 1;
  uint32_t count = 0;
  char chr;
  bool is_signed = (number < 0)?true:false;
  const double half_ = 0.5;

  double mul = 1.0;
  for (int i=0;i <specifier->precision; i++)
    mul *= 10;

  if (is_signed)
    number = -number;

  number += (half_ / mul);
  val = (uint64_t)(number * mul);

  do {
    uint64_t t = val / 10;
    chr = (val - t * 10) + '0';
    val = t;
    buf[idx--] = chr;
    count++;
    if (count == specifier->precision) {
      buf[idx--] = '.';
      count++;
    }
  } while (val);
  while (count < specifier->precision) {
    buf[idx--] = '0';
    count++;
  }
  if (count == specifier->precision) {
    buf[idx--] = '.';
    count++;
  }
  if (count == specifier->precision + 1)
  {
    buf[idx--] = '0';
    count++;
  }
  if (is_signed) {
    buf[idx--] = '-';
    count++;
  }
  while (count < specifier->width) {
    buf[idx--] = ' ';
    count++;
  }
  idx++;

  for(int i = 0; i < count; i++) {
    specifier->putc_fct(specifier->user, (buf + idx)[i]);
  }

  return count;
}
#endif

static inline char _getChar(const char** fmtP)
{
  return *(*fmtP)++;
}

int vlc_print(void (*putc_fct)(void*, const char), void *user, const char* fmt, va_list vl)
{
  char c, t;
  int count = 0;
  bool error = false;

  while (((c = _getChar(&fmt)) != 0) && (!error))
  {
    if (c == '\n')
    {
      putc_fct(user, c);
      count++;
    }
    else if (c == '%')
    {
      bool done = false;
      bool has_precision = false;
      bool long_type = false;
      struct specifier specifier = { 0, 0, 0, putc_fct, user };

      while (done == false) {
        c = _getChar(&fmt);
        if (!c)
        { /* error */
          error = true;
          done = true;
        }
        else if (c == '0')
        { /* flags or width/precision */
          if (!specifier.width && !specifier.precision) {
            specifier.flags |= FLAG_ZERO_EXTEND;
          } else if (has_precision) {
            specifier.precision = specifier.precision * 10 + (c - '0');
          } else {
            specifier.width = specifier.width * 10 + (c - '0');
          }
        }
        else if (c == '-')
        {
          specifier.flags |= FLAG_NEG_PAD;
        }
        else if (c == '.')
        { /* set precision */
          has_precision = true;
        }
        else if ((c >= '1') && (c <= '9'))
        { /* width/precision */
          if (has_precision) {
            specifier.precision = specifier.precision * 10 + (c - '0');
          } else {
            specifier.width = specifier.width * 10 + (c - '0');
          }
        }
        else if (c == 'l')
        {
          long_type = true;
        }
        else
        { /* type */
          char* str;
          uint64_t val;
          double dbl;
          switch(c)
          {
          case '%':
            putc_fct(user, c);
            count++;
            break;
          case 'c':
            t = (char)va_arg(vl, signed int);
            putc_fct(user, t);
            count++;
            break;
          case 's':
            str = va_arg(vl, char*);
            if (!str) str = "(null)";
            count += _write_string(str, &specifier);
            break;
          case 'p':
            specifier.flags |= FLAG_ZERO_EXTEND;
            specifier.width = 8;
          case 'X':
            specifier.flags |= FLAG_CAPS;
          case 'x':
            specifier.flags |= FLAG_HEX;
            val = va_arg(vl, unsigned int);
            count += _write_number(val, &specifier);
            break;
          case 'd':
          case 'i':
            specifier.flags |= FLAG_IS_SIGNED;
          case 'u':
            if (c == 'u')
              val = va_arg(vl, unsigned int);
            else {
              if (long_type)
                val = va_arg(vl, signed long);
              else
                val = va_arg(vl, signed int);
            }
            count += _write_number(val, &specifier);
            break;
#if defined(LC_PRINT_FLOAT_SUPPORT) && LC_PRINT_FLOAT_SUPPORT == 1
          case 'f':
          case 'F':
            dbl = va_arg(vl, double);
            if (specifier.precision == 0)
              specifier.precision = 6;
            count += _write_double(dbl, &specifier);
            break;
#endif
          default:
            error=true;
          }
          done = true;
        }
      }
      if (error) {
        va_arg(vl, signed long);
        putc_fct(user, '<');
        putc_fct(user, '!');
        putc_fct(user, '>');
        count += 3;
        error = false;
      }
    }
    else
    {
      putc_fct(user, c);
      count++;
    }
  }
  return count;
}


