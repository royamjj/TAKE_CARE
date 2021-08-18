# minimum temperature in refrigerator 0.68dc, all values are assumed accordingly
import conf
import numpy as np
from sklearn.linear_model import LinearRegression
from boltiot import Bolt
import requests
import json, time, math, statistics
my_device = Bolt(conf.api_key, conf.device_id)
x = [i for i in range(50)]
minimum_temperature = 0
temp_range = 1
threshold = 10
temperature_values = []


def send_telegram_message(message):
    url = 'https://api.telegram.org/' + conf.telegram_bot_id + "/sendMessage"
    data = {"chat_id": conf.telegram_chat_id, "text":message}
    try:
        response = requests.request("GET", url, params = data)
        telegram_data = json.loads(response.text)
    except Exception as e:
        print(e)


def compute_bounds(data, frame_size, factor):
    if len(data) < frame_size:
        return None
    elif len(data) == frame_size:
        pass
    else:
        del data[0:len(data) - frame_size]

    mean_for_Z = statistics.mean(data)
    var_for_Z = sum([(i-mean_for_Z)**2 for i in data])
    z_for_Z = factor * math.sqrt(var_for_Z/frame_size)
    High_bound = data[frame_size-1] + z_for_Z
    Low_bound = data[frame_size-1] - z_for_Z
    return [High_bound, Low_bound]


def regression(data_x, data_y, time_of_prediction):
    no_predictions = int((time_of_prediction*60) / 10)
    test_x = np.reshape([[i] for i in data_x], (len(data_x), 1))
    test_y = np.reshape([[i] for i in data_y], (len(data_y), 1))
    model = LinearRegression()
    model.fit(test_x, test_y)
    x_to_predict = np.reshape([[i] for i in range(51, no_predictions+51)], (no_predictions, 1))
    predictions = model.predict(x_to_predict)
    return predictions


while True:
    temp_data = my_device.analogRead('A0')
    response = json.loads(temp_data)
    if response['success'] != 1:
        print('Error reading sensor value, error is : ' + response['value'])
        continue
    try:
        sensor_value = int(response['value'])
    except Exception as e:
        print("There was an error while parsing the response: ", e)
        continue
    v = round(float(sensor_value / 10.24), 3)
    if v >= threshold:
        send_telegram_message('Temperature has crossed threshold!')
    print(v)
    temperature_values.append(v)
    bound = compute_bounds(temperature_values, conf.frame_size, conf.factor)
    if not bound:
        print('Not enough data, required {} more'.format(conf.frame_size - len(temperature_values)))
        time.sleep(5)
        continue
    print('high bound {} low bound {}'.format(bound[0], bound[1]))
    pre = regression(x, temperature_values, 20)
    if pre.min() >= temp_range:
        send_telegram_message("The temperature can rise in next 20 minutes! Take care.")
        time.sleep(10)
        continue
    if v >= bound[0]:
        send_telegram_message('ANOMALY DETECTED!')
    time.sleep(20)