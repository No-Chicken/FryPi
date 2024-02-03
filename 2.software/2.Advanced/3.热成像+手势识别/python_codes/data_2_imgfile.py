import os
import numpy as np
import matplotlib.pyplot as plt

# 创建保存图像的文件夹
output_folder = 'images'
os.makedirs(output_folder, exist_ok=True)

# 加载.npz文件
data = np.load('Embedded_things\\thermal\camera_data\\g1_train_data.npz')

# 获取数据
x_train = data['x_train']
y_train = data['y_train']
x_train = x_train.reshape(x_train.shape[0], 24, 32)

# 遍历每个样本
for i in range(x_train.shape[0]):
    # 创建图像文件名，可以根据需要调整文件名格式
    image_filename = os.path.join(output_folder, f"sample_{i+1}_label_{y_train[i]}.png")

    # 显示图像并保存
    plt.imshow(x_train[i], cmap='viridis')  # 假设x_train的每个元素是一个2D数组
    plt.title(f"Sample {i+1}, Label {y_train[i]}")
    plt.savefig(image_filename)
    plt.close()

# 关闭文件
data.close()
