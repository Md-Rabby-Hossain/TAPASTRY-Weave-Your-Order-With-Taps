# TAPASTRY: Weave Your Order With Taps

A smart, fun, and efficient ordering system using physical buttons and an OLED display. TAPASTRY lets users quickly place orders with simple taps — making menu navigation, quantity selection, and payment choosing effortless without a touchscreen.

---

## 📦 Features

- 📋 Menu navigation with physical buttons
- 🛒 Add items and select quantities
- 💳 Choose payment methods
- 📄 Order summary display
- 📤 Send order data to Google Sheets via Wi-Fi 

---

## 🎛️ Button Functions

- **Button 1 (Green):**
  - Reset order in quantity selection.
  - Open payment selection in menu.
  - Return to main menu elsewhere.

- **Button 2 (Blue):**
  - Open quantity selection in menu.
  - Add item to cart in quantity dialog.
  - Long press (1s) to place order.

- **Button 3 (Yellow):**
  - Move selection up in menu.
  - Increase quantity in quantity dialog.
  - Move payment option up in payment selection.

- **Button 4 (Red):**
  - Move selection down in menu.
  - Decrease quantity in quantity dialog.
  - Move payment option down in payment selection.

---

## 🚀 Tech Stack

- **ESP32** (Microcontroller)
- **OLED Display**
- **Push Buttons**
- **Google Apps Script (Web App URL for Google Sheets logging)**

---

## 📌 How It Works

Physical buttons control the entire ordering process:
- Select items from a menu.
- Choose quantities.
- Pick a payment method.
- Place orders, which are logged wirelessly into a Google Sheet.

---

## 📑 Project Name Meaning

**TAPASTRY** — because every tap weaves together an order(Pastry).

---

## 📃 License

Open-source for personal and educational use.

---

