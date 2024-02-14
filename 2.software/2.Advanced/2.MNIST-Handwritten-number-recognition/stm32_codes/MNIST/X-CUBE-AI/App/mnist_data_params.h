/**
  ******************************************************************************
  * @file    mnist_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    Wed Feb  7 10:18:01 2024
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef MNIST_DATA_PARAMS_H
#define MNIST_DATA_PARAMS_H
#pragma once

#include "ai_platform.h"

/*
#define AI_MNIST_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_mnist_data_weights_params[1]))
*/

#define AI_MNIST_DATA_CONFIG               (NULL)


#define AI_MNIST_DATA_ACTIVATIONS_SIZES \
  { 9264, }
#define AI_MNIST_DATA_ACTIVATIONS_SIZE     (9264)
#define AI_MNIST_DATA_ACTIVATIONS_COUNT    (1)
#define AI_MNIST_DATA_ACTIVATION_1_SIZE    (9264)



#define AI_MNIST_DATA_WEIGHTS_SIZES \
  { 68128, }
#define AI_MNIST_DATA_WEIGHTS_SIZE         (68128)
#define AI_MNIST_DATA_WEIGHTS_COUNT        (1)
#define AI_MNIST_DATA_WEIGHT_1_SIZE        (68128)



#define AI_MNIST_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_mnist_activations_table[1])

extern ai_handle g_mnist_activations_table[1 + 2];



#define AI_MNIST_DATA_WEIGHTS_TABLE_GET() \
  (&g_mnist_weights_table[1])

extern ai_handle g_mnist_weights_table[1 + 2];


#endif    /* MNIST_DATA_PARAMS_H */
