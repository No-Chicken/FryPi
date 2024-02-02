import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

model = tf.keras.models.load_model('./Embedded_things/sine/sine_calcu.h5')
#model = tf.keras.models.load_model('./Embedded_things/sine_calcu.tflite')

true_values = [np.sin(i*np.pi/180) for i in range(0,360)]

predictions = model.predict([i for i in range(0,360)])

plt.plot(true_values)
plt.plot(predictions)
plt.show()

