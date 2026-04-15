/**
 ******************************************************************************
 * @file    aiTestHelper.h
 * @author  MCD/AIS Team
 * @brief   Helper functions for AI test application
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

#ifndef __AI_TEST_HELPER_H__
#define __AI_TEST_HELPER_H__

#if !defined(TFLM_RUNTIME) && !defined(NO_STM_AI_RUNTIME)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ai_platform.h>

void aiPlatformVersion(void);

ai_u32 aiBufferSize(const ai_buffer* buffer);
void aiPrintNetworkInfo(const ai_network_report* report);
void aiLogErr(const ai_error err, const char *fct);
void aiPrintBufferInfo(const ai_buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif /* !TFLM_RUNTIME */

#endif /* __AI_TEST_HELPER_H__ */
