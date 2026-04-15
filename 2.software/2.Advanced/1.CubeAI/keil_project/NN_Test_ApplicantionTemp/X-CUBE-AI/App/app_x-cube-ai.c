
/**
  ******************************************************************************
  * @file    app_x-cube-ai.c
  * @author  X-CUBE-AI C code generator
  * @brief   AI program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

 /*
  * Description
  *   v1.0 - Minimum template to show how to use the Embedded Client API
  *          model. Only one input and one output is supported. All
  *          memory resources are allocated statically (AI_NETWORK_XX, defines
  *          are used).
  *          Re-target of the printf function is out-of-scope.
  *   v2.0 - add multiple IO and/or multiple heap support
  *
  *   For more information, see the embeded documentation:
  *
  *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
  *
  *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
  *   typical : C:\Users\[user_name]\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined ( __ICCARM__ )
#elif defined ( __CC_ARM ) || ( __GNUC__ )
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "sin_calcu.h"
#include "sin_calcu_data.h"

/* USER CODE BEGIN includes */
/* USER CODE END includes */

/* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_SIN_CALCU_INPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_in_1[AI_SIN_CALCU_IN_1_SIZE_BYTES];
ai_i8* data_ins[AI_SIN_CALCU_IN_NUM] = {
data_in_1
};
#else
ai_i8* data_ins[AI_SIN_CALCU_IN_NUM] = {
NULL
};
#endif

#if !defined(AI_SIN_CALCU_OUTPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_out_1[AI_SIN_CALCU_OUT_1_SIZE_BYTES];
ai_i8* data_outs[AI_SIN_CALCU_OUT_NUM] = {
data_out_1
};
#else
ai_i8* data_outs[AI_SIN_CALCU_OUT_NUM] = {
NULL
};
#endif

/* Activations buffers -------------------------------------------------------*/

AI_ALIGNED(32)
static uint8_t pool0[AI_SIN_CALCU_DATA_ACTIVATION_1_SIZE];

ai_handle data_activations0[] = {pool0};

/* AI objects ----------------------------------------------------------------*/

static ai_handle sin_calcu = AI_HANDLE_NULL;

static ai_buffer* ai_input;
static ai_buffer* ai_output;

static void ai_log_err(const ai_error err, const char *fct)
{
  /* USER CODE BEGIN log */
  if (fct)
    printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
        err.type, err.code);
  else
    printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

  do {} while (1);
  /* USER CODE END log */
}

static int ai_boostrap(ai_handle *act_addr)
{
  ai_error err;

  /* Create and initialize an instance of the model */
  err = ai_sin_calcu_create_and_init(&sin_calcu, act_addr, NULL);
  if (err.type != AI_ERROR_NONE) {
    ai_log_err(err, "ai_sin_calcu_create_and_init");
    return -1;
  }

  ai_input = ai_sin_calcu_inputs_get(sin_calcu, NULL);
  ai_output = ai_sin_calcu_outputs_get(sin_calcu, NULL);

#if defined(AI_SIN_CALCU_INPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-inputs" option is used, memory buffer can be
   *  used from the activations buffer. This is not mandatory.
   */
  for (int idx=0; idx < AI_SIN_CALCU_IN_NUM; idx++) {
	data_ins[idx] = ai_input[idx].data;
  }
#else
  for (int idx=0; idx < AI_SIN_CALCU_IN_NUM; idx++) {
	  ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_SIN_CALCU_OUTPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-outputs" option is used, memory buffer can be
   *  used from the activations buffer. This is no mandatory.
   */
  for (int idx=0; idx < AI_SIN_CALCU_OUT_NUM; idx++) {
	data_outs[idx] = ai_output[idx].data;
  }
#else
  for (int idx=0; idx < AI_SIN_CALCU_OUT_NUM; idx++) {
	ai_output[idx].data = data_outs[idx];
  }
#endif

  return 0;
}

static int ai_run(void)
{
  ai_i32 batch;

  batch = ai_sin_calcu_run(sin_calcu, ai_input, ai_output);
  if (batch != 1) {
    ai_log_err(ai_sin_calcu_get_error(sin_calcu),
        "ai_sin_calcu_run");
    return -1;
  }

  return 0;
}

/* USER CODE BEGIN 2 */
static float s_input_deg = 0.0f;
static float s_y_true = 0.0f;
static float s_deg = 0.0f;

#define APP_PI (3.14159265359f)
#define APP_INPUT_MAX_DEG (180.0f)
#define APP_INPUT_STEP_DEG (1.0f)

int acquire_and_process_data(ai_i8* data[])
{
  float* input = (float*)data[0];

  s_input_deg = s_deg;
  s_y_true = sinf(s_input_deg * APP_PI / 180.0f);
  input[0] = s_input_deg;

  s_deg += APP_INPUT_STEP_DEG;
  if (s_deg > APP_INPUT_MAX_DEG) {
    s_deg = 0.0f;
  }

  return 0;
}

int post_process(ai_i8* data[])
{
  const float* output = (const float*)data[0];

  /* FireWater text frame for VOFA+: input(deg), prediction, ground truth */
  printf("input, y_pre, y_ture: %.2f, %.2f, %.2f\r\n", s_input_deg, output[0], s_y_true);

  return 0;
}
/* USER CODE END 2 */

/* Entry points --------------------------------------------------------------*/

void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 5 */
  printf("\r\nTEMPLATE - initialization\r\n");

  ai_boostrap(data_activations0);
    /* USER CODE END 5 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 6 */
  int res = -1;

  if (sin_calcu) {
    /* 1 - acquire and pre-process input data */
    res = acquire_and_process_data(data_ins);
    /* 2 - process the data - call inference engine */
    if (res == 0)
      res = ai_run();
    /* 3 - post-process the predictions */
    if (res == 0)
      res = post_process(data_outs);
    if (res == 0)
      HAL_Delay(100);
  }

  if (res) {
    ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
    ai_log_err(err, "Process has FAILED");
  }
    /* USER CODE END 6 */
}
#ifdef __cplusplus
}
#endif
