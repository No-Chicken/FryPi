/**
  ******************************************************************************
  * @file    network_sine_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sun Dec 17 14:25:55 2023
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_sine_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_sine_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_sine_weights_array_u64[41] = {
  0x3eeb727a3ecdb4bbU, 0xbf05ab973f1e4ae1U, 0xbd4dffc33f0f15f7U, 0x3d073e6c3f596ffdU,
  0x3cb5259cbcb6396fU, 0xbf66f7d33d2c6dcaU, 0xbf322f01bd84e8e9U, 0xbda8ef223e15caecU,
  0xbfd29b293f2dc7ccU, 0xc04f9245404506f2U, 0xbf170bbcbe7b3d4bU, 0x3efc0b1b3f92a7daU,
  0x3e7ebe79bf3fa417U, 0xbea3a7c2bf1364f1U, 0xbebd6a003f18e438U, 0xbe1849aa3e809490U,
  0xbebeacebbf0016b1U, 0xbe82eb9c3ef1536eU, 0xbd2fc77e3dade46aU, 0xbee424bdbe9a5eb9U,
  0xbea9ceb53e3bc59fU, 0x3efa7aa13da91c0eU, 0x3d386f58bf22b769U, 0x3ea52327be94cb00U,
  0x3e396c8f3e944674U, 0x3edc96213ed0f519U, 0xbdaf8177be1307c4U, 0xbdc26ec53e2edc23U,
  0xbe9a6a793e49b4abU, 0x3ea03f64be8a70f5U, 0xbf0cf8d2bdbf013aU, 0x3d8401d1bccdc4e5U,
  0xbe9959bebeefb0efU, 0x3f163c753cef9052U, 0xbfc4212b3fb9c281U, 0x3dabc6853e8accb6U,
  0xbe92cbebbe5e7831U, 0x3d960ea93f6a1352U, 0xbdb7c899bd6a45a8U, 0x3de324cfbda909e1U,
  0x3eda1169U,
};


ai_handle g_network_sine_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_sine_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

