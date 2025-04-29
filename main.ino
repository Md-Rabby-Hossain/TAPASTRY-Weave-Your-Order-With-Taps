#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
const int button1 = 12;
const int button2 = 14;
const int button3 = 27;
const int button4 = 26;

// Wi-Fi credentials
const char* ssid = "Wokwi-GUEST"; // need to change if you are using real esp32 
const char* password = "";

// Google Apps Script Web App URL
const char* serverName = "https://script.google.com/macros/s/AKfycby6T_YuRErwIDHleRHBt9mqP0LmGeUPIQMmg01-mtt26R_NlHThF8cQJHoHCgpvtE0/exec";

// Menu and order details
String menu[] = {"Mutton", "Beef", "Chicken", "Rice", "Aluu Vorta", "Daal"};
float prices[] = {200.0, 180.0, 150.0, 50.0, 20.0, 30.0};
int numItems = 6;
int cartQuantity[6] = {0, 0, 0, 0, 0, 0};
int selectedItem = 0;
bool inMenu = false;
bool inQuantityDialog = false;
bool inPaymentMethodSelection = false;
unsigned long buttonPressStart = 0;
const unsigned long longPressDuration = 1000;

// Payment options
String paymentMethods[] = {"Cash", "Card", "Bkash", "Nagad"};
int selectedPaymentMethod = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  display.println("WiFi Connected");
  display.display();
  delay(1000);

  displayMainMenu();
}

void loop() {
  if (digitalRead(button1) == LOW) {
    handleButton1();
    delay(500);
  }
  if (digitalRead(button2) == LOW) {
    handleButton2();
    delay(500);
  }
  if (digitalRead(button3) == LOW) {
    handleButton3();
    delay(500);
  }
  if (digitalRead(button4) == LOW) {
    handleButton4();
    delay(500);
  }

  if (digitalRead(button2) == LOW) {
    if (millis() - buttonPressStart > longPressDuration) {
      placeOrder();
    }
  } else {
    buttonPressStart = millis();
  }
}

void handleButton1() {
  if (inQuantityDialog) {
    resetOrder();
  } else if (inMenu) {
    displayPaymentMethods();
  } else {
    displayMainMenu();
  }
}

void handleButton2() {
  if (inMenu) {
    displayQuantityDialog();
  } else if (inQuantityDialog) {
    addToCart();
  }
}

void handleButton3() {
  if (inMenu) {
    selectedItem = (selectedItem - 1 + numItems) % numItems;
    displayMainMenu();
  } else if (inQuantityDialog) {
    cartQuantity[selectedItem]++;
    displayQuantityDialog();
  } else if (inPaymentMethodSelection) {
    selectedPaymentMethod = (selectedPaymentMethod - 1 + 4) % 4;
    displayPaymentMethods();
  }
}

void handleButton4() {
  if (inMenu) {
    selectedItem = (selectedItem + 1) % numItems;
    displayMainMenu();
  } else if (inQuantityDialog) {
    if (cartQuantity[selectedItem] > 0) {
      cartQuantity[selectedItem]--;
    }
    displayQuantityDialog();
  } else if (inPaymentMethodSelection) {
    selectedPaymentMethod = (selectedPaymentMethod + 1) % 4;
    displayPaymentMethods();
  }
}

void displayMainMenu() {
  inMenu = true;
  inQuantityDialog = false;
  inPaymentMethodSelection = false;
  display.clearDisplay();
  display.setCursor(0, 0);
  
  // Show current payment method
  display.print("Pay: ");
  display.println(paymentMethods[selectedPaymentMethod]);
  display.println("-----------");

  for (int i = 0; i < numItems; i++) {
    if (i == selectedItem) {
      display.setTextSize(2);
      display.setTextColor(SSD1306_INVERSE);
      display.println(menu[i]);
      display.setTextColor(WHITE);
      display.setTextSize(1);
    } else {
      display.print(menu[i]);
      display.print(" (");
      display.print(prices[i]);
      display.println(")");
    }
  }
  display.display();
}

void displayQuantityDialog() {
  inMenu = false;
  inQuantityDialog = true;
  inPaymentMethodSelection = false;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Select Quantity:");
  display.print(menu[selectedItem]);
  display.print(" (");
  display.print(prices[selectedItem]);
  display.println(")");
  display.print("Qty: ");
  display.println(cartQuantity[selectedItem]);
  display.display();
}

void addToCart() {
  displayMainMenu();
}

void displayPaymentMethods() {
  inMenu = false;
  inQuantityDialog = false;
  inPaymentMethodSelection = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Payment Method:");
  display.println("---------------");
  for (int i = 0; i < 4; i++) {
    if (i == selectedPaymentMethod) {
      display.setTextSize(2);
      display.setTextColor(SSD1306_INVERSE);
      display.println(paymentMethods[i]);
      display.setTextColor(WHITE);
      display.setTextSize(1);
    } else {
      display.println(paymentMethods[i]);
    }
  }
  display.display();
}

void placeOrder() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Order Placed!");
  display.display();

  String orderData = "Table: 1\n";
  float totalPrice = 0.0;
  for (int i = 0; i < numItems; i++) {
    if (cartQuantity[i] > 0) {
      float itemTotal = cartQuantity[i] * prices[i];
      totalPrice += itemTotal;
      orderData += menu[i] + " x" + String(cartQuantity[i]) + " = " + String(itemTotal) + "\n";
    }
  }
  orderData += "Payment: " + paymentMethods[selectedPaymentMethod] + "\n";
  orderData += "Total: " + String(totalPrice);

  Serial.println("Order Data:");
  Serial.println(orderData);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverName) + "?order=" + urlencode(orderData);
    http.begin(url);
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }

  delay(2000);
  resetOrder();
}

void resetOrder() {
  for (int i = 0; i < numItems; i++) {
    cartQuantity[i] = 0;
  }
  selectedPaymentMethod = 0;
  displayMainMenu();
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}
