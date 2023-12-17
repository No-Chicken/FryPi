#导入工具包
import tensorflow as tf
import pandas as pd
import numpy as np

#读取数据
data = pd.read_csv('./Embedded_things/sin_values.csv', sep=',', header=None)
raw_x = data.iloc[:,0].astype(float)
sinex = data.iloc[:,1].astype(float)
print(sinex.shape)

#建立模型
model = tf.keras.Sequential()
model.add(tf.keras.layers.Dense(units=10, activation='tanh', input_shape=(1,)))
model.add(tf.keras.layers.Dense(units=5, activation='tanh'))
model.add(tf.keras.layers.Dense(units=1))
model.summary()

model.compile(optimizer=tf.keras.optimizers.AdamW(0.001),
             loss=tf.keras.losses.mse,                      #loss使用均方差，刚才的分类用的交叉熵
             metrics=[tf.keras.metrics.mse])
history = model.fit(x=raw_x, y=sinex, epochs=2000)

print(model.evaluate(raw_x, sinex))

#保存模型
model.save('./Embedded_things/sine_calcu.h5')

#转换模型为tf lite格式 不量化
load_model = tf.keras.models.load_model('./Embedded_things/sine_calcu.h5')
converter = tf.lite.TFLiteConverter.from_keras_model(load_model)
tflite_model = converter.convert()
open("./Embedded_things/sine_calcu.tflite", "wb").write(tflite_model)

