import machine
import urequests
import ujson as json
from machine import Pin,ADC
import network, time
import dht

dht_sensor = dht.DHT11(machine.Pin(33)) #kết nối cảm biến dht với chân 33 (A2 trên Shield)
soil = ADC(Pin(32)) # kết nối cảm biến với chân 32 (A3 trên shield)
soil.atten(ADC.ATTN_11DB)
light= ADC (Pin(34)) #kết nối cảm biến với chân 34 (A1 trên shield)
light.atten(ADC.ATTN_11DB)
led_pin=13
led = Pin(led_pin, Pin.OUT)


#Thông tin tài khoản Thingspeak 
HTTP_HEADERS = {'Content-Type': 'application/json'} 
THINGSPEAK_WRITE_API_KEY = '41AN3MHDXP6GL9JX' 

ssid='Tenda_CAD7D8' #tên mạng wifi
password='taolakhanh' #password wifi

# Cài đặt kết nối wifi
sta_if=network.WLAN(network.STA_IF)
sta_if.active(True)

if not sta_if.isconnected():
    print('connecting to network...')
    sta_if.connect(ssid, password)
    while not sta_if.isconnected():
     pass
print('network config:', sta_if.ifconfig()) 

field1_request = 'https://api.thingspeak.com/channels/2611173/fields/1/last.json'

def controlLED ():
    if light_value < 200:
        led.value(0)
    else:
        led.value(1)
while True:
    dht_sensor.measure()
    temperature = dht_sensor.temperature() #đọc giá trị nhiệt độ
    humidity = dht_sensor.humidity() #đọc giá trị độ ẩm
    soil_value=soil.read()
    light_value=light.read()
    sensor_reading = {'field1':temperature, 'field2':humidity, 'field3':soil_value, 'field4':light_value} #nối các giá trị thành 1 chuỗi để gửi 
    print(sensor_reading) #in 2 giá trị gửi
    print (light_value)
    request = urequests.post( 'http://api.thingspeak.com/update?api_key=' + THINGSPEAK_WRITE_API_KEY,json = sensor_reading, headers = HTTP_HEADERS )  
    request.close()    
    controlLED ()
    #đọc giá trị từ field5 (nút nhấn) và bật-tắt đèn tương ứng
    
    time.sleep(0.1)
'''from machine import Pin
import time
led_pin=13
led = Pin(led_pin, Pin.OUT)
while True:
    led.value(0)
    time.sleep_ms(500)
    led.value(1)
    time.sleep_ms(500)'''