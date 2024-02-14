/**
  ******************************************************************************
  * @file    lite_upsample.h
  * @author  AIS
  * @brief   header file of AI platform lite pw kernel datatypes
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  @endverbatim
  ******************************************************************************
  */
#ifndef LITE_UPSAMPLE_H
#define LITE_UPSAMPLE_H
#pragma once

#include "ai_lite_interface.h"


void forward_lite_upsample_bilinear_if32of32(const ai_float* in_data,
                                    ai_float* out_data,
                                    const ai_size width_in,
                                    const ai_size height_in,
                                    const ai_float width_scale,
                                    const ai_float height_scale,
                                    const ai_size width_out,
                                    const ai_size height_out,
                                    const ai_bool center,
                                    const ai_size n_channel);
void forward_lite_upsample_bilinear_is8os8(const ai_i8* in_data,
                                    ai_i8* out_data,
                                    const ai_size width_in,
                                    const ai_size height_in,
                                    const ai_float width_scale,
                                    const ai_float height_scale,
                                    const ai_size width_out,
                                    const ai_size height_out,
                                    const ai_bool center,
                                    const ai_size n_channel);
void forward_lite_upsample_bilinear_iu8ou8(const ai_u8* in_data,
                                    ai_u8* out_data,
                                    const ai_size width_in,
                                    const ai_size height_in,
                                    const ai_float width_scale,
                                    const ai_float height_scale,
                                    const ai_size width_out,
                                    const ai_size height_out,
                                    const ai_bool center,
                                    const ai_size n_channel);
void forward_lite_upsample_bilinear_is16os16(const ai_i16* in_data,
                                    ai_i16* out_data,
                                    const ai_size width_in,
                                    const ai_size height_in,
                                    const ai_float width_scale,
                                    const ai_float height_scale,
                                    const ai_size width_out,
                                    const ai_size height_out,
                                    const ai_bool center,
                                    const ai_size n_channel);
void forward_lite_upsample_bilinear_iu16ou16(const ai_u16* in_data,
                                    ai_u16* out_data,
                                    const ai_size width_in,
                                    const ai_size height_in,
                                    const ai_float width_scale,
                                    const ai_float height_scale,
                                    const ai_size width_out,
                                    const ai_size height_out,
                                    const ai_bool center,
                                    const ai_size n_channel);


#endif    /*LITE_UPSAMPLE__H*/
