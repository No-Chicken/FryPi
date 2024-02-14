/**
  ******************************************************************************
  * @file    gesture_cnn.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sat Feb  3 17:16:14 2024
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "gesture_cnn.h"
#include "gesture_cnn_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_gesture_cnn
 
#undef AI_GESTURE_CNN_MODEL_SIGNATURE
#define AI_GESTURE_CNN_MODEL_SIGNATURE     "58e168a269f9c3f0cc8187a031e49b30"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Sat Feb  3 17:16:14 2024"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_GESTURE_CNN_N_BATCHES
#define AI_GESTURE_CNN_N_BATCHES         (1)

static ai_ptr g_gesture_cnn_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_gesture_cnn_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  dense_dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2304, AI_STATIC)
/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  dense_dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)
/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 512, AI_STATIC)
/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)
/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  dense_2_dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 96, AI_STATIC)
/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  dense_2_dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)
/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 768, AI_STATIC)
/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2800, AI_STATIC)
/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2800, AI_STATIC)
/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  max_pooling2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 700, AI_STATIC)
/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)
/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 288, AI_STATIC)
/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  max_pooling2d_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)
/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  dense_dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)
/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)
/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)
/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)
/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  dense_2_dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 6, AI_STATIC)
/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  dense_2_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 6, AI_STATIC)
/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_conv2d_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 125, AI_STATIC)
/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_conv2d_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 5, AI_STATIC)
/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_conv2d_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 135, AI_STATIC)
/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_conv2d_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3, AI_STATIC)
/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  dense_dense_weights, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 72, 32, 1, 1), AI_STRIDE_INIT(4, 4, 288, 9216, 9216),
  1, &dense_dense_weights_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  dense_dense_bias, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &dense_dense_bias_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_dense_weights, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 32, 16, 1, 1), AI_STRIDE_INIT(4, 4, 128, 2048, 2048),
  1, &dense_1_dense_weights_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_dense_bias, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &dense_1_dense_bias_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  dense_2_dense_weights, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 16, 6, 1, 1), AI_STRIDE_INIT(4, 4, 64, 384, 384),
  1, &dense_2_dense_weights_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  dense_2_dense_bias, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &dense_2_dense_bias_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 32, 24), AI_STRIDE_INIT(4, 4, 4, 4, 128),
  1, &input_0_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_conv2d_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 28, 20), AI_STRIDE_INIT(4, 4, 4, 20, 560),
  1, &conv2d_conv2d_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_output, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 28, 20), AI_STRIDE_INIT(4, 4, 4, 20, 560),
  1, &conv2d_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  max_pooling2d_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 14, 10), AI_STRIDE_INIT(4, 4, 4, 20, 280),
  1, &max_pooling2d_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_conv2d_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 12, 8), AI_STRIDE_INIT(4, 4, 4, 12, 144),
  1, &conv2d_1_conv2d_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 12, 8), AI_STRIDE_INIT(4, 4, 4, 12, 144),
  1, &conv2d_1_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  max_pooling2d_1_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 6, 4), AI_STRIDE_INIT(4, 4, 4, 12, 72),
  1, &max_pooling2d_1_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  max_pooling2d_1_output0, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 72, 1, 1), AI_STRIDE_INIT(4, 4, 4, 288, 288),
  1, &max_pooling2d_1_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  dense_dense_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &dense_dense_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  dense_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &dense_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_dense_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &dense_1_dense_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_output, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &dense_1_output_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  dense_2_dense_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &dense_2_dense_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  dense_2_output, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 6, 1, 1), AI_STRIDE_INIT(4, 4, 4, 24, 24),
  1, &dense_2_output_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_conv2d_weights, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 5, 5), AI_STRIDE_INIT(4, 4, 4, 20, 100),
  1, &conv2d_conv2d_weights_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_conv2d_bias, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &conv2d_conv2d_bias_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_conv2d_weights, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 5, 3, 3, 3), AI_STRIDE_INIT(4, 4, 20, 60, 180),
  1, &conv2d_1_conv2d_weights_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_conv2d_bias, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 1, 1), AI_STRIDE_INIT(4, 4, 4, 12, 12),
  1, &conv2d_1_conv2d_bias_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_2_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_2_layer, 7,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &dense_2_chain,
  NULL, &dense_2_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_2_dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_2_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_2_dense_weights, &dense_2_dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_2_dense_layer, 7,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_2_dense_chain,
  NULL, &dense_2_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_1_layer, 6,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &dense_1_chain,
  NULL, &dense_2_dense_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_1_dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_1_dense_weights, &dense_1_dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_1_dense_layer, 6,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_1_dense_chain,
  NULL, &dense_1_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &dense_chain,
  NULL, &dense_1_dense_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &max_pooling2d_1_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_dense_weights, &dense_dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_dense_layer, 5,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_dense_chain,
  NULL, &dense_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  max_pooling2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &max_pooling2d_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  max_pooling2d_1_layer, 3,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &max_pooling2d_1_chain,
  NULL, &dense_dense_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &conv2d_1_chain,
  NULL, &max_pooling2d_1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &max_pooling2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_1_conv2d_weights, &conv2d_1_conv2d_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_conv2d_layer, 2,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &conv2d_1_conv2d_chain,
  NULL, &conv2d_1_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  max_pooling2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &max_pooling2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  max_pooling2d_layer, 1,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &max_pooling2d_chain,
  NULL, &conv2d_1_conv2d_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_layer, 0,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &conv2d_chain,
  NULL, &max_pooling2d_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_conv2d_weights, &conv2d_conv2d_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_conv2d_layer, 0,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &conv2d_conv2d_chain,
  NULL, &conv2d_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 12936, 1, 1),
    12936, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 13004, 1, 1),
    13004, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_GESTURE_CNN_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_GESTURE_CNN_OUT_NUM, &dense_2_output),
  &conv2d_conv2d_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 12936, 1, 1),
      12936, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 13004, 1, 1),
      13004, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_GESTURE_CNN_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_GESTURE_CNN_OUT_NUM, &dense_2_output),
  &conv2d_conv2d_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool gesture_cnn_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_gesture_cnn_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_0_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 9932);
    input_0_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 9932);
    
    conv2d_conv2d_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 300);
    conv2d_conv2d_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 300);
    
    conv2d_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 300);
    conv2d_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 300);
    
    max_pooling2d_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    max_pooling2d_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    
    conv2d_1_conv2d_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 2800);
    conv2d_1_conv2d_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 2800);
    
    conv2d_1_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    conv2d_1_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    
    max_pooling2d_1_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 1152);
    max_pooling2d_1_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 1152);
    
    dense_dense_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    dense_dense_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    
    dense_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 128);
    dense_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 128);
    
    dense_1_dense_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    dense_1_dense_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    
    dense_1_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 64);
    dense_1_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 64);
    
    dense_2_dense_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    dense_2_dense_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 0);
    
    dense_2_output_array.data = AI_PTR(g_gesture_cnn_activations_map[0] + 24);
    dense_2_output_array.data_start = AI_PTR(g_gesture_cnn_activations_map[0] + 24);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool gesture_cnn_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_gesture_cnn_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    dense_dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_dense_weights_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 0);
    dense_dense_weights_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 0);
    
    dense_dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_dense_bias_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 9216);
    dense_dense_bias_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 9216);
    
    dense_1_dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_1_dense_weights_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 9344);
    dense_1_dense_weights_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 9344);
    
    dense_1_dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_1_dense_bias_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 11392);
    dense_1_dense_bias_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 11392);
    
    dense_2_dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_2_dense_weights_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 11456);
    dense_2_dense_weights_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 11456);
    
    dense_2_dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_2_dense_bias_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 11840);
    dense_2_dense_bias_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 11840);
    
    conv2d_conv2d_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_conv2d_weights_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 11864);
    conv2d_conv2d_weights_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 11864);
    
    conv2d_conv2d_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_conv2d_bias_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 12364);
    conv2d_conv2d_bias_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 12364);
    
    conv2d_1_conv2d_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_conv2d_weights_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 12384);
    conv2d_1_conv2d_weights_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 12384);
    
    conv2d_1_conv2d_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_conv2d_bias_array.data = AI_PTR(g_gesture_cnn_weights_map[0] + 12924);
    conv2d_1_conv2d_bias_array.data_start = AI_PTR(g_gesture_cnn_weights_map[0] + 12924);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_gesture_cnn_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_GESTURE_CNN_MODEL_NAME,
      .model_signature   = AI_GESTURE_CNN_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 92248,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_gesture_cnn_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_GESTURE_CNN_MODEL_NAME,
      .model_signature   = AI_GESTURE_CNN_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 92248,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}

AI_API_ENTRY
ai_error ai_gesture_cnn_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_gesture_cnn_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_gesture_cnn_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_gesture_cnn_create(network, AI_GESTURE_CNN_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_gesture_cnn_data_params_get(&params) != true) {
        err = ai_gesture_cnn_get_error(*network);
        return err;
    }
#if defined(AI_GESTURE_CNN_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_GESTURE_CNN_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_gesture_cnn_init(*network, &params) != true) {
        err = ai_gesture_cnn_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_gesture_cnn_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_gesture_cnn_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_gesture_cnn_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_gesture_cnn_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= gesture_cnn_configure_weights(net_ctx, params);
  ok &= gesture_cnn_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_gesture_cnn_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_gesture_cnn_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_GESTURE_CNN_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

