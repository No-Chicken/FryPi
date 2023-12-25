使用了如下函数进行了print重定向，记得勾选use Mirco LIB

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

如果需要不定长发送，可以看一下如何使用Idel空闲标志位如何使用，结合DMA一起使用