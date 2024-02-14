/**
 ******************************************************************************
 * @file    aiTestUtility.c
 * @author  MCD/AIS Team
 * @brief   Utility functions for AI test application
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
 * Description:
 *
 * History:
 *  - v1.0 - initial version (from initial aiSystemPerformance file - v5.1)
 *  - v1.1 - add Arm Compiler 6 support (MDK)
 *  - v1.2 - add io low level code to manage a COM through the STM32 USB CDC profile
 *           enabled with the USE_USB_CDC_CLASS = 1 define.
 *  - v1.3 - Fix compilation issue for H7 dual core
 *  - v2.0 - add Cortex-m0/m0plus support
 *  - v2.1 - add U575ZI id
 *  - v2.2 - clean the way to build/report the STM32 configuration
 *  - v3.1 - use ai_device_adaptor.h
 *
 */

/*
 *  NOTE about the USE_USB_CDC_CLASS support
 *
 *   When USE_USB_CDC_CLASS is set to 1 the code to manage the
 *   STM32 USB_DEVICE and the CDC class should added in the project.
 *
 *   CDC_Receive_FS()/CDC_Control_FS() functions (file: "usb_cdc_if.c")
 *   should be adapted/patched
 *
 *   o CDC_Receive_FS() to handle the received data
 *    ...
 *    static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len) {
 *      extern void ioPushInUserUsb(uint8_t *pw, uint32_t *len);
 *      ioPushInUserUsb(Buf, Len);
 *      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
 *      USBD_CDC_ReceivePacket(&hUsbDeviceFS);
 *      return (USBD_OK);
 *      }
 *    ...
 *
 *   o CDC_Control_FS() to return the valid data for CDC_GET_LINE_CODING
 *     request.This is requested by the Python module in charge of the
 *     Serial COM with the validation process.
 *
 *      ...
 *      case CDC_GET_LINE_CODING:
 *
 *        if (length == 7) {
 *        *(uint32_t *)pbuf = 115200;
 *        pbuf[4] = 0;
 *        pbuf[5] = 0;
 *        pbuf[6] = 8;
 *        }
 *
 *      break;
 *      ...
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "aiTestUtility.h"

#ifndef USE_USB_CDC_CLASS
#define USE_USB_CDC_CLASS 0
#endif


/* -----------------------------------------------------------------------------
 * Sanity check
 * -----------------------------------------------------------------------------
 */

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
#if defined(__ARMCC_VERSION)
#error GCC tool-chain is used, __ARMCC_VERSION should be not defined
#endif
#endif

#if defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
#if !defined(__ICCARM__)
#error IAR tool-chain is used, __ICCARM__ should be defined
#endif
#endif

#if defined(_IS_GHS_COMPILER) && _IS_GHS_COMPILER
#if !defined(__ghs__)
#error GHS tool-chain is used, __ghs__ should be defined
#endif
#endif

#if defined(_IS_HTC_COMPILER) && _IS_HTC_COMPILER
#if !defined(__clang__)
#error HIGHTEC tool-chain is used, __clang__ should be defined
#endif
#endif

/* -----------------------------------------------------------------------------
 * IO re-target functions
 * -----------------------------------------------------------------------------
 */

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
#include <errno.h>
#include <sys/unistd.h> /* STDOUT_FILENO, STDERR_FILENO */

#elif defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
#if (__IAR_SYSTEMS_ICC__ <= 8)
/* Temporary workaround - LowLevelIOInterface.h seems not available
   with IAR 7.80.4 */
#define _LLIO_STDIN  0
#define _LLIO_STDOUT 1
#define _LLIO_STDERR 2
#define _LLIO_ERROR ((size_t)-1) /* For __read and __write. */
#else
#include <LowLevelIOInterface.h> /* _LLIO_STDOUT, _LLIO_STDERR */
#endif

#elif defined(_IS_ACx_COMPILER) && _IS_ACx_COMPILER

#endif


static bool _ioWriteAllowed = true;

 int ioRawGetUint8(uint8_t *c, uint32_t timeout)
 {
   return port_io_get(c, timeout) == true?1:-1;
 }

#if defined(USE_USB_CDC_CLASS) && USE_USB_CDC_CLASS == 1

#include "usbd_cdc_if.h"

#define _MAX_USB_USER_ELEM (128)

uint8_t _usb_fifo[_MAX_USB_USER_ELEM];
volatile uint32_t _usb_nb_w_item = 0;
volatile uint32_t _usb_nb_r_item = 0;
uint8_t *_usb_p_read = &_usb_fifo[0];
uint8_t *_usb_p_write = &_usb_fifo[0];

void ioPushInUserUsb(uint8_t *pw, uint32_t *len)
{
  uint8_t *pr = pw;
  for (int i=0; i < *len; i++) {
    *_usb_p_write++ = *pr++;
    if (_usb_p_write == &_usb_fifo[_MAX_USB_USER_ELEM])
      _usb_p_write = &_usb_fifo[0];
    _usb_nb_w_item++;
  }
}

bool ioRawWriteBuffer(uint8_t *buff, int count)
{
  HAL_StatusTypeDef status = HAL_OK;
  while (USBD_BUSY == CDC_Transmit_FS(buff, count));
  // CDC_Transmit_FS(buff, count);
  return (status == HAL_OK);
}

bool ioRawReadBuffer(uint8_t *buff, int count)
{
  HAL_StatusTypeDef status = HAL_OK;
  while ((_usb_nb_w_item - _usb_nb_r_item) < count) {};

  uint8_t *pw = buff;
  for (int i=0; i < count; i++) {
    *pw++ = *_usb_p_read++;
    if (_usb_p_read == &_usb_fifo[_MAX_USB_USER_ELEM])
      _usb_p_read = &_usb_fifo[0];
    _usb_nb_r_item++;
  }
  return (status == HAL_OK);
}

void ioRawDisableLLWrite(void)
{
}

#else /* defined(USE_USB_CDC_CLASS) */

bool ioRawWriteBuffer(uint8_t *buff, int count)
{
  return port_io_write(buff, count);
}

bool ioRawReadBuffer(uint8_t *buff, int count)
{
  return port_io_read(buff, count);
}

void ioRawDisableLLWrite(void)
{
  _ioWriteAllowed = false;
}

#endif /* !defined(USE_USB_CDC_CLASS) */

#if !defined(USE_RETARGET_IO_FROM_TOOL) || USE_RETARGET_IO_FROM_TOOL != 1

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER

int _write(int fd, const void *buff, int count);

int _write(int fd, const void *buff, int count)
{
  bool status = true;

  if ((count < 0) && (fd != STDOUT_FILENO) && (fd != STDERR_FILENO)) {
    errno = EBADF;
    return -1;
  }

#if defined(HAS_DEDICATED_PRINT_PORT) && HAS_DEDICATED_PRINT_PORT == 1
  status = port_io_dedicated_write((uint8_t *)buff, count);
#else /* HAS_DEDICATED_PRINT_PORT */
  if (_ioWriteAllowed)
#ifdef SWO_OUTPUT
  {
    const uint8_t *p8_buff = (uint8_t *) buff;
    for(int i_c = 0; i_c < count; i_c++)
    {
      const uint8_t ui8_char = *p8_buff++;
      /* Filter out Escape sequences (No color on Debug terminal */
      if (ui8_char != 27)
      {
        ITM_SendChar(ui8_char);
      }
      else
      {
        p8_buff+=4;
        i_c +=4;
      }
    }
    status = count;
  }
#endif /* SWO_OUTPUT */
  status = port_io_write((uint8_t *)buff, count);
#endif /* !HAS_DEDICATED_PRINT_PORT */

  return (status ? count : 0);
}

#elif defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER

__ATTRIBUTES  size_t __write(int handle, const unsigned char *buffer,
    size_t size);

__ATTRIBUTES  size_t __write(int handle, const unsigned char *buffer,
    size_t size)
{
  bool status = true;

  /*
   * This means that we should flush internal buffers.  Since we
   * don't we just return.  (Remember, "handle" == -1 means that all
   * handles should be flushed.)
   */
  if (buffer == 0)
    return 0;

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure.
   */
  if ((handle != _LLIO_STDOUT) && (handle != _LLIO_STDERR))
    return _LLIO_ERROR;

#if defined(HAS_DEDICATED_PRINT_PORT) && HAS_DEDICATED_PRINT_PORT == 1
  status = port_io_dedicated_write((uint8_t *)buffer, size);
#else
  if (_ioWriteAllowed)
    status = port_io_write((uint8_t *)buffer, size);
#endif
  return (status ? size : _LLIO_ERROR);
}

#elif defined(_IS_ACx_COMPILER) && _IS_ACx_COMPILER

int fputc(int ch, FILE *f)
{
#if defined(HAS_DEDICATED_PRINT_PORT) && HAS_DEDICATED_PRINT_PORT == 1
  port_io_dedicated_putc(ch);
#else
  if (_ioWriteAllowed)
    port_io_write((uint8_t *)&ch, 1);
#endif
  return ch;
}

#else
#error ARM MCU tool-chain is not supported.
#endif

#endif /* !USE_RETARGET_IO_FROM_TOOL */

/* -----------------------------------------------------------------------------
 * Log functions
 * -----------------------------------------------------------------------------
 */

#if !defined(USE_PRINTF_FROM_TOOL) || USE_PRINTF_FROM_TOOL != 1

#include "lc_print.h"

static void _put_c_driver(void *user, const char c)
{
#if defined(HAS_DEDICATED_PRINT_PORT) && HAS_DEDICATED_PRINT_PORT == 1
  port_io_dedicated_putc(c);
#else
  if (_ioWriteAllowed)
  {
    port_io_write((uint8_t *)&c, 1);
  }
#ifdef SWO_OUTPUT
  ITM_SendChar(c);
#endif 
#endif
}

void lc_print(const char* fmt, ... )
{
  va_list ap;
  va_start(ap, fmt);
  vlc_print(&_put_c_driver, NULL, fmt, ap);
  va_end(ap);
}

struct _buffer {
  char *buffer;
  size_t size;
  size_t pos;
};

static void _put_c_buffer_cb(void *user, const char c)
{
  struct _buffer *buffer = (struct _buffer *)user;
  if (buffer->pos < (buffer->size - 1)) {
    buffer->buffer[buffer->pos] = c;
    buffer->pos++;
    buffer->buffer[buffer->pos] = 0;
  }
}

size_t lc_vsnprint(char *buff, size_t size, const char* fmt, va_list vlist)
{
  struct _buffer buffer = {buff, size, 0};
  vlc_print(&_put_c_buffer_cb, &buffer, fmt, vlist);
  return buffer.pos - 1;
}

#endif /* !USE_PRINTF_FROM_TOOL */


/* -----------------------------------------------------------------------------
 * HEAP Monitor functions
 * -----------------------------------------------------------------------------
 */

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER

#define MAGIC_MALLOC_NUMBER 0xefdcba98

struct io_malloc io_malloc;

void* __real_malloc(size_t bytes);
void __real_free(void *ptr);

void* __wrap_malloc(size_t bytes)
{
  uint8_t *ptr;

  io_malloc.cfg |= 1 << 1;

  /* ensure alignment for magic number */
  bytes = (bytes + 3) & ~3;

  /* add 2x32-bit for size and magic  number */
  ptr = (uint8_t*)__real_malloc(bytes + 8);

  /* remember size */
  if (ptr) {
    *((uint32_t*)ptr) = bytes;
    *((uint32_t*)(ptr + 4 + bytes)) = MAGIC_MALLOC_NUMBER;
  }

  if ((ptr) && (io_malloc.cfg & 1UL)) {
    io_malloc.alloc_req++;
    io_malloc.alloc += bytes;

    io_malloc.used += bytes;

    if (io_malloc.used > io_malloc.max) {
      io_malloc.max = io_malloc.used;
    }

#if _IO_MALLOC_TRACK_MODE == 1
    io_malloc.a_ptr[io_malloc.a_idx] = (ptr + 4);
    io_malloc.a_s[io_malloc.a_idx] = bytes;
    io_malloc.a_idx++;

    if (io_malloc.a_idx >= _IO_MALLOC_TRACK_MODE)
      io_malloc.a_idx = 0;
#endif
  }
  return ptr?(ptr + 4):NULL;
}

void __wrap_free(void *ptr)
{
  uint8_t* p;
  uint32_t bytes;

  io_malloc.cfg |= 1 << 2;

  if (!ptr)
    return;

  p = (uint8_t*)ptr - 4;
  bytes = *((uint32_t*)p);

  if (*((uint32_t*)(p + 4 + bytes)) == MAGIC_MALLOC_NUMBER) {
    *((uint32_t*)(p + 4 + bytes)) = 0;
  }

  if (io_malloc.cfg & 1UL) {
    io_malloc.free_req++;
    io_malloc.free += bytes;
    io_malloc.used -= bytes;

#if _IO_MALLOC_TRACK_MODE == 1
    io_malloc.f_ptr[io_malloc.f_idx] = ptr;
    io_malloc.f_s[io_malloc.f_idx] = bytes;
    io_malloc.f_idx++;

    if (io_malloc.f_idx >= _IO_MALLOC_TRACK_MODE)
      io_malloc.f_idx = 0;
#endif
  }
  __real_free(p);
}

#endif


/* -----------------------------------------------------------------------------
 * STACK Monitor functions
 * -----------------------------------------------------------------------------
 */

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
extern uint32_t _estack[];

#elif defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
extern int CSTACK$$Limit;
extern int CSTACK$$Base;

#endif

struct io_stack io_stack;

void stackMonInit(uint32_t ctrl, uint32_t cstack, uint32_t msize)
{
  memset(&io_stack, 0, sizeof(struct io_stack));

  /* Reading ARM Core registers */
  io_stack.ctrl = ctrl;
  io_stack.cstack = cstack;

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
  io_stack.estack = (uint32_t)_estack;
  io_stack.bstack = io_stack.estack - msize;
  io_stack.mstack_size = msize;
#elif defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
  io_stack.estack = (uint32_t)&CSTACK$$Limit;
  io_stack.bstack = (uint32_t)&CSTACK$$Base;
  io_stack.mstack_size = (uint32_t)&CSTACK$$Limit - (uint32_t)&CSTACK$$Base;
#endif

  /* Check that MSP is the active stack */
  if (io_stack.ctrl & CONTROL_SPSEL_Msk) {
    LC_PRINT("E: MSP is not the active stack (stack monitoring is disabled)\r\n");
    io_stack.stack_mon = false;
  } else
    io_stack.stack_mon = true;

  /* Calculating used stack before test */
  io_stack.ustack_size = io_stack.estack - io_stack.cstack;

  if ((io_stack.stack_mon) && (io_stack.ustack_size > io_stack.mstack_size)) {
    LC_PRINT("E: !stack overflow detected %d > %d\r\n", (int)io_stack.ustack_size,
        (int)io_stack.mstack_size);
    io_stack.stack_mon = false;
  }
}


/* -----------------------------------------------------------------------------
 * Device HW-setting functions
 * -----------------------------------------------------------------------------
 */

struct cyclesCount cyclesCount;

int dwtCyclesToTime(uint64_t clks, struct dwtTime *t)
{
  if (!t)
    return -1;
  uint32_t fcpu = port_hal_get_cpu_freq();
  uint64_t s  = clks / fcpu;
  uint64_t ms = (clks * 1000) / fcpu;
  uint64_t us = (clks * 1000 * 1000) / fcpu;
  ms -= (s * 1000);
  us -= (ms * 1000 + s * 1000000);
  t->fcpu = fcpu;
  t->s = s;
  t->ms = ms;
  t->us = us;
  return 0;
}

float dwtCyclesToFloatMs(uint64_t clks)
{
  float res;
  float fcpu = (float)port_hal_get_cpu_freq();
  res = ((float)clks * (float)1000.0) / fcpu;
  return res;
}

static const char *devIdToStr(uint16_t dev_id)
{
  /* DEV_ID field from DBGMCU register (see BZ119679)*/
  const char *str;
  switch (dev_id) {
#if defined(STM32F3)
  case 0x439: str = "STM32F301"; break;       /* Fugu */
  case 0x432: str = "STM32F37x"; break;       /* GoldFish */
  case 0x422: str = "STM32F303xB/C"; break;   /* MantaEdge */
  case 0x438: str = "STM32F30x/F33x"; break;  /* Piccolo */
  case 0x446: str = "STM32F303D/E"; break;    /* Sunfish512k */
#elif defined(STM32F4)
  case 0x419: str = "STM32F4x7"; break;     /* BigManta */
  case 0x434: str = "STM32F469"; break;     /* MantaDragon */
  case 0x423: str = "STM32F401xB/C"; break; /* TinyManta */
  case 0x431: str = "STM32F411xC/E"; break; /* TinyMantaLP */
  case 0x441: str = "STM32F412"; break;     /* TinyMantaLP1M */
  case 0x421: str = "STM32F446"; break;     /* LittleManta512K */
  case 0x413: str = "STM32F4x"; break;      /* MantaF405 */
  case 0x458: str = "STM32F410"; break;     /* TinyManta128KLP */
  case 0x463: str = "STM32F413"; break;     /* TinyMantaLP1.5M */
  case 0x433: str = "STM32F401xD/E"; break;
#elif defined(STM32L0)
  case 0x447: str = "STM32L0x5"; break;  /* BigNemo192K */
  case 0x425: str = "STM32L0x2"; break;  /* LittleNemo32K */
  case 0x417: str = "STM32L0x3"; break;  /* NemoL051 */
  case 0x457: str = "STM32L0x5"; break;  /* TinyNemo16K */
#elif defined(STM32F0)
  case 0x448: str = "STM32F07x"; break;  /* Stingray128k */
  case 0x442: str = "STM32F091"; break;  /* Stingray256k see RM0091 */
  case 0x445: str = "STM32F042"; break;  /* Stingray32k */
  case 0x440: str = "STM32F03x/F05x"; break;  /* Stingray64k */
  case 0x444: str = "STM32F03x"; break;  /* Stingray32k-F03x */
#elif defined(STM32L4)
  case 0x461: str = "STM32L49xxx"; break;  /* BigOrca_1M */
  case 0x462: str = "STM32L45xxx"; break;  /* Orca_512K */
  case 0x464: str = "STM32L4x2"; break;    /* Orca_128K */
  case 0x435: str = "STM32L43xxx"; break;  /* Little_Orca_256K */
  case 0x415: str = "STM32L4x6xx"; break;  /* Orca_1M */
  case 0x471: str = "STM32L4P/Q"; break;   /* Orca_plus_1MB */
  case 0x470: str = "STM32L4R/S"; break;   /* Orca_plus_2MB */
#elif defined(STM32L5)
  case 0x472: str = "STM32L5[5,6]2xx"; break;  /* Beluga */
#elif defined(STM32U5)
  case 0x482: str = "STM32U575/585"; break;  /* Cobra2M */
#elif defined(STM32F7)
  case 0x451: str = "STM32F76x/F77x"; break; /* BigBirdie */
  case 0x449: str = "STM32F746"; break;      /* Birdie */
  case 0x452: str = "STM32F72x/F73x"; break; /* LittleBirdie */
#elif defined(STM32H7)
  case 0x450: str = "STM32H743/53/50xx and STM32H745/55/47/57xx"; break; /* Shark2M - see RM0433 & RM0399 */
  case 0x480: str = "STM32H7A/7B"; break;  /* TinyShark2M */
  case 0x483: str = "STM32H7[2,3]x"; break;  /* LittleShark1M */
#elif defined(STM32G4)
  case 0x469: str = "STM32G474"; break;  /* Rockfish_512K */
  case 0x468: str = "STM32G454"; break;  /* Tiny_Rockfish_128K */
  case 0x479: str = "STM32G491"; break;  /* Tiny_Rockfish_512K */
#elif defined(STM32G0)
  case 0x460: str = "STM32G07x/8x"; break;  /* OrcaZero128k */
  case 0x467: str = "STM32G0B1/C1"; break;  /* OrcaZero512k see RM0444 */
  case 0x466: str = "STM32G03x/4x"; break;  /* OrcaZero64k */
  case 0x456: str = "STM32G051/61"; break;  /* OrcaZeroMax64k */
#elif defined(STM32WB)
  case 0x494: str = "STM32WB1"; break;    /* TinyDory320k */
  case 0x495: str = "STM32WB5/3"; break;  /* Dory1M-STM32WB5 LIttleDory512K */
#elif defined(STM32WL)
  case 0x497: str = "Bee-WLx"; break;     /* Bee-WLx */
#elif defined(STM32MP1)
  case 0x500: str = "STM32MP1"; break;  /* */
#elif defined(STM32H5)
  case 0x484: str = "STM32H5x"; break;  /* */
  case 0x474: str = "STM32H503"; break;  /* */
#elif defined(STM32N6)
  case 0x486: str = "STM32N6"; break;  /* */
#elif defined(CORSTONE_300)
  case 0x155: str = "Corstone SSE-300 FVP"; break;  /* */
#elif defined(SR5E1) /* STELLAR-E device */
  case 0x2511: str = "SR5E1x"; break;  /* see RM0483 */
#endif
  default:    str = "UNKNOWN";
  }
  return str;
}

#if defined(DUAL_CORE) && defined(STM32H7) && defined(CORE_CM4)
#define _STM32H7_DUAL_CORE_CM4
#endif

#if !defined(STM32F0) && !defined(STM32L0) && !defined(STM32G0) && !defined(STM32C0) && !defined(STM32F3) && !defined(STM32MP1) && !defined(_STM32H7_DUAL_CORE_CM4)
__STATIC_INLINE const char* bitToStr(uint32_t val)
{
  if (val)
    return "True";
  else
    return "False";
}
#endif


void getSysConf(struct mcu_conf *conf)
{
  if (!conf)
    return;

  conf->sclk = port_hal_get_cpu_freq();
  conf->hclk = port_hal_get_sys_freq();
  conf->conf = getFlashCacheConf();
  conf->devid = port_hal_get_dev_id();
  conf->revid = port_hal_get_rev_id();

#if defined(HAS_EXTRA_CONF) && HAS_EXTRA_CONF > 0
  port_hal_set_extra_conf(&conf->extra[0]);
#endif
}

/*
 -- conf 32b --

    b31..b24               (8b) series     L4/F4:1, F7:2, H7:3, U5/L5:4, N6:5
    b16                    (1b) fpu

    F7/H7/N6 b11 b10       (2b) D$, I$
    F7       b9 b8         (2b) art_en, art_prefetch

    F4       b10 b9 b8     (3b) art_prefetch, art_icache, art_dcache
             b7..b0        (8b) lat cycles

    L5       b9            (1b) icache
    N6       b7..b0        (8b) misc/extra

 */

#define MCONF_L4_F4     (1 << 24)
#define MCONF_F7        (2 << 24)
#define MCONF_H7        (3 << 24)  /* Used for SR5E1 device - Cortex-m7 based */
#define MCONF_U5_L5     (4 << 24)
#define MCONF_N6        (5 << 24)

#define MCONF_SR5E1     (3 << 24)

#define MCONF_FPU       (1 << 16)

#define MCONF_ART_DCACHE    (1 << 8)
#define MCONF_ART_ICACHE    (1 << 9)
#define MCONF_ART_PREFETCH  (1 << 10)

#define MCONF_CORE_ICACHE    (1 << 10)
#define MCONF_CORE_DCACHE    (1 << 11)

#define MCONF_ART_EN        (1 << 9)
#define MCONF_ART_PREF      (1 << 8)


uint32_t getFlashCacheConf(void)
{
  uint32_t mconf = 0;
#if defined(STM32F0) || defined(STM32L0) || defined(STM32G0) || defined(STM32C0) || defined(STM32F3)

#endif

#if defined(STM32L4) || defined(STM32F4)

  mconf |= MCONF_L4_F4;

  __I uint32_t acr = FLASH->ACR ;

  mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);

  if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
    mconf |= MCONF_ART_DCACHE;
  if ((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos)
    mconf |= MCONF_ART_ICACHE;
  if ((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos)
    mconf |= MCONF_ART_PREFETCH;

#endif

#if defined(STM32F7)

  mconf |= MCONF_F7;

  __I uint32_t acr = FLASH->ACR;
  __I uint32_t ccr = SCB->CCR;

  mconf |= ((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
  if ((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos)
    mconf |= MCONF_ART_PREF;
  if ((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos)
    mconf |= MCONF_ART_EN;

  if (ccr & SCB_CCR_IC_Msk)
    mconf |= MCONF_CORE_ICACHE;
  if (ccr & SCB_CCR_DC_Msk)
    mconf |= MCONF_CORE_DCACHE;

#endif

#if defined(STM32H7)

  mconf |= MCONF_H7;

  __I uint32_t acr = FLASH->ACR;

  mconf |= (acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos;
#if !defined(CORE_CM4)
  __I uint32_t ccr = SCB->CCR;

  if (ccr & SCB_CCR_IC_Msk)
    mconf |= MCONF_CORE_ICACHE;
  if (ccr & SCB_CCR_DC_Msk)
    mconf |= MCONF_CORE_DCACHE;
#endif

#endif

#if defined(STM32L5) || defined(STM32U5) || defined(STM32H5)
  mconf |= MCONF_U5_L5;
  if (READ_BIT(ICACHE->CR, ICACHE_CR_EN))
    mconf |= MCONF_ART_ICACHE;
#endif

#if defined(STM32N6)
  mconf |= MCONF_N6;
#endif

#if (__FPU_PRESENT == 1) && __FPU_USED
  mconf |= MCONF_FPU;  /* FPU present and used */
#endif

#if defined(CORSTONE_300) || defined(ARMCM55) || defined(STM32N6)
  mconf |= MCONF_N6;
  __I uint32_t ccr = SCB->CCR;
  if (ccr & SCB_CCR_DC_Msk)
    mconf |= MCONF_CORE_DCACHE;

  if (ccr & SCB_CCR_IC_Msk)
    mconf |= MCONF_CORE_ICACHE;
#endif

#if defined(SR5E1)

  mconf |= MCONF_SR5E1;

  __I uint32_t ccr = SCB->CCR;

  if (ccr & SCB_CCR_IC_Msk)
    mconf |= MCONF_CORE_ICACHE;
  if (ccr & SCB_CCR_DC_Msk)
    mconf |= MCONF_CORE_DCACHE;

#endif

  return mconf;
}

void systemSettingLog(void)
{
  struct mcu_conf sys_conf;

  /* Display ARM Complier version */

#if defined(_IS_AC6_COMPILER) && _IS_AC6_COMPILER
  LC_PRINT("Compiled with MDK-ARM Keil (Arm Compiler 6) %d\r\n", __ARMCC_VERSION);
#elif defined(_IS_GHS_COMPILER) && _IS_GHS_COMPILER
  LC_PRINT("Compiled with GHS-ARM Compiler %d\r\n", __GHS_VERSION_NUMBER);
#elif defined(_IS_HTC_COMPILER) && _IS_HTC_COMPILER
  LC_PRINT("Compiled with HIGHTEC Clang %d.%d.%d\r\n", __clang_major__, __clang_minor__,
      __clang_patchlevel__);
#elif defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
  LC_PRINT("Compiled with GCC %d.%d.%d\r\n", __GNUC__, __GNUC_MINOR__,
      __GNUC_PATCHLEVEL__);
#elif defined(_IS_IAR_COMPILER) && _IS_IAR_COMPILER
  LC_PRINT("Compiled with IAR %d (build %d)\r\n", __IAR_SYSTEMS_ICC__,
      __BUILD_NUMBER__
  );
#elif defined(_IS_AC5_COMPILER) && _IS_AC5_COMPILER
  LC_PRINT("Compiled with MDK-ARM Keil %d\r\n", __ARMCC_VERSION);
#endif


  /* Display Device configuration */
  getSysConf(&sys_conf);

  LC_PRINT(DEVICE_FAMILY " device configuration...\r\n");

  LC_PRINT(" Device       : DevID:0x%04x (%s) RevID:0x%04x\r\n",
      (int)sys_conf.devid,
      devIdToStr(sys_conf.devid),
      (int)sys_conf.revid
  );

#if (__FPU_PRESENT == 1)
  LC_PRINT(" Core Arch.   : M%d - %s %s\r\n", __CORTEX_M, "FPU ", ((__FPU_USED) ? "used" : "not used!"));
#else
  LC_PRINT(" Core Arch.   : M%d - %s\r\n", __CORTEX_M, "no FPU");
#endif

  LC_PRINT(" HAL version  : 0x%08x\r\n", (int)port_hal_get_hal_version());
  LC_PRINT(" SYSCLK clock : %u MHz\r\n", (int)sys_conf.sclk/1000000);
  LC_PRINT(" HCLK clock   : %u MHz\r\n", (int)sys_conf.hclk/1000000);

#if defined(STM32F7)
  uint32_t acr = FLASH->ACR;
  uint32_t ccr = SCB->CCR;
  LC_PRINT(" FLASH conf.  : ACR=0x%08x - Prefetch=%s ART=%s latency=%d\r\n",
      (int)acr,
      bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
      bitToStr((acr & FLASH_ACR_ARTEN_Msk) >> FLASH_ACR_ARTEN_Pos),
      (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
  LC_PRINT(" CACHE conf.  : $I/$D=(%s,%s)\r\n",
      bitToStr(ccr & SCB_CCR_IC_Msk),
      bitToStr(ccr & SCB_CCR_DC_Msk));
#endif

#if defined(STM32H7)
  uint32_t acr = FLASH->ACR;
  LC_PRINT(" FLASH conf.  : ACR=0x%08x - latency=%d\r\n",
      (int)acr,
      (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#if !defined(CORE_CM4)
  uint32_t ccr = SCB->CCR;
  LC_PRINT(" CACHE conf.  : $I/$D=(%s,%s)\r\n",
      bitToStr(ccr & SCB_CCR_IC_Msk),
      bitToStr(ccr & SCB_CCR_DC_Msk));
#endif
#endif

#if defined(STM32F4) || defined(STM32L4)
  uint32_t acr = FLASH->ACR;
  LC_PRINT(" FLASH conf.  : ACR=0x%08x - Prefetch=%s $I/$D=(%s,%s) latency=%d\r\n",
      (int)acr,
      bitToStr((acr & FLASH_ACR_PRFTEN_Msk) >> FLASH_ACR_PRFTEN_Pos),
      bitToStr((acr & FLASH_ACR_ICEN_Msk) >> FLASH_ACR_ICEN_Pos),
      bitToStr((acr & FLASH_ACR_DCEN_Msk) >> FLASH_ACR_DCEN_Pos),
      (int)((acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos));
#endif

#if defined(STM32L5) || defined(STM32U5) || defined(STM32H5)
  LC_PRINT(" ICACHE       : %s\r\n", bitToStr(READ_BIT(ICACHE->CR, ICACHE_CR_EN)));
#endif

#if defined(ARMCM55) || defined(STM32N6)
  uint32_t ccr = SCB->CCR;
  LC_PRINT(" CACHE conf.  : $I/$D=(%s,%s)\r\n",
      bitToStr(ccr & SCB_CCR_IC_Msk),
      bitToStr(ccr & SCB_CCR_DC_Msk));
#endif

#if defined(SR5E1)
  uint32_t ccr = SCB->CCR;
  LC_PRINT(" CACHE conf.  : $I/$D=(%s,%s)\r\n",
      bitToStr(ccr & SCB_CCR_IC_Msk),
      bitToStr(ccr & SCB_CCR_DC_Msk));
#endif

}

