import numpy as np
import matplotlib.pyplot as plt

# 加载.npz文件
data = np.load('Embedded_things\\thermal\camera_data\\train_data.npz')

# 获取数据
x_test = data['x_train']
y_test = data['y_train']

# 查看数据形状
print("x_test shape:", x_test.shape)
print("y_test shape:", y_test.shape)

# 将一维数组转换为二维数组（32x24）
two_dimensional_array = x_test.reshape(x_test.shape[0], 24, 32)

# 绘制图表
image_index = 4500
while image_index < x_test.shape[0]:
    plt.imshow(two_dimensional_array[image_index], cmap='viridis')
    plt.title("2D Array Visualization")
    plt.show()
    print("label:",y_test[image_index],"type:",type(y_test[image_index]))
    image_index+=1



