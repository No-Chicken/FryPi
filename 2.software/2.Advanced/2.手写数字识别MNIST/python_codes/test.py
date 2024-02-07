import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import datetime

f = np.load("E:\projects\programs\python_codes\Embedded_things\LeNetTest\mnist.npz")
x_train, y_train = f['x_train'],f['y_train']
x_test, y_test = f['x_test'],f['y_test']
f.close()

print(x_train.shape, y_train.shape)
print(x_test.shape, y_test.shape)

#------------------------------【查看数据】---------------------------------
image_index = 123
print("label:",y_train[image_index])     #查看随机一张图片的label

x_train = np.pad(x_train, ((0, 0), (1, 1), (1, 1)), 'constant', constant_values=0)  #将图片从28*28扩展成30*30
x_train = x_train.astype('float32')  #数据类型转换
x_train /= 255  #数据正则化
x_train = x_train.reshape(x_train.shape[0], 30, 30, 1)  #数据维度转换
plt.imshow(x_train[image_index], cmap='Greys')  #图片显示
plt.show()

#------------------------------【搭模型】---------------------------------
model = tf.keras.models.Sequential([
    tf.keras.layers.Conv2D(filters=3, kernel_size=(5, 5), padding='valid', activation=tf.nn.relu, input_shape=(30, 30, 1)),
    tf.keras.layers.MaxPool2D(pool_size=(2, 2), padding='same'),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(units=32, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=16, activation=tf.nn.relu),
    tf.keras.layers.Dense(units=10, activation=tf.nn.softmax)
])

model.summary()
exit()
#------------------------------【训练】---------------------------------

#超参数设置
num_epochs = 10
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
model.save('Embedded_things\LeNetTest\mnist.h5')

#------------------------------【评估】---------------------------------

x_test = np.pad(x_test, ((0, 0), (1, 1), (1, 1)), 'constant', constant_values=0)  #将图片从28*28扩展成30*30
print(x_test.shape)
print(x_test[0])
model = tf.keras.models.load_model('Embedded_things\LeNetTest\mnist.h5')
model.summary()
print(model.evaluate(x_test, y_test))

#------------------------------【预测】---------------------------------

image_index = 2336
plt.imshow(x_test[image_index].reshape(30, 30), cmap='Greys')
plt.show()

pred = model.predict(x_test[image_index].reshape(1, 30, 30, 1))
print("实际数字为: ",y_test[image_index])
print("预测数字为: ",pred.argmax())

