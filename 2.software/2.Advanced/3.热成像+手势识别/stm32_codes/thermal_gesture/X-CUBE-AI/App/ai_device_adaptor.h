/**
 ******************************************************************************
 * @file    ai_device_adaptor.h
 * @author  MCD/AIS Team
 * @brief   Device port functions for DEVICE AI test application
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019,2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * Simple header file with helper macro/function to adapt the ai test applications.
 *  - before to include this file, _NO_DEVICE_PORT_FUNC can be defined to remove the
 *    port functions. Only ARM tool-chain and CMSIS-M-CORE related macros will be defined.
 *
 * History:
 *  - v1.0: initial version
 *  - v1.1: rework/clean port functions
 *
 *
 * ** Core functions/macros
 *
 *  cmsis_compiler.h              Compiler CMSIS Cortex-M file (implementation is Embedded ARM tool-chain dependent)
 *  _IS_XX_COMPILER               define the used Embedded ARM tool-chain (AC5/AC6, GCC, IAR)
 *  MEM_ALIGNED(x)                indicate the expected memory alignment (1,2,4,8,16,32 or 64 bytes)
 *
 * ** System/IO functions
 *
 *  _NO_DEVICE_PORT_FUNC          if defined only the core functions/macros are defined/exported.
 *
 *  HAS_CRC_IP                    =1 indicates that a CRC IP is available
 *  HAS_DWT_CTRL                  =1 indicates that a DWT IP is available
 *  HAS_PMU_CTRL                  =1 indicates that a PMU IP is available
 *  HAS_SYS_TICK                  =1 indicates that a SYS_TICK is available
 *
 *  port_hal_get_hal_version()    return HAL/OSAL version if used (else 0)
 *  port_hal_get_dev_id()         return device ID
 *  port_hal_get_rev_id()         return revision ID
 *
 *  port_hal_crc_ip_init()        enable the CRC IP access if used by the library
 *
 *  port_hal_get_cpu_freq()       return main CPU/MCU frequency (Hz)
 *  port_hal_get_sys_freq()       return main sys-bus frequency (Hz)
 *
 *  port_hal_delay(x)             wait x milliseconds
 *  port_hal_get_tick()           return number of tick (1 tick = 1ms)
 *
 *  port_dwt_init()               initialize the DWT MCU clock counter
 *  port_dwt_reset()              reset the DWT MCU clock counter
 *  port_dwt_get_cycles()         return the current DWT MCU clock counter
 *
 *
 *  HAS_DEDICATED_PRINT_PORT      if defined and equal to 1, indicate that a dedicated COM port is available
 *                                for the log services.
 *  USE_PRINTF_FROM_TOOL          if defined, indicate that the LC_PRINT(..) function is mapped on the default
 *                                printf(..) implementation from the SDK/tool-chain else low-cost implementation is
 *                                used (lc_print())
 *
 *  USE_RETARGET_IO_FROM_TOOL     if defined (and ==1) indicates that the retarget-io feature is natively
 *                                supported by the SDK/tool-chain (not provided by the aiValidation code, see aiTestUtility.c)
 *
 *  Notes:
 *    - aiTestUtility.c implements the Low-level character I/O (output only) function for the printf() function.
 *      It is mapped on the serial port (port_io_write() fct). This serial port is also used to exchange
 *      the binary messages (nanopb format, aiValidation app) with the HOST. When the AI COM stack is initialized,
 *      characters are no more forwarded on this serial port, they are skipped (_ioWriteAllowed=false). If the default printf()
 *      support is always requested, a dedicated serial port can be defined (port_io_dedicated_write/port_io_dedicated_putc).
 *    - setting of the serial ports should be done by the main application code.
 *
 *  port_io_dedicated_putc(b)     write a byte to the dedicated COM port (blocking mode)
 *  port_io_dedicated_write(&b,n) write n byte(s) to the dedicated COM port (blocking mode)
 *
 *  port_io_get(&d, timeout)      wait byte from the default COM port
 *  port_io_write(&b, n)          write n byte(s) to the default COM port (blocking mode)
 *  port_io_read(&b, n)           read n byte(s) from the default COM port (blocking) mode
 */

#ifndef __AI_DEVICE_ADAPTOR_H__
#define __AI_DEVICE_ADAPTOR_H__

#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


/* -----------------------------------------------------------------------------
 * MACRO - ARM tool chain definition
 * -----------------------------------------------------------------------------
 */

#undef _IS_AC5_COMPILER
#undef _IS_AC6_COMPILER
#undef _IS_GHS_COMPILER
#undef _IS_HTC_COMPILER
#undef _IS_GCC_COMPILER
#undef _IS_IAR_COMPILER

/* ARM Compiler 5 tool-chain */
#if defined ( __CC_ARM )
// #if ((__ARMCC_VERSION >= 5000000) && (__ARMCC_VERSION < 6000000))
#define _IS_AC5_COMPILER    1

/* ARM Compiler 6 tool-chain */
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define _IS_AC6_COMPILER    1

/* GHS tool-chain */
#elif defined(__ghs__)
#define _IS_GHS_COMPILER    1

/* HIGHTEC tool-chain */
#elif defined(__clang__)
#define _IS_HTC_COMPILER    1

/* GCC tool-chain */
#elif defined ( __GNUC__ )
#define _IS_GCC_COMPILER    1

/* IAR tool-chain */
#elif defined ( __ICCARM__ )
#define _IS_IAR_COMPILER    1

#else
#error ARM MCU tool chain is not detected/supported
#endif

#undef _IS_ACx_COMPILER
#if defined(_IS_AC5_COMPILER) && _IS_AC5_COMPILER   \
    || defined(_IS_AC6_COMPILER) && _IS_AC6_COMPILER
#define _IS_ACx_COMPILER    1
#endif


/* -----------------------------------------------------------------------------
 * MACRO - MEM align definition
 * -----------------------------------------------------------------------------
 */

#define _CONCAT_ARG(a, b)     a ## b
#define _CONCAT(a, b)         _CONCAT_ARG(a, b)

#if defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
  #define MEM_ALIGNED(x)         _CONCAT(MEM_ALIGNED_,x)
  #define MEM_ALIGNED_1          _Pragma("data_alignment = 1")
  #define MEM_ALIGNED_2          _Pragma("data_alignment = 2")
  #define MEM_ALIGNED_4          _Pragma("data_alignment = 4")
  #define MEM_ALIGNED_8          _Pragma("data_alignment = 8")
  #define MEM_ALIGNED_16         _Pragma("data_alignment = 16")
  #define MEM_ALIGNED_32         _Pragma("data_alignment = 32")
  #define MEM_ALIGNED_64         _Pragma("data_alignment = 64")
#elif defined(_IS_ACx_COMPILER) && _IS_ACx_COMPILER
  #define MEM_ALIGNED(x)         __attribute__((aligned (x)))
#elif defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
  #define MEM_ALIGNED(x)         __attribute__((aligned(x)))
#else
  #define MEM_ALIGNED(x)
#endif


#if !defined(_NO_DEVICE_PORT_FUNC)

#include <stdint.h>
#include <stdbool.h>

#if defined(USE_PRINTF_FROM_TOOL)
// Legacy definition
#if USE_PRINTF_FROM_TOOL != 0
#undef USE_PRINTF_FROM_TOOL
#define USE_PRINTF_FROM_TOOL 1
#endif
#endif


/* -----------------------------------------------------------------------------
 * System/IO functions
 * -----------------------------------------------------------------------------
 */

#if (defined(CORSTONE_300) || defined(ARMCM55)) && !defined(STM32N6)

#define DEVICE_FAMILY "CORSTONE_300"

/* --------------------------------------------
 * CORSTONE 300 / FVP environment
 * --------------------------------------------
 */

// #include <bsp_ai.h>  /* generated STM32 platform file to import the HAL and the UART definition */
#include "uart.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define port_hal_get_hal_version()        0
#define port_hal_get_dev_id()             0x155
#define port_hal_get_rev_id()             0

#define port_hal_get_cpu_freq()           25000000
#define port_hal_get_sys_freq()           25000000

#define port_hal_delay(delay_)
#define port_hal_get_tick()               1

#define HAS_CRC_IP                        0

#define HAS_DWT_CTRL                      0
#define HAS_PMU_CTRL                      1
#define HAS_SYS_TICK                      0

#define port_dwt_init()                   port_dwt_init_imp()
#define port_dwt_reset()                  ARM_PMU_CYCCNT_Reset()
#define port_dwt_get_cycles()             ARM_PMU_Get_CCNTR()

#define HAS_DEDICATED_PRINT_PORT          1

__STATIC_INLINE void port_io_dedicated_putc(unsigned char ch)
{
  uart1_putc(ch);
}

__STATIC_INLINE bool port_io_dedicated_write(uint8_t *buff, int count)
{
  for (int i=0; i<count; i++)
    uart1_putc(*buff++);

  return true;
}

__STATIC_INLINE bool port_io_get(uint8_t *c, uint32_t timeout)
{
  *c = uart_getc();
	return 1;
}

__STATIC_INLINE bool port_io_write(uint8_t *buff, int count)
{
  for (int i=0; i<count; i++)
    uart_putc(*buff++);

  return true;
}

__STATIC_INLINE bool port_io_read(uint8_t *buff, int count)
{
  for (int i=0; i<count; i++)
    *buff++ = uart_getc();

  return true;
}

#elif defined(STM32N657xx) || defined(STM32N6)

#define DEVICE_FAMILY "STM32"

/* --------------------------------------------
 * STM32N6xx series
 * --------------------------------------------
 */

#include <bsp_ai.h>  /* generated STM32 platform file to import the HAL and the UART definition */
   
#define port_hal_get_hal_version()        HAL_GetHalVersion()
#define port_hal_get_dev_id()             0x486
#define port_hal_get_rev_id()             0

#define port_hal_get_cpu_freq()           HAL_RCC_GetCpuClockFreq()
#define port_hal_get_sys_freq()           HAL_RCC_GetSysClockFreq()

#define port_hal_delay(delay_)            HAL_Delay(delay_)
#define port_hal_get_tick()               HAL_GetTick()

#define HAS_EXTRA_CONF                    4

#define HAS_CRC_IP                        0

#define HAS_DWT_CTRL                      0
#define HAS_PMU_CTRL                      1
#define HAS_SYS_TICK                      1

#define port_dwt_init()                   port_dwt_init_imp()
#define port_dwt_reset()                  ARM_PMU_CYCCNT_Reset()
#define port_dwt_get_cycles()             ARM_PMU_Get_CCNTR()


__STATIC_INLINE bool port_io_get(uint8_t *c, uint32_t timeout)
{ 
  return (uart_read(c, 1, timeout) == 1);
}

__STATIC_INLINE bool port_io_write(uint8_t *buff, int count)
{
  int res = uart_write(buff, count);

  return (res == count);
}

__STATIC_INLINE bool port_io_read(uint8_t *buff, int count)
{
  int res = uart_read(buff, count, 0);

  return (res == count);
}

#elif defined(SR5E1)

/* --------------------------------------------
 * STELLAR-E device
 * --------------------------------------------
 */

#define DEVICE_FAMILY "STELLAR-E"

#include "sr5e1_ai_validate.h"

#ifndef UNUSED
#define UNUSED(x)                         ((void)(x))
#endif

#define port_hal_get_hal_version()        0
#define port_hal_get_dev_id()             SR5E1_GetDEVID()
#define port_hal_get_rev_id()             SR5E1_GetREVID()

#define port_hal_get_cpu_freq()           SR5E1_GetSysClockDivFreq()
#define port_hal_get_sys_freq()           SR5E1_GetSysClockFreq()

#define port_hal_delay(delay_)            osal_delay_millisec(delay_)
#define port_hal_get_tick()               osal_get_tick()

#define USE_RETARGET_IO_FROM_TOOL         1
#define USE_PRINTF_FROM_TOOL              1
#define HAS_DEDICATED_PRINT_PORT          1


#define HAS_CRC_IP                        0
#define HAS_DWT_CTRL                      1
#define HAS_SYS_TICK                      1

#define NO_PORT_DWT_INIT_IMP              1  /* specific function is used, provided by the OSAL */
#define port_dwt_init()                   dwt_init()
#define port_dwt_reset()                  dwt_reset()
#define port_dwt_get_cycles()             dwt_get_ticks()


#define _MAX_DELAY                        10000UL

__STATIC_INLINE bool port_io_get(uint8_t *c, uint32_t timeout)
{
  if (!c)
    return false;

  int status = SR5E1_UART_Receive(c, 1, timeout);

  if (status == 2U) /* TIMEOUT */
    return false;

  return (status == SR5E1_UART_OK);
}

__STATIC_INLINE bool port_io_write(uint8_t *buff, int count)
{
  return (SR5E1_UART_Transmit(buff, count, _MAX_DELAY) == SR5E1_UART_OK);
}

__STATIC_INLINE bool port_io_read(uint8_t *buff, int count)
{
  return (SR5E1_UART_Receive(buff, count, _MAX_DELAY) == SR5E1_UART_OK);
}

#else

#define DEVICE_FAMILY "STM32"

/* --------------------------------------------
 * Default STM32 series implementation.
 *  All requested HAL functions are available.
 * --------------------------------------------
 */

#include <bsp_ai.h>  /* generated STM32 platform file to import the HAL and the UART definition */

#if defined(CHECK_STM32_FAMILY)
#if !defined(STM32F7) && !defined(STM32L4) && !defined(STM32L5) && !defined(STM32F4) && !defined(STM32H7) && !defined(STM32F3) && !defined(STM32U5) && !defined(STM32H5) && !defined(STM32G0) && !defined(STM32F0) && !defined(STM32C0)
#error Only STM32H7, STM32F7, STM32F4, STM32L4, STM32L5, STM32U5, STM32H5, STM32G0, STM32F0, STM32C0 or STM32F3 device are supported
#endif
#endif

#define port_hal_get_hal_version()        HAL_GetHalVersion()      
#define port_hal_get_dev_id()             HAL_GetDEVID() 
#define port_hal_get_rev_id()             HAL_GetREVID() 

#if defined(STM32MP1)
#define port_hal_get_cpu_freq()           HAL_RCC_GetSystemCoreClockFreq()
#elif !defined(STM32H7)
#define port_hal_get_cpu_freq()           HAL_RCC_GetHCLKFreq()
#else
#define port_hal_get_cpu_freq()           HAL_RCC_GetSysClockFreq()
#endif

#if defined(STM32MP1)
#define port_hal_get_sys_freq()           HAL_RCC_GetSystemCoreClockFreq()
#else
#define port_hal_get_sys_freq()           HAL_RCC_GetHCLKFreq()
#endif

#define port_hal_delay(delay_)            HAL_Delay(delay_)
#define port_hal_get_tick()               HAL_GetTick()

#define USE_RETARGET_IO_FROM_TOOL         0

#define HAS_CRC_IP                        1

#define HAS_PMU_CTRL                      0
#if defined(STM32G0) || defined(STM32F0) || defined(STM32L0) || defined(STM32C0)
#define HAS_DWT_CTRL                      0
#else
#define HAS_DWT_CTRL                      1
#endif
#define HAS_SYS_TICK                      1

#define port_dwt_init()                   port_dwt_init_imp()
#define port_dwt_reset()                  DWT->CYCCNT = 0
#define port_dwt_get_cycles()             DWT->CYCCNT
    
extern UART_HandleTypeDef UartHandle;

__STATIC_INLINE bool port_io_get(uint8_t *c, uint32_t timeout)
{
  HAL_StatusTypeDef status;

  if (!c)
    return false;

  status = HAL_UART_Receive(&UartHandle, (uint8_t *)c, 1, timeout);

  if (status == HAL_TIMEOUT)
    return false;

  return (status == HAL_OK);
}

__STATIC_INLINE bool port_io_write(uint8_t *buff, int count)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Transmit(&UartHandle, buff, count, HAL_MAX_DELAY);

  return (status == HAL_OK);
}

__STATIC_INLINE bool port_io_read(uint8_t *buff, int count)
{
  HAL_StatusTypeDef status;

  status = HAL_UART_Receive(&UartHandle, buff, count, HAL_MAX_DELAY);

  return (status == HAL_OK);
}

#endif /* other STM32xx series */


#if defined(HAS_CRC_IP) && HAS_CRC_IP == 1
void port_hal_crc_ip_init_imp(void);
#define port_hal_crc_ip_init()      port_hal_crc_ip_init_imp()
#else
#define port_hal_crc_ip_init()
#endif


void port_dwt_init_imp(void);
void port_hal_set_extra_conf(uint32_t *extra);


/* -----------------------------------------------------------------------------
 * Set LC_PRINT(.) definition
 * -----------------------------------------------------------------------------
 */

#if !defined(USE_PRINTF_FROM_TOOL) || USE_PRINTF_FROM_TOOL != 1

#include "lc_print.h"
#include <stdarg.h>

extern void lc_print(const char* fmt, ... );
extern size_t lc_vsnprint(char *buff, size_t size, const char* fmt, va_list vlist);

#define LC_PRINT(fmt, ...)                    lc_print(fmt, ##__VA_ARGS__)
#define LC_VSNPRINT(buff, size, fmt, va_list) lc_vsnprint(buff, size, fmt, va_list)

#else /* !USE_PRINTF_FROM_TOOL */

#include <stdio.h>

#define LC_PRINT(fmt, ...)                    printf(fmt, ##__VA_ARGS__)
#define LC_VSNPRINT(buff, size, fmt, va_list) vsnprintf(buff, size, fmt, va_list)

#endif /* USE_PRINTF_FROM_TOOL */


#endif /* !_NO_DEVICE_PORT_FUNC */

#ifdef __cplusplus
}
#endif

#endif /* __AI_DEVICE_ADAPTOR_H__  */
