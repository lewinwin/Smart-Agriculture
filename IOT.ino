#define LED_PIN 13     // chân D9
#define LDR_PIN 32     // kết nối cảm biến với chân 32 (A3 trên shield)
#define Soil 34        // kết nối cảm biến với chân 34 (A1 trên shield)
#define PUMP_PIN 23    // chân D11

int temperature = 0;
int humidity = 0;

// Copy thông tin này trên Blynk
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

// Bao gồm các thư viện cần thiết
#include <WiFi.h>                // Thư viện WiFi cho ESP32
#include "BlynkGate.h"           // Thư viện BlynkGate cho kết nối Blynk
#include "MKL_LiquidCrystal_I2C.h" // Thư viện LCD I2C
#include <DHT11.h>               // Thư viện cho cảm biến DHT11

DHT11 dht11(33);                 // Kết nối cảm biến DHT với chân 33 (A2 trên Shield)
MKL_LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ của màn hình LCD đầu tiên

// Thiết lập WiFi
char auth[] = BLYNK_AUTH_TOKEN;    // Mã xác thực Blynk
char ssid[] = "";                  // Tên mạng Wi-Fi (chỉ hoạt động với băng tần 2.4Ghz)
char pass[] = "";                  // Mật khẩu Wi-Fi

// Định nghĩa các chân Virtual Pins như trên Web BLYNK
const int Virtual_Pins_Pump = 1;
const int Virtual_Pins_LDR = 2;
const int Virtual_Pins_temp = 3;
const int Virtual_Pins_hum = 4;
const int Virtual_Pins_Soil=5;

unsigned long lastTimeSend = 0;  // Biến thời gian gửi dữ liệu cuối cùng
int PumpState = 0;               // Biến trạng thái của bơm

void setup() {
  pinMode(LED_PIN, OUTPUT);          // Thiết lập chân LED là OUTPUT
  pinMode(LDR_PIN, INPUT);  
  pinMode(Soil, INPUT);              // Thiết lập chân cảm biến độ ẩm đất là INPUT
  Serial.begin(115200);              // Khởi tạo giao tiếp Serial để debug
  Serial.println("Start BlynkGate I2C");
  lcd.init();
  lcd.backlight(); 
  lcd.print("IOT GARDEN");
  Blynk.begin(auth, ssid, pass);     // Kết nối với Blynk bằng thông tin xác thực và Wi-Fi
}

void loop() {
  Blynk.run();                       // Xử lý các yêu cầu từ Blynk, không sử dụng delay trong loop này
  updatePumpState();                 // Cập nhật trạng thái của bơm
  controlLED();

  // Gửi giá trị quang trở
  int vsoil= map(analogRead(Soil), 0, 4095, 0, 100); 
  int result = dht11.read(temperature, humidity);

  if (millis() - lastTimeSend >= 1000) {                               // Kiểm tra nếu đã qua 100ms kể từ lần gửi cuối cùng
     Blynk.virtualWrite(Virtual_Pins_LDR, map(analogRead(LDR_PIN), 0, 4095, 100, 0));  // Gửi giá trị của cảm biến ánh sáng đến Blynk
     Blynk.virtualWrite(Virtual_Pins_temp, temperature);
     Blynk.virtualWrite(Virtual_Pins_hum, humidity);
     Blynk.virtualWrite(Virtual_Pins_Soil, vsoil);
     lastTimeSend = millis();                                          // Cập nhật thời gian gửi dữ liệu cuối cùng
  }

  lcd.setCursor(0, 0);
  lcd.print("Li:");
  lcd.print(map(analogRead(LDR_PIN), 0, 4095, 100, 0));
  lcd.print(" ");
  lcd.print("Tem:");
  lcd.print(temperature);
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print("hum:");
  lcd.print(humidity);
  lcd.print(" ");
  lcd.print("soil:");
  lcd.print(vsoil);
  lcd.print("  "); 
}

// Nhận giá trị từ Virtual Pin
BLYNK_WRITE_DEFAULT() {
  int myInt = param.asInt();  // Nhận giá trị từ Blynk và chuyển đổi thành kiểu int

  Serial.print("input V");    // In ra pin đầu vào
  Serial.print(request.pin);  // In ra số pin
  Serial.print(": ");
  Serial.println(myInt);      // In ra giá trị nhận được

  if (request.pin == Virtual_Pins_Pump) {  // Kiểm tra nếu là Virtual_Pins_Pump
    if (myInt == 1) {                       // Nếu giá trị nhận được là 1
      PumpState = 1;                        // Bật bơm
    } else {                                // Nếu giá trị nhận được không phải là 1
      PumpState = 0;                        // Tắt bơm
    }
  }
}

void updatePumpState() {
  digitalWrite(PUMP_PIN, PumpState);  // Cập nhật trạng thái của bơm (bật/tắt)
}

void controlLED() {
  int lightLevel = map(analogRead(LDR_PIN), 0, 4095, 100, 0);
  if (lightLevel < 50) {               // Ngưỡng ánh sáng, giá trị có thể thay đổi tùy điều kiện
    digitalWrite(LED_PIN, HIGH);       // Bật đèn khi trời tối
  } else {
    digitalWrite(LED_PIN, LOW);        // Tắt đèn khi trời sáng
  }
}