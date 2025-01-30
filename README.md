# ESPxStorEase
 A little Arduino scripts with a simple Web File Manager for your ESP8266/ESP32 LittleFS files

![ESP Boards](https://img.shields.io/badge/Supports-ESP32%20%7C%20ESP8266-green) 
[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fpeff74%2FESPxStorEase&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)

Control files on your ESP board through a web browser! 📂➡️🖥️  
Upload, delete, and view files directly from your phone or computer.

## What's Inside? 🧰
- Works with **ESP32** and **ESP8266**
- No apps needed - works in any browser
- See file sizes and dates
- Upload multiple files at once
- Delete files with 1 click
- Simple "Format" button to erase everything

## What You Need 🔌
- ESP32 **or** ESP8266 board
- USB cable
- WiFi network
- Computer with Arduino IDE

## Easy Setup Guide 🛠️

### 1. Install Tools (First Time Only)
1. [Download Arduino IDE](https://www.arduino.cc/en/software)
2. Add board support:
   - **ESP32**: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - **ESP8266**: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   
   *(File > Preferences > Additional Boards URLs)*

### 2. Copy the Code 📝
  - Open the *.ino file for your board in the Arduino IDE

### 3. Configuration 🔧
   ```cpp
   // WiFi Credentials
   #define WIFI_SSID "Your_WiFi_SSID"
   #define WIFI_PASSWORD "Your_WiFi_Password"
   
   // NTP Configuration (GMT+1 with DST)
   configTime(3600, 3600, "pool.ntp.org");
```


### 4. Usage
1. Connect to serial monitor (115200 baud)  
2. Wait for IP address confirmation  
3. Access in browser: `http://[ESP-IP]`

   
### 5. Use the following routes
- http://[ESP-IP]/         → Redirects to /main (if index.html exists)
- http://[ESP-IP]/fs       → File Manager UI
- http://[ESP-IP]/reset    → Restart the ESP

### 6. Example Output
<details>
<summary>📁 Click to show code</summary>
  
```ccp
ets Jul 29 2019 12:21:46

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:4916
load:0x40078000,len:16436
load:0x40080400,len:4
ho 8 tail 4 room 4
load:0x40080404,len:3524
entry 0x400805b8
..
Connected! IP-Adresse: 192.168.206.238
LittleFS initialized.

Server is now listening ...

Upload started: /infos.txt
File opened for writing
Bytes written to file: 1436
Bytes written to file: 1436
Bytes written to file: 1436
Bytes written to file: 1436
Bytes written to file: 1436
Bytes written to file: 1324
Upload finished. File closed
Upload started: /Test.txt
File opened for writing
Bytes written to file: 75
Upload finished. File closed
View: Test.txt
.......
Connected! IP-Adresse: 192.168.206.105
LittleFS initialized.

Server is now listening ...

Upload started: /infos.txt
File opened for writing
Bytes written to file: 2048
Bytes written to file: 2048
Bytes written to file: 2048
Bytes written to file: 2048
Bytes written to file: 312
Upload finished. File closed
Upload started: /Jumbli.txt
File opened for writing
Bytes written to file: 75
Upload finished. File closed
View: Test.txt
```
</details>



## Need Help? 🤔

Ask in the [GitHub Discussions](https://github.com/peff74/ESPxStorEase/discussions)  
I'm happy to help beginners! 😊


## 📜 License 

[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This project is open-source and available under the **MIT License** - see the [LICENSE](LICENSE) file for full details.

**You are free to:**
- Use commercially
- Modify freely
- Distribute copies
- Place private use

**Only requirement:** Include original license in copies

---

**Made with ❤️ for IoT Beginners**  
*Works with Arduino IDE 3.x - Last tested: Jan 2025*
