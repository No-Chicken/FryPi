import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

def run_reference_by_tflite(input):
    interpreter = tf.lite.Interpreter(model_path="./Embedded_things/sine_calcu.tflite")
    interpreter.allocate_tensors()
    # Get input and output tensors.
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    # input details
    print(input_details, len(input_details))
 
    # output details
    print(output_details)
 
    # input_details[0]['index'] = the index which accepts the input
    interpreter.set_tensor(input_details[0]['index'], input)
 
 
    # run the inference
    interpreter.invoke()
 
    # output_details[0]['index'] = the index which provides the input
    output_data = interpreter.get_tensor(output_details[0]['index'])
    print('interpreter: ', output_data)
 
run_reference_by_tflite([90])

model = tf.keras.models.load_model('./Embedded_things/sine_calcu.h5')
#model = tf.keras.models.load_model('./Embedded_things/sine_calcu.tflite')

true_values = [np.sin(i*np.pi/180) for i in range(0,360)]

predictions = model.predict([i for i in range(0,360)])

plt.plot(true_values)
plt.plot(predictions)
plt.show()

