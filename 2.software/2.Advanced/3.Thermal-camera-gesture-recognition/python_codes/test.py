import serial
import numpy as np
import tensorflow as tf

# 串口配置
ser = serial.Serial('COM10', baudrate=115200, timeout=None)

# 初始化变量
data_list = []
y_train_list = []

# model = tf.keras.models.load_model('gesture.h5')
model = tf.keras.models.load_model('Embedded_things\\thermal\\gesture.h5')
try:

    while 1:
        line = ser.readline().decode('utf-8').strip()
        # 检查数据是否完整
        if line.startswith('begin,') and line.endswith('end,'):
            # 提取data部分并转换为浮点数列表
            data_str = line[len('begin,'):-len('end,\r\n')].strip()
            data_float = [float(num) for num in data_str.split(',')]
            x_test = np.array(data_float)
            max_value = np.max(x_test)
            min_value = np.min(x_test)

            # 最大最小值归一化操作
            x_train_normalized = (x_test - min_value) / (max_value - min_value)
            x_test = x_train_normalized.reshape(1, 24, 32, 1)  #数据维度转换
            pred = model.predict(x_test)
            print("预测数字为: ",pred.argmax())
            

except KeyboardInterrupt:
    # 中断时执行的代码
    pass

finally:
    # 关闭串口
    ser.close()
