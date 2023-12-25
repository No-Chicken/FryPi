/**
 ******************************************************************************
 * @file    aiTestUtility.h
 * @author  MCD/AIS Team
 * @brief   STM32 Utility functions for STM32 AI test application
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

#ifndef __AI_TEST_UTILITY_H__
#define __AI_TEST_UTILITY_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <bsp_ai.h>  /* generated STM32 platform file to import the HAL and the UART definition */


#ifdef __cplusplus
extern "C" {
#endif

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* Keil ARM Compiler 6 toolchain */
#define _APP_STACK_MONITOR_ 0   /* not yet supported */
#define _APP_HEAP_MONITOR_  0   /* not yet supported */	
#define _ARM_TOOLS_ID       4   /* proto msg 2.2 definition AC6 */
#elif defined(__GNUC__) && !defined (__ARMCC_VERSION) /* GNU GCC toolchain */
#define _APP_STACK_MONITOR_ 1
#define _APP_HEAP_MONITOR_  1
#define _ARM_TOOLS_ID       1   /* proto msg 2.2 definition GCC */
#elif defined (__ICCARM__) /* IAR toolchain */
#define _APP_STACK_MONITOR_ 1
#define _APP_HEAP_MONITOR_  0   /* not yet supported */
#define _ARM_TOOLS_ID       2   /* proto msg 2.2 definition IAR */
#else
#define _APP_STACK_MONITOR_ 0   /* not yet supported */
#define _APP_HEAP_MONITOR_  0   /* not yet supported */
#define _ARM_TOOLS_ID       3   /* proto msg 2.2 definition AC5 / Keil */
#endif


#if defined(CHECK_STM32_FAMILY)
#if !defined(STM32F7) && !defined(STM32L4) && !defined(STM32L5) && !defined(STM32F4) && !defined(STM32H7) && !defined(STM32F3) && !defined(STM32U5) && !defined(STM32G0) && !defined(STM32F0) && !defined(STM32C0)
#error Only STM32H7, STM32F7, STM32F4, STM32L4, STM32L5, STM32U5, STM32G0, STM32F0, STM32C0 or STM32F3 device are supported
#endif
#endif


#if !defined(USE_SYSTICK_ONLY)
#define USE_SYSTICK_ONLY 0
#else
#if USE_SYSTICK_ONLY != 0 && USE_SYSTICK_ONLY != 1
#error USE_SYSTICK_ONLY should be equal to 0 or 1
#endif
#endif

#if defined(STM32G0) || defined(STM32F0) || defined(STM32L0) || defined(STM32C0)
#undef USE_SYSTICK_ONLY
#define USE_SYSTICK_ONLY 1
#endif

#if !defined(USE_CORE_CLOCK_ONLY)
#define USE_CORE_CLOCK_ONLY 0
#endif


#if !defined(USE_PRINTF_FROM_TOOL)
extern void lc_print(const char* fmt, ... );

#define LC_PRINT(fmt, ...) lc_print(fmt, ##__VA_ARGS__)

#else

#define LC_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif


/* -----------------------------------------------------------------------------
 * MISC definitions
 * -----------------------------------------------------------------------------
 */

#define _CONCAT_ARG(a, b)     a ## b
#define _CONCAT(a, b)         _CONCAT_ARG(a, b)

#if defined(_MSC_VER)
  #define MEM_ALIGNED(x)
#elif defined(__ICCARM__) || defined (__IAR_SYSTEMS_ICC__)
  #define MEM_ALIGNED(x)         _CONCAT(MEM_ALIGNED_,x)
  #define MEM_ALIGNED_1          _Pragma("data_alignment = 1")
  #define MEM_ALIGNED_2          _Pragma("data_alignment = 2")
  #define MEM_ALIGNED_4          _Pragma("data_alignment = 4")
  #define MEM_ALIGNED_8          _Pragma("data_alignment = 8")
  #define MEM_ALIGNED_16         _Pragma("data_alignment = 16")
  #define MEM_ALIGNED_32         _Pragma("data_alignment = 32")
#elif defined(__CC_ARM)
  #define MEM_ALIGNED(x)         __attribute__((aligned (x)))
#elif defined(__GNUC__)
  #define MEM_ALIGNED(x)         __attribute__((aligned(x)))
#else
  #define MEM_ALIGNED(x)
#endif

/* -----------------------------------------------------------------------------
 * HEAP monitor functions
 * -----------------------------------------------------------------------------
 */

#define _IO_MALLOC_TRACK_MODE 0  /* allows to track the allocated/released @/s */
#define _IO_MALLOC_TRACK_DEPTH_SIZE (16)

struct io_malloc {
  uint32_t cfg;           /* configuration and state */
  uint32_t alloc;         /* accumulated size of allocated memory */
  uint32_t free;          /* accumulated size of freed memory */
  uint32_t alloc_req;     /* number of requested alloc */
  uint32_t free_req;      /* number of requested free */
  uint32_t max;           /* maximum allocated memory */
  uint32_t used;          /* current allocated memory */
#if _IO_MALLOC_TRACK_MODE == 1
  void* a_ptr[_IO_MALLOC_TRACK_DEPTH_SIZE];
  size_t a_s[_IO_MALLOC_TRACK_DEPTH_SIZE];
  int a_idx;
  void *f_ptr[_IO_MALLOC_TRACK_DEPTH_SIZE];
  size_t f_s[_IO_MALLOC_TRACK_DEPTH_SIZE];
  int f_idx;
#endif
};

extern struct io_malloc io_malloc;


#if defined(__GNUC__) && !defined(__ARMCC_VERSION)

#include <stdlib.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")
static inline void _mon_alloc_reset(void) {
    memset((void *)&io_malloc, 0, sizeof(struct io_malloc));
    /* force a call of wrap functions */\
    free(malloc(10));
}
#pragma GCC pop_options
#define MON_ALLOC_RESET()\
    _mon_alloc_reset()
#else
#define MON_ALLOC_RESET()
#endif

#if defined(_APP_HEAP_MONITOR_) && _APP_HEAP_MONITOR_ == 1

#define MON_ALLOC_ENABLE() io_malloc.cfg |= 1UL

#define MON_ALLOC_DISABLE() io_malloc.cfg &= ~1UL

#define MON_ALLOC_REPORT() \
		LC_PRINT(" used heap    : %ld:%ld %ld:%ld (req:allocated,req:released) max=%ld cur=%ld (cfg=%ld)\r\n", \
        io_malloc.alloc_req, io_malloc.alloc, \
        io_malloc.free_req, io_malloc.free, \
        io_malloc.max, io_malloc.used, \
        (io_malloc.cfg & (3 << 1)) >> 1)

#define MON_ALLOC_MAX_USED() (int)io_malloc.max
#define MON_ALLOC_USED() (int)io_malloc.used

#else

#define MON_ALLOC_ENABLE()
#define MON_ALLOC_DISABLE()

#define MON_ALLOC_REPORT() \
		LC_PRINT(" used heap    : DISABLED or NOT YET SUPPORTED\r\n")

#define MON_ALLOC_MAX_USED() (-1)
#define MON_ALLOC_USED() (-1)

#endif


/* -----------------------------------------------------------------------------
 * STACK monitor functions
 * -----------------------------------------------------------------------------
 */

struct io_stack {
  uint32_t ctrl;
  bool stack_mon;
  int32_t  susage;

  uint32_t ustack_size; /* used stack before test */
  uint32_t estack;      /* end of stack @ */
  uint32_t mstack_size; /* minimal master stack size */
  uint32_t cstack;      /* current stack @ */
  uint32_t bstack;      /* base stack @ */
};

extern struct io_stack io_stack;

void stackMonInit(uint32_t ctrl, uint32_t cstack, uint32_t msize);

#if defined(_APP_STACK_MONITOR_) && _APP_STACK_MONITOR_ == 1

#define MON_STACK_INIT() stackMonInit(__get_CONTROL(), __get_MSP(), MIN_STACK_SIZE)

#define MON_STACK_MARK()\
    if (io_stack.stack_mon) {\
      uint32_t *pw =  (uint32_t*)((io_stack.bstack + 3) & (~3));\
      while ((uint32_t)pw < io_stack.cstack) {\
        *pw = 0xDEDEDEDE;\
        pw++;\
      }\
    }

#define MON_STACK_CHECK0()\
    if (__get_MSP() != io_stack.cstack) {\
    	LC_PRINT("E: !current stack address is not coherent 0x%08lx instead 0x%08lx\r\n",\
          __get_MSP(), io_stack.cstack);\
    }

#define MON_STACK_EVALUATE()\
    /* Calculating the used stack */\
    io_stack.susage = 0UL;\
    if (io_stack.stack_mon) {\
      uint32_t rstack = io_stack.mstack_size - io_stack.ustack_size;\
      uint32_t *pr =  (uint32_t*)((io_stack.bstack + 3) & (~3));\
      bool overflow = false;\
      \
      /* check potential stack overflow with 8 last words*/\
    for (int i = 0; i < 8; i++) {\
      if (*pr != 0xDEDEDEDE)\
      overflow = true;\
      pr++;\
    }\
    if (!overflow) {\
      io_stack.susage = 8*4;\
      while ((*pr == 0xDEDEDEDE) && ((uint32_t)pr < io_stack.cstack)) { pr++; io_stack.susage += 4; }\
      io_stack.susage = rstack - io_stack.susage;\
    } else {\
      io_stack.susage = -1;\
      LC_PRINT("E: !stack overflow detected > %ld\r\n", rstack);\
      LC_PRINT("note: MIN_STACK_SIZE value/definition should be verified (app_x-cube-ai.h & linker file)");\
    }\
    }

#define MON_STACK_REPORT()\
    if (io_stack.stack_mon)\
	LC_PRINT(" used stack   : %ld bytes\r\n", io_stack.susage);\
    else\
	LC_PRINT(" used stack   : NOT CALCULATED\r\n")

#define MON_STACK_STATE(msg)\
		LC_PRINT("D: %s (0x%08lx-0x%08lx %ld/%ld ctrl=0x%08lx\r\n",msg,\
        io_stack.estack, io_stack.cstack, io_stack.ustack_size, io_stack.mstack_size, io_stack.ctrl)

#else

#define MON_STACK_INIT()
#define MON_STACK_MARK()
#define MON_STACK_CHECK0()
#define MON_STACK_EVALUATE()

#define MON_STACK_REPORT()\
		LC_PRINT(" used stack   : DISABLED\r\n");

#define MON_STACK_STATE(msg);

#endif


/* -----------------------------------------------------------------------------
 * Timer/clock count services
 * -----------------------------------------------------------------------------
 */

struct dwtTime {
  uint32_t fcpu;
  int s;
  int ms;
  int us;
};

#if USE_SYSTICK_ONLY == 0

__STATIC_INLINE void dwtIpInit(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

#ifdef STM32F7
  DWT->LAR = 0xC5ACCE55;
#endif

  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk | DWT_CTRL_CPIEVTENA_Msk;
}

__STATIC_FORCEINLINE void dwtReset(void) {
  DWT->CYCCNT = 0; /* Clear DWT cycle counter */
}

__STATIC_FORCEINLINE  uint32_t dwtGetCycles(void) {
  return DWT->CYCCNT;
}

#endif

uint32_t systemCoreClock(void);
int dwtCyclesToTime(uint64_t clks, struct dwtTime *t);
float dwtCyclesToFloatMs(uint64_t clks);

struct cyclesCount {
#if USE_SYSTICK_ONLY == 1
  uint32_t tick_start;
  __IOM uint32_t count_start;
#else
#if USE_CORE_CLOCK_ONLY == 1
  uint32_t dwt_start;
#else
  uint32_t dwt_max;
  uint32_t dwt_start;
  uint32_t tick_start;
#endif
#endif
};

extern struct cyclesCount cyclesCount;

__STATIC_INLINE void cyclesCounterInit(void)
{
#if USE_SYSTICK_ONLY == 1
  __IOM uint32_t ctrl = SysTick->CTRL;
  uint32_t val = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk   | SysTick_CTRL_ENABLE_Msk;

  LC_PRINT(" Timestamp    : SysTick only (%d)\r\n", (ctrl & val) == val);

  cyclesCount.tick_start = HAL_GetTick();
  cyclesCount.count_start = SysTick->VAL;
#else
  struct dwtTime t;
  uint32_t st;

  dwtIpInit();
  dwtReset();
  HAL_Delay(100);
  st = dwtGetCycles();
  dwtCyclesToTime(st/100, &t);

#if USE_CORE_CLOCK_ONLY == 1
  LC_PRINT(" Timestamp    : DWT only (HAL_Delay(1)=%d.%03d ms)\r\n",
      t.s * 100 + t.ms, t.us);
#else
  LC_PRINT(" Timestamp    : SysTick + DWT (HAL_Delay(1)=%d.%03d ms)\r\n",
      t.s * 100 + t.ms, t.us);
  dwtCyclesToTime(UINT32_MAX, &t);
  cyclesCount.dwt_max = t.s * 1000 + t.ms;
#endif
  dwtIpInit();
#endif
}

__STATIC_INLINE void cyclesCounterStart(void)
{
#if USE_SYSTICK_ONLY == 1
  cyclesCount.tick_start = HAL_GetTick();
  cyclesCount.count_start = SysTick->VAL;
#else
#if USE_CORE_CLOCK_ONLY == 1
  dwtReset();
  cyclesCount.dwt_start = dwtGetCycles();
#else
  cyclesCount.tick_start = HAL_GetTick();
  dwtReset();
  cyclesCount.dwt_start = dwtGetCycles();
#endif
#endif
}

__STATIC_INLINE uint64_t cyclesCounterEnd(void)
{
#if USE_SYSTICK_ONLY == 1
  const uint32_t cur_val = SysTick->VAL;
  uint32_t cur_load;
  const uint32_t tick_e = HAL_GetTick() - cyclesCount.tick_start;
  uint64_t clk_n = 0ULL;
  if (tick_e) {
    cur_load = SysTick->LOAD;
    clk_n = (cur_load - cur_val) + cyclesCount.count_start + (tick_e - 1) * (cur_load + 1);
  }
  else {
    clk_n = cyclesCount.count_start - cur_val;
  }
  return clk_n;
#else
#if USE_CORE_CLOCK_ONLY == 1
  return (uint64_t)(dwtGetCycles() - cyclesCount.dwt_start);
#else
  uint64_t dwt_e;
  uint64_t tick_e;
  dwt_e = dwtGetCycles() - cyclesCount.dwt_start;
  tick_e = HAL_GetTick() - cyclesCount.tick_start;
  if (tick_e > cyclesCount.dwt_max) {
    /* overflow detected */
    dwt_e = ((tick_e/cyclesCount.dwt_max) * (uint64_t)UINT32_MAX + (uint64_t)dwt_e);
  }
  return dwt_e;
#endif
#endif
}

/* -----------------------------------------------------------------------------
 * IO functions
 * -----------------------------------------------------------------------------
 */

int ioRawGetUint8(uint8_t *c, uint32_t timeout);

void ioRawDisableLLWrite(void);
bool ioRawWriteBuffer(uint8_t *buff, int count);
bool ioRawReadBuffer(uint8_t *buff, int count);


/* -----------------------------------------------------------------------------
 * System services
 * -----------------------------------------------------------------------------
 */

void crcIpInit(void);

__STATIC_INLINE uint32_t disableInts(void)
{
  uint32_t state;

  state = __get_PRIMASK();
  __disable_irq();

  return state;
}

__STATIC_INLINE void restoreInts(uint32_t state)
{
  __set_PRIMASK(state);
}

void systemSettingLog(void);
uint32_t getFlashCacheConf(void);

struct mcu_conf {
  uint32_t sclk;
  uint32_t hclk;
  uint32_t conf;
  uint32_t devid;
  uint32_t revid;
};

void getSysConf(struct mcu_conf *conf);


#ifdef __cplusplus
}
#endif

#endif /* __AI_TEST_UTILITY_H__ */
