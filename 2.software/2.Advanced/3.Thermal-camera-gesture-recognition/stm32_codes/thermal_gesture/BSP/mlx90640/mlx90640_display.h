/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLX90640_LCD_DISPLAY_H
#define __MLX90640_LCD_DISPLAY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
	 
#include "main.h"
#include "spi.h"

#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) 

#define min(a,b)  ((a)<(b)?(a):(b))

#define max(a,b)  ((a)>(b)?(a):(b))

#define abs(x)    ((x)>0?(x):(-x))

void mlx90640_display_init(void);
void mlx90640_display_process(void);


#ifdef __cplusplus
}
#endif

#endif /* __MLX90640_LCD_DISPLAY_H */
