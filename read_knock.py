from urllib.request import urlopen
from pynput.keyboard import Key, Controller
import time

keyboard = Controller()

import json
# reference the json file that looks up the knock value stored in the database stores it in the json file
url = "http://** your domain here ** /knock.json"

response = urlopen(url)
prev = json.loads(response.read())

while True:
    # store the response of URL
    response = urlopen(url)

    # storing the JSON response
    # from url in data
    data_json = json.loads(response.read())

    # print the json response
    print(data_json)

    if (prev != data_json):
        print("Pass")
        keyboard.press(Key.space)

    prev = data_json
    time.sleep(0.5)