#ifndef LITE_BN_F32_H
#define LITE_BN_F32_H
#pragma once

#include "ai_lite_interface.h"

/*!
 * @brief Forward function for a batch normalization (BN) layer with 
 * signed float input, signed float output, and float parameters.
 * @ingroup lite_bn_f32
 * @param output The pointer to output buffer.
 * @param input The pointer to input buffer.
 * @param scale The pointer to BN scale param.
 * @param bias The pointer to bias.
 * @param n_elements The number of elements in the input tensor.
 * @param n_channel_in The number of channel in the input tensor.
 */
LITE_API_ENTRY
void forward_lite_bn_if32of32wf32(
  ai_float* output, const ai_float* input,
  const ai_float* scale, const ai_float* bias,
  const ai_u32 n_elements, const ai_u32 n_channel_in);


#endif    /* LITE_BN_F32_H */
