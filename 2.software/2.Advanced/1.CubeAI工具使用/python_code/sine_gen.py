import pandas as pd
import numpy as np

# 创建一个包含x的NumPy数组，从1到360，间隔为1
x_values = np.arange(1, 361)

# 计算每个x对应的sin(x)值
sin_values = np.sin(np.radians(x_values))  # 使用np.radians将角度转换为弧度

# 创建一个包含x和sin(x)的Pandas DataFrame
data = pd.DataFrame({'x': x_values, 'sin(x)': sin_values})

# 将数据保存为CSV文件
data.to_csv('./Embedded_things/sin_values.csv', index=False)

