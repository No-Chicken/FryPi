import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import datetime

f = np.load("Embedded_things\\thermal\\camera_data\\train_data.npz")
x_train, y_train = f['x_train'],f['y_train']
f.close()

print(x_train.shape, y_train.shape)

#------------------------------【查看数据】---------------------------------

image_index=230
print(y_train[image_index])     #查看随机一张图片的label
x_train = x_train.astype('float32')  #数据类型转换

# 计算最大值和最小值
max_value = np.max(x_train)
min_value = np.min(x_train)

# 最大最小值归一化操作
x_train_normalized = (x_train - min_value) / (max_value - min_value)

x_train = x_train_normalized.reshape(x_train.shape[0], 24, 32, 1)  #数据维度转换
plt.imshow(x_train[image_index])  #图片显示
plt.show()

#------------------------------【搭模型】---------------------------------
model = tf.keras.models.Sequential([
    tf.keras.layers.Conv2D(filters=5, kernel_size=(5, 5), padding='valid', activation=tf.nn.relu, input_shape=(24, 32, 1)),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2), padding='same'),
    tf.keras.layers.Conv2D(filters=3, kernel_size=(3, 3), padding='valid', activation=tf.nn.relu, input_shape=(10, 14, 5)),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2), padding='same'),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(units=32, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=16, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=6, activation=tf.nn.softmax)
])

model.summary()
exit()
#------------------------------【训练】---------------------------------

#超参数设置
num_epochs = 50
batch_size = 64
learning_rate = 0.001

#优化器
adam_optimizer = tf.keras.optimizers.Adam(learning_rate)

#编译
model.compile(optimizer=adam_optimizer,
               loss=tf.keras.losses.sparse_categorical_crossentropy,
               metrics=['accuracy'])

#训练
start_time = datetime.datetime.now()

model.fit(x=x_train,
         y=y_train,
         batch_size=batch_size,
         epochs=num_epochs)

endtime = datetime.datetime.now()

time_cost = endtime - start_time
print('time_cost = ', time_cost)
#保存/加载模型
model.save('Embedded_things\\thermal\\gesture.h5')

#------------------------------【评估】---------------------------------

f = np.load("Embedded_things\\thermal\\camera_data\\test_data.npz")
x_test, y_test = f['x_test'],f['y_test']
f.close()
x_test = x_test.astype('float32')  #数据类型转换

# 计算最大值和最小值
max_value = np.max(x_test)
min_value = np.min(x_test)

# 最大最小值归一化操作
x_test_normalized = (x_test - min_value) / (max_value - min_value)

x_test = x_test_normalized.reshape(x_test.shape[0], 24, 32)
print(x_test.shape, y_test.shape)

model = tf.keras.models.load_model('Embedded_things\\thermal\\gesture.h5')
model.summary()
print(model.evaluate(x_test, y_test))

#------------------------------【预测】---------------------------------

image_index = 151
plt.imshow(x_test[image_index])
plt.show()
mod_input = x_test[image_index].reshape(1, 24, 32, 1)
print(mod_input.shape)
pred = model.predict(mod_input)
print("实际数字为: ", y_test[image_index])
print("预测数字为: ", pred.argmax())

