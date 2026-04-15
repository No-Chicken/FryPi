/**
 ******************************************************************************
 * @file    ai_device_adaptor.c
 * @author  MCD/AIS Team
 * @brief   Specific DEVICE port functions for AI test application
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "ai_device_adaptor.h"
 
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_IS_GCC_COMPILER) && _IS_GCC_COMPILER
#if defined(SR5E1)
/* --------------------------------------------
 * STELLAR-E series
 * --------------------------------------------
 */
extern uint32_t __process_stack_end__;
uint32_t *_estack = &__process_stack_end__;
#elif defined(SR6X)
/* --------------------------------------------
 * STELLAR-PG series
 * --------------------------------------------
 */
extern uint32_t __user_stack_limit__;
uint32_t *_estack = &__user_stack_limit__;
#endif
#endif

#if defined(STM32N657xx) || defined(STM32N6)

/* --------------------------------------------
 * STM32N6xx series
 * --------------------------------------------
 */
/******************************************************************************
 * @brief Retrieves IC frequencies from the MCU registers
 * @param[in]  ic_config:  pointer to an ICInitTypedef struct
 * @param[in]  pll_freqs:  array of PLL frequencies {PLL1_f, PLL2_f, PLL3_f, PLL4_f}
 ******************************************************************************/
static uint32_t get_IC_freq(RCC_ICInitTypeDef* ic_config, uint32_t* pll_freqs)
{
  uint8_t idx;
  switch(ic_config->ClockSelection & RCC_IC1CFGR_IC1SEL_Msk)
  {
  case LL_RCC_ICCLKSOURCE_PLL1:
    idx = 0;
    break;
  case LL_RCC_ICCLKSOURCE_PLL2:
    idx = 1;
    break;
  case LL_RCC_ICCLKSOURCE_PLL3:
    idx = 2;
    break;
  case LL_RCC_ICCLKSOURCE_PLL4:
    idx = 3;
    break;
  default:
    // Should never happen
    idx = 0;
    break;
  }
  return (pll_freqs[idx] / ic_config->ClockDivider);
}

/******************************************************************************
 * @brief Retrieves system frequencies from the MCU registers
 * @param[out] fmcu_mhz:  MCU frequency (MHz)
 * @param[out] fsysb_mhz: System bus frequency (MHz)
 * @param[out] fnpu_mhz:  NPU frequency (MHz)
 * @param[out] fnpur_mhz: NPU Rams frequency (MHz)
 ******************************************************************************/
static void get_system_freqs(uint32_t* fmcu_mhz, uint32_t* fsysb_mhz, uint32_t* fnpu_mhz, uint32_t* fnpur_mhz)
{
  RCC_ClkInitTypeDef clks;
  uint32_t fsysclk;
  uint32_t fpll[4];
  
  // Get current config
  HAL_RCC_GetClockConfig(&clks);
  fsysclk = HAL_RCC_GetSysClockFreq();
  fpll[0] = HAL_RCCEx_GetPLL1CLKFreq();
  fpll[1] = HAL_RCCEx_GetPLL2CLKFreq();
  fpll[2] = HAL_RCCEx_GetPLL3CLKFreq();
  fpll[3] = HAL_RCCEx_GetPLL4CLKFreq();
  
  if (clks.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11)
  {
    // CPU freq is IC1 freq
    // System bus freq is IC2 freq
    // NPU freq is IC6 freq
    // NPU Rams freq is IC11 freq
    *fmcu_mhz = get_IC_freq(&clks.IC1Selection, fpll) / 1000000;
    *fsysb_mhz = get_IC_freq(&clks.IC2Selection, fpll) / 1000000;
    *fnpu_mhz = get_IC_freq(&clks.IC6Selection, fpll) / 1000000;
    *fnpur_mhz = get_IC_freq(&clks.IC11Selection, fpll) / 1000000;
  }
  else
  {
    // sysclk is either HSI/MSI/HSE
    *fmcu_mhz = fsysclk / 1000000;
    *fsysb_mhz = fsysclk / 1000000;
    *fnpu_mhz = fsysclk / 1000000;
    *fnpur_mhz = fsysclk / 1000000;
  }
}

#if defined(HAS_EXTRA_CONF) && HAS_EXTRA_CONF >= 4
void port_hal_set_extra_conf(uint32_t *extra)
{
  uint32_t npur_freq_mhz = 64;         /* NIC freq */
  uint32_t npu_freq_mhz = 64;          /* NPU freq */
  uint32_t sysb_freq_mhz = 64;         /* NOC freq */
  uint32_t mcu_freq_mhz = 64;          /* MCU freq */
  // Get frequencies from MCU registers
  get_system_freqs(&mcu_freq_mhz, &sysb_freq_mhz, &npu_freq_mhz, &npur_freq_mhz);
  
  /* Add specific infos in relation to the ATONss
   * Note: NOC freq is returned by port_hal_get_sys_freq()
   */
  extra[0] = 0;                  /* extra config
                                        b7..0  : type/format    00h : N6/ATONss
                                        b8..31 : TBD
                                  */
  extra[1] = (npu_freq_mhz *1000*1000);   /* NPU freq. */
  extra[2] = (npur_freq_mhz*1000*1000);    /* NIC freq, AXIMRAM3,4,5,6, NPUCACHE */
  extra[3] = 0;
}
#endif

#endif /* STM32N657xx, STM32N6 */


#if !defined(NO_PORT_DWT_INIT_IMP)

#if defined(HAS_PMU_CTRL) && defined(HAS_DWT_CTRL) && HAS_PMU_CTRL == 1 && HAS_DWT_CTRL == 1
#error Only HAS_PMU_CTRL or HAS_DWT_CTRL should be defined.
#endif

#if defined(HAS_PMU_CTRL) && HAS_PMU_CTRL == 1

void port_dwt_init_imp()
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ARM_PMU_Enable();
  ARM_PMU_CNTR_Enable(PMU_CNTENSET_CCNTR_ENABLE_Msk);
}

#endif  /* HAS_PMU_CTRL */

#if defined(HAS_DWT_CTRL) && HAS_DWT_CTRL == 1

void port_dwt_init_imp()
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#ifdef STM32F7
  DWT->LAR = 0xC5ACCE55;
#endif
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

#endif /* HAS_DWT_CTRL */

#endif /* !NO_PORT_DWT_INIT_IMP */

#ifdef __cplusplus
}
#endif
