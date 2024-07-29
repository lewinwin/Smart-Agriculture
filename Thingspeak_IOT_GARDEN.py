import machine
import urequests
import ujson as json
from machine import Pin,ADC
import network, time
import dht

dht_sensor = dht.DHT11(machine.Pin(34)) #kết nối cảm biến dht với chân 34 (A1 trên Shield)
soil = ADC(Pin(32)) # kết nối cảm biến với chân 32 (A3 trên shield)
soil.atten(ADC.ATTN_11DB)
light = ADC (Pin(33)) #kết nối cảm biến với chân 33 (A2 trên shield)
light.atten(ADC.ATTN_11DB)
led = Pin(2, Pin.OUT)
pump = Pin(3, Pin.OUT)

#Thông tin tài khoản Thingspeak 
HTTP_HEADERS = {'Content-Type': 'application/json'} 
THINGSPEAK_WRITE_API_KEY = '3R5JP1O402P3BLKZ' 

ssid='MakerLab.vn' #tên mạng wifi
password='' #password wifi

# Cài đặt kết nối wifi
sta_if=network.WLAN(network.STA_IF)
sta_if.active(True)

if not sta_if.isconnected():
    print('connecting to network...')
    sta_if.connect(ssid, password)
    while not sta_if.isconnected():
     pass
print('network config:', sta_if.ifconfig()) 

field1_request = 'https://api.thingspeak.com/channels/2609150/fields/1/last.json'

while True:
    dht_sensor.measure()
    temperature = dht_sensor.temperature() #đọc giá trị nhiệt độ
    humidity = dht_sensor.humidity() #đọc giá trị độ ẩm
    soil_value=soil.read()
    light_value=light.read()
    sensor_reading = {'field1':temperature, 'field2':humidity, 'field3':soil_value,'field4':light_value} #nối các giá trị thành 1 chuỗi để gửi
    print(sensor_reading) #in 2 giá trị gửi
    request = urequests.post( 'http://api.thingspeak.com/update?api_key=' + THINGSPEAK_WRITE_API_KEY,json = sensor_reading, headers = HTTP_HEADERS )  
    request.close()
    controlLED()
    controlWaterPump()
    request.close()
		#đọc giá trị từ field5 (nút nhấn) và bật-tắt đèn tương ứng
    
    time.sleep(2)
def controlLED ():
    if (light.read()<200):
        led.value(1)
    else:
        led.value(0)
def controlWaterPump ():
    if (soil.read() <512):
        pump.value(1)
        while (soil.read()<614):
            time.sleep(1)
        pump.value(0)