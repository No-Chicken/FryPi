#ifndef LITE_BN_INTEGER_H
#define LITE_BN_INTEGER_H
#pragma once

#include "ai_lite_interface.h"

/**
 * @brief Batch Normalization with 16-bit input, 16-bit threshold and binary output. 
 * It is implemented using a threshold, and this is possible because the output is binary.
 * 
 * @param[in] pIn Input data pointer
 * @param[out] pOut_32 Output data pointer
 * @param[in] pThreshold Thresholds pointer (one per channel)
 * @param[in] dim_x X dimension
 * @param[in] dim_y Y dimension
 * @param[in] channels_num Channels number
 */
LITE_API_ENTRY
void forward_lite_bn_is16os1ws16(const ai_i16 *pIn,
                                 ai_u32 *pOut_32,
                                 const ai_i16 *pThreshold,
                                 const ai_i16 dim_x,
                                 const ai_i16 dim_y,
                                 const ai_i16 channels_num);
#endif    /* LITE_BN_INTEGER_H */
