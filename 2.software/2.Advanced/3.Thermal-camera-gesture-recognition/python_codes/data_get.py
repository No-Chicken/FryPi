import serial
import numpy as np

# 串口配置
ser = serial.Serial('COM10', baudrate=115200, timeout=None)

# 初始化变量
data_list = []
y_train_list = []
max_data_count = 200  # 设置接收的最大数据组数

try:
    data_count = 0

    while data_count < max_data_count:
        line = ser.readline().decode('utf-8').strip()
        # 检查数据是否完整
        if line.startswith('begin,') and line.endswith('end,'):
            # 提取data部分并转换为浮点数列表
            data_str = line[len('begin,'):-len('end,\r\n')].strip()
            data_float = [float(num) for num in data_str.split(',')]

            # 将数据加入列表
            data_list.append(data_float)

            # 添加对应的y_train值
            y_train_list.append(5)  # y_train值

            # 更新计数器
            data_count += 1
            print(data_count)

except KeyboardInterrupt:
    # 中断时执行的代码
    pass

finally:
    # 将数据转换为NumPy数组
    x_train = np.array(data_list)
    y_train = np.array(y_train_list)

    # 保存为.npz文件
    np.savez('Embedded_things\\thermal\camera_data\\g5_train_data.npz', x_train=x_train, y_train=y_train)

    # 关闭串口
    ser.close()




# import numpy as np

# # 定义合并后的文件路径
# merged_file_path = 'Embedded_things\\thermal\camera_data\merged_data2.npz'

# # 初始化合并后的数据
# merged_x_train = np.empty((0, 768))
# merged_y_train = np.empty((0,))

# # 定义多个文件路径
# file_paths = ['Embedded_things\\thermal\camera_data\merged_data.npz', 'Embedded_things\\thermal\camera_data\\train_data.npz']  # 替换为你实际的文件路径

# for file_path in file_paths:
#     data = np.load(file_path)
#     x_train = data['x_train']
#     y_train = data['y_train']

#     # 合并数据
#     merged_x_train = np.vstack((merged_x_train, x_train))
#     merged_y_train = np.concatenate((merged_y_train, y_train))

#     data.close()  # 关闭文件

# # 保存合并后的数据为新的.npz文件
# np.savez(merged_file_path, x_train=merged_x_train, y_train=merged_y_train)






# import numpy as np

# # 加载合并后的.npz文件
# merged_data = np.load('Embedded_things\\thermal\camera_data\\t.npz')

# # 获取合并后的数据
# merged_x_train = merged_data['x_train']
# merged_y_train = merged_data['y_train']

# # 将y_train的值类型全部改为int32
# merged_y_train = merged_y_train.astype(np.int32)

# # 保存修改后的数据为新的.npz文件
# np.savez('train_data.npz', x_train=merged_x_train, y_train=merged_y_train)

# # 关闭文件
# merged_data.close()

