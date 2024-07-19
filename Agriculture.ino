#include <DHT11.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
DHT11 dht11(10);

int temperature = 0;
int humidity = 0;
unsigned long intervalLCD = 0;
LiquidCrystal_I2C lcd1(0x27, 16, 2); // Địa chỉ của màn hình LCD đầu tiên
LiquidCrystal_I2C lcd2(0x3F, 16, 2); // Địa chỉ của màn hình LCD thứ hai
LiquidCrystal_I2C lcd3(0x20, 16, 2); // Địa chỉ của màn hình LCD thứ ba
// Định nghĩa các chân GPIO cho vi điều khiển
#define LED_PIN 11  // Chân điều khiển LED
#define LDR_PIN A2  // Chân kết nối cảm biến ánh sáng
#define Soil A3
#define PUMP_PIN 12   // Chân điều khiển bơm nước

#define BLYNK_TEMPLATE_ID "TMPL6iyBlvWFf"
#define BLYNK_TEMPLATE_NAME "sdadSD"
#define BLYNK_AUTH_TOKEN "FW3QgBiPsQLr5Hze8Z4UNF55ImkOhKQj"


// Bao gồm các thư viện cần thiết
#include "BlynkGate.h"  // Thư viện BlynkGate cho kết nối Blynk


// Thiết lập WiFi
char auth[] = BLYNK_AUTH_TOKEN;  // Mã xác thực Blynk
char ssid[] = "MakerLab.vn";     // Tên mạng Wi-Fi (chỉ hoạt động với băng tần 2.4Ghz)
char pass[] = "";                // Mật khẩu Wi-Fi


// Định nghĩa các chân Virtual Pins như trên Web BLYNK
const int Virtual_Pins_LED = 1;
const int Virtual_Pins_LDR = 2;
const int Virtual_Pins_temp = 3;
const int Virtual_Pins_hum = 4;
const int Virtual_Pins_Soil=5;


unsigned long lastTimeSend = 0;  // Biến thời gian gửi dữ liệu cuối cùng
int ledState = 0;                // Biến trạng thái của LED (bật/tắt)


void setup() {
  pinMode(LED_PIN, OUTPUT);               // Thiết lập chân LED là OUTPUT
  pinMode(LDR_PIN, INPUT);                // Thiết lập chân cảm biến ánh sáng là INPUT
  pinMode(Soil, INPUT);
  Serial.begin(115200);                   // Khởi tạo giao tiếp Serial để debug
  Serial.println("Start BlynkGate I2C");  // In ra thông báo khởi tạo BlynkGate I2C
  lcd1.init();
  lcd1.backlight();
  lcd1.print("Hello LCD 1");
  lcd2.init();
  lcd2.backlight();
  lcd2.print("Hello LCD 2");
  lcd3.init();
  lcd3.backlight();
  lcd3.print("Hello LCD 3");
  Blynk.begin(auth, ssid, pass);  // Kết nối với Blynk bằng thông tin xác thực và Wi-Fi
}



void loop() {
  Blynk.run();       // Xử lý các yêu cầu từ Blynk, không sử dụng delay trong loop này
  updateLedState();  // Cập nhật trạng thái của LED
  controlWaterPump(); // Kiểm tra và điều khiển bơm nước khi cần
  adjustLedBrightness(); // Điều chỉnh độ sáng đèn theo cường độ ánh sáng

  int vsoil=analogRead(Soil);
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (millis() - lastTimeSend >= 1000) {                                             // Kiểm tra nếu đã qua 100ms kể từ lần gửi cuối cùng
    Blynk.virtualWrite(Virtual_Pins_LDR, map(analogRead(LDR_PIN), 0, 400, 100, 0));  // Gửi giá trị của cảm biến ánh sáng đến Blynk
    Blynk.virtualWrite(Virtual_Pins_temp, temperature);
    Blynk.virtualWrite(Virtual_Pins_hum, humidity);
    Blynk.virtualWrite(Virtual_Pins_Soil, vsoil);
    lastTimeSend = millis();  // Cập nhật thời gian gửi dữ liệu cuối cùng
  }
  lcd1.setCursor(0, 0);
  lcd1.print("Temp:");
  lcd1.print(temperature);
  lcd1.setCursor(0,1);
  lcd1.print("Humidity:");
  lcd1.print(humidity);
  
  lcd2.setCursor(0, 0);
  lcd2.print("Light:");
  lcd2.print(map(analogRead(LDR_PIN), 0, 400, 100, 0));

  lcd3.setCursor(0, 0);
  lcd3.print("Soil:");
  lcd3.print(vsoil);

  //###############################################################################################
}


// Nhận giá trị từ Virtual Pin
BLYNK_WRITE_DEFAULT() 
{
  int myInt = param.asInt();  // Nhận giá trị từ Blynk và chuyển đổi thành kiểu int

  Serial.print("input V");    // In ra pin đầu vào
  Serial.print(request.pin);  // In ra số pin
  Serial.print(": ");
  Serial.println(myInt);  // In ra giá trị nhận được

  if (request.pin == Virtual_Pins_LED) {  // Kiểm tra nếu là Virtual_Pins_LED
    if (myInt == 1) {                     // Nếu giá trị nhận được là 1
      ledState = 1;                       // Bật LED
    } else {                              // Nếu giá trị nhận được không phải là 1
      ledState = 0;                       // Tắt LED
    }
  }
}


void updateLedState() 
{
  digitalWrite(LED_PIN, ledState);  // Cập nhật trạng thái của LED (bật/tắt)
}

void controlWaterPump() {
  int soilMoisture = analogRead(Soil);
  if (soilMoisture < 512) { // Giả sử giá trị 512 tương đương với 50% độ ẩm
    digitalWrite(PUMP_PIN, HIGH); // Bật bơm nước
    while (analogRead(Soil) < 614) { // Chờ đến khi độ ẩm đạt 60%
      delay(1000);
    }
    digitalWrite(PUMP_PIN, LOW); // Tắt bơm nước
  }
}

void adjustLedBrightness() {
  int lightIntensity = analogRead(LDR_PIN);
  int ledBrightness = map(lightIntensity, 0, 1023, 255, 0); // Giả sử giá trị ánh sáng thấp nhất là 0 và cao nhất là 1023
  analogWrite(LED_PIN, ledBrightness);
}