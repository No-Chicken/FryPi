/**
  ******************************************************************************
  * @file    sin_calcu_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-04-14T16:41:13+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef SIN_CALCU_DATA_PARAMS_H
#define SIN_CALCU_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_SIN_CALCU_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_sin_calcu_data_weights_params[1]))
*/

#define AI_SIN_CALCU_DATA_CONFIG               (NULL)


#define AI_SIN_CALCU_DATA_ACTIVATIONS_SIZES \
  { 60, }
#define AI_SIN_CALCU_DATA_ACTIVATIONS_SIZE     (60)
#define AI_SIN_CALCU_DATA_ACTIVATIONS_COUNT    (1)
#define AI_SIN_CALCU_DATA_ACTIVATION_1_SIZE    (60)



#define AI_SIN_CALCU_DATA_WEIGHTS_SIZES \
  { 324, }
#define AI_SIN_CALCU_DATA_WEIGHTS_SIZE         (324)
#define AI_SIN_CALCU_DATA_WEIGHTS_COUNT        (1)
#define AI_SIN_CALCU_DATA_WEIGHT_1_SIZE        (324)



#define AI_SIN_CALCU_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_sin_calcu_activations_table[1])

extern ai_handle g_sin_calcu_activations_table[1 + 2];



#define AI_SIN_CALCU_DATA_WEIGHTS_TABLE_GET() \
  (&g_sin_calcu_weights_table[1])

extern ai_handle g_sin_calcu_weights_table[1 + 2];


#endif    /* SIN_CALCU_DATA_PARAMS_H */
