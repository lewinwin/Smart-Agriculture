import machine
import urequests
import ujson as json
from machine import Pin,ADC
import network, time
import dht
from machine import SoftI2C
from lcd_api import LcdApi
from i2c_lcd import I2cLcd
from time import sleep


I2C_ADDR = 0x27
totalRows = 2
totalColumns = 16

i2c = SoftI2C(scl=Pin(22), sda=Pin(21), freq=10000)     #initializing the I2C method for ESP32
#i2c = I2C(scl=Pin(5), sda=Pin(4), freq=10000)       #initializing the I2C method for ESP8266

lcd = I2cLcd(i2c, I2C_ADDR, totalRows, totalColumns)

dht_sensor = dht.DHT11(machine.Pin(33)) #kết nối cảm biến dht với chân 33 (A2 trên Shield)
soil = machine.ADC(machine.Pin(34)) # kết nối cảm biến với chân 34 (A1 trên shield)
soil.atten(machine.ADC.ATTN_11DB)
light= machine.ADC(machine.Pin(32)) #kết nối cảm biến với chân 32 (A3 trên shield)
light.atten(machine.ADC.ATTN_11DB)
led_pin=13 # chân D9
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
    if light_value < 2000:
        led.value(0)
    else:
        led.value(1)
while True:
    dht_sensor.measure()
    temperature = dht_sensor.temperature() #đọc giá trị nhiệt độ
    humidity = dht_sensor.humidity() #đọc giá trị độ ẩm
    soil_value=soil.read()
    light_value=light.read()
    sensor_reading = {'field1':temperature, 'field2':humidity, 'field3':light_value, 'field5':soil_value} #nối các giá trị thành 1 chuỗi để gửi 
    print(sensor_reading) #in 2 giá trị gửi
    request = urequests.post( 'http://api.thingspeak.com/update?api_key=' + THINGSPEAK_WRITE_API_KEY,json = sensor_reading, headers = HTTP_HEADERS )  
    request.close()
    controlLED()
    lcd.move_to(0,0)
    lcd.putstr("Temp:")
    lcd.putstr(str(temperature))
    lcd.putstr("  ")
    lcd.putstr("Hum:")
    lcd.putstr(str(humidity))
    lcd.move_to(0,1)
    lcd.putstr("Li:")
    lcd.putstr(str(light_value))
    lcd.putstr(" ")
    lcd.putstr("Soil:")
    lcd.putstr(str(soil_value))
    lcd.putstr(" ")
    
    #đọc giá trị từ field5 (nút nhấn) và bật-tắt đèn tương ứng
    
    time.sleep(5)
'''from machine import Pin
import time
led_pin=13
led = Pin(led_pin, Pin.OUT)
while True:
    led.value(0)
    time.sleep_ms(500)
    led.value(1)
    time.sleep_ms(500)'''