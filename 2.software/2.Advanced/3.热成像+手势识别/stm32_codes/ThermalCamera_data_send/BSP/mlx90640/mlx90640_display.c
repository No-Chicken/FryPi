#include <string.h>
#include "mlx90640_display.h"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "lcd.h"

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS8HZ 
#define EMMISIVITY 0.95f
#define TA_SHIFT 8 //Default shift for MLX90640 in open air

#define PIC_MUL 8
#define PIC_SIZE 24*8*32*8
#define LCD_Buf_Size 24576//24*8*32*4
// static uint8_t lcd_buf[LCD_Buf_Size];

paramsMLX90640 mlx90640;
static uint16_t eeMLX90640[832];  
int status;

// start with some initial colors
float minTemp = 20.0f;
float maxTemp = 40.0f;
float centerTemp;

char tempBuffer[6];
// variables for interpolated colors
uint8_t red, green, blue;

// variables for row/column interpolation
float intPoint, val, a, b, c, d, ii;
int x, y, i, j;

// array for the 32 x 24 measured tempValues
static float tempValues[32*24];


static uint16_t TempToColor(float val);
static void setTempScale(void);
static void setAbcd(void);
static void readTempValues(void);

static uint16_t TempToColor(float val){
	  /*
    pass in value and figure out R G B
    several published ways to do this I basically graphed R G B and developed simple linear equations
    again a 5-6-5 color display will not need accurate temp to R G B color calculation

    equations based on
    http://web-tech.ga-usa.com/2012/05/creating-a-custom-hot-to-cold-temperature-color-gradient-for-use-with-rrdtool/index.html

  */

  red = constrain(255.0f / (c - b) * val - ((b * 255.0f) / (c - b)), 0, 255);

  if ((val > minTemp) & (val < a)) {
    green = constrain(255.0f / (a - minTemp) * val - (255.0f * minTemp) / (a - minTemp), 0, 255);
  }
  else if ((val >= a) & (val <= c)) {
    green = 255;
  }
  else if (val > c) {
    green = constrain(255.0f / (c - d) * val - (d * 255.0f) / (c - d), 0, 255);
  }
  else if ((val > d) | (val < a)) {
    green = 0;
  }

  if (val <= b) {
    blue = constrain(255.0f / (a - b) * val - (255.0f * b) / (a - b), 0, 255);
  }
  else if ((val > b) & (val <= d)) {
    blue = 0;
  }
  else if (val > d) {
    blue = constrain(240.0f / (maxTemp - d) * val - (d * 240.0f) / (maxTemp - d), 0, 240);
  }

  // use the displays color mapping function to get 5-6-5 color palet (R=5 bits, G=6 bits, B-5 bits)
  return (((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue) >> 3));
}

static void setTempScale(void) {
  minTemp = 255;
  maxTemp = 0;

  for (i = 0; i < 768; i++) {
    minTemp = min(minTemp, tempValues[i]);
    maxTemp = max(maxTemp, tempValues[i]);
  }

  setAbcd();
}


// Function to get the cutoff points in the temp vs RGB graph.
static void setAbcd(void) {
  a = minTemp + (maxTemp - minTemp) * 0.2121f;
  b = minTemp + (maxTemp - minTemp) * 0.3182f;
  c = minTemp + (maxTemp - minTemp) * 0.4242f;
  d = minTemp + (maxTemp - minTemp) * 0.8182f;
}


// Read pixel data from MLX90640.
static void readTempValues(void) {

  for (uint8_t x = 0 ; x < 2 ; x++) // Read both subpages
  {
    uint16_t mlx90640Frame[834];
    status = MLX90640_GetFrameData(MLX90640_ADDR, mlx90640Frame);
    if (status < 0)
    {
      printf("GetFrame Error: %d\r\n",status);
    }

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, EMMISIVITY, tr, tempValues);
  }
}

static void drawPicture(void) {

  static uint8_t lcd_buf[LCD_Buf_Size];//able to store 32*8 * 6*8 pixes
  uint16_t color;
  uint16_t indexes=0;
  uint8_t refreshtimes=0;
  uint8_t data[2];
  LCD_Address_Set(12+20 , 24 , 12+20+32*8-1, 24+24*8-1);//设置显示范围
  //pix_horizontal
	for (y=0; y<24; y++) {
    //pix_vertical
		for (x=0; x<32; x++) {
      //FillRect(Xpos, Ypos, Width, Height, RGBCode);

			//LCD_Fill(12 + x*8, 24 + y*8, 12 + x*8 + 8, 24 + y*8 + 8, TempToColor(tempValues[(31-x) + (y*32)]));
      uint16_t index_sta = 32*8*8*2*y + 8*2*x - refreshtimes*(LCD_Buf_Size);
      color = TempToColor(tempValues[(31-x) + (y*32)]);
      data[0] = color>>8;
      data[1] = color&0xff;
      for(uint8_t i=0; i<8 ; i++) {
        uint16_t line_gap = 32*8*2*i; 
        for(uint8_t j=0; j<8 ; j++) {
          lcd_buf[index_sta + line_gap + 2*j + 0] = data[0];
          lcd_buf[index_sta + line_gap + 2*j + 1] = data[1];
          indexes += 1;
        }
      }
      
      
      if(indexes>=LCD_Buf_Size/2) {
        indexes = 0;
        refreshtimes+=1;
        if(HAL_SPI_Transmit(&hspi1, lcd_buf, LCD_Buf_Size, 0))
	      {printf("\r\nspi erro\r\n");}	
      }
		}
	}
}

void mlx90640_display_process(void)
{
	readTempValues();
  printf("\r\nbegin,");
  for(uint16_t i=0; i<32*24; i++)
  {
    printf("%.1f,",tempValues[i]);
  }
  printf("end,\r\n");
  setTempScale();
  //draw legend

  //draw pic
  drawPicture();

}

void mlx90640_display_init(void){

	status = MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
  if (status != 0) printf("\r\nset refresh erro code:%d\r\n",status);
	status = MLX90640_SetChessMode(MLX90640_ADDR);
	if (status != 0) printf("\r\nset chess erro code:%d\r\n",status);
  status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
  if (status != 0) printf("\r\nload system parameters error with code:%d\r\n",status);
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0) printf("\r\nParameter extraction failed with error code:%d\r\n",status);
}

