/**
 ******************************************************************************
 * @file    lc_print.h
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

#ifndef __LC_PRINT__
#define __LC_PRINT__

#include <stdarg.h>

#define LC_PRINT_FLOAT_SUPPORT  1

int vlc_print(void (*putc_fct)(void*, const char), void *user, const char* fmt, va_list vl);

#endif /* __LC_PRINT__ */
