#include <TFT_eSPI.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include "Constants.h"

// ------------------- НАСТРОЙКИ -------------------
#define BUTTON_PIN 40

TFT_eSPI tft = TFT_eSPI();
HardwareSerial mySerial(2);
DFRobotDFPlayerMini player;

// ------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ -------------------

// Рисует графические блоки разной плотности: 0=█, 1=▒, 2=░
void drawBlock(int x, int y, int w, int h, int type, uint16_t color) {
  if (type == 0) {
    tft.fillRect(x, y, w, h, color); // Сплошной блок █
  } 
  else if (type == 1) {
    // Сетка ▒ (через пиксель)
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        if ((i + j) % 2 == 0) tft.drawPixel(x + j, y + i, color);
      }
    }
  } 
  else if (type == 2) {
    // Редкие точки ░
    for (int i = 0; i < h; i += 2) {
      for (int j = 0; j < w; j += 2) {
        tft.drawPixel(x + j, y + i, color);
      }
    }
  }
}

// Анимация "взлома" текста
void revealTextScramble(String finalText) {
  int len = finalText.length();
  int charWidth = 20;  // Ширина под один символ
  int charHeight = 24; // Высота под один символ
  int startX = 30;     // Отступ слева
  int startY = 100;    // Центрирование по вертикали (настрой под свой Bar LCD)

  for (int progress = 0; progress <= len; progress++) {
    for (int i = 0; i < len; i++) {
      int x = startX + (i * charWidth);
      
      // Стираем старое состояние символа
      tft.fillRect(x, startY, charWidth, charHeight, TFT_BLACK);

      if (i < progress) {
        // Рисуем финальный символ
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(x, startY);
        tft.print(finalText[i]);
      } else {
        // Рисуем шум, если это не пробел
        if (finalText[i] != ' ') {
          int r = random(0, 10);
          if (r < 4) {
            // Рисуем один из графических блоков (█ ▒ ░)
            drawBlock(x, startY + 2, 14, 18, random(0, 3), TFT_GREEN);
          } else if (r < 8) {
            // Рисуем случайный символ из набора в Constants.h
            tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(x, startY);
            tft.print(scrambleChars[random(0, strlen(scrambleChars))]);
          }
        }
      }
    }
    delay(70); // Скорость анимации
  }
}

// ------------------- SETUP -------------------
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Экран
  tft.init();
  tft.setRotation(1); // 1 или 3 для альбомной ориентации Bar LCD
  tft.fillScreen(TFT_BLACK);
  
  // Звук (Пины 16 RX, 17 TX для ESP32)
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!player.begin(mySerial)) {
    Serial.println("DFPlayer Error - check SD card/wiring");
  }
  player.volume(25);

  randomSeed(analogRead(0));

  tft.setTextColor(TFT_GREEN);
  tft.drawString("SYSTEM READY_V1.0", 10, 10, 2);
}

// ------------------- LOOP -------------------
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Небольшая задержка перед началом
    delay(200);
    
    // 1. Очистка зоны текста
    tft.fillScreen(TFT_BLACK);
    
    // 2. Звуковой сигнал
    player.play(1); 
    
    // 3. Выбор фразы

    
    TJpgDec.drawSdJpg(40, 20, "/logo.jpg"); 
    String selected = prescripts[random(0, totalTexts)];
    
    // 4. Запуск анимации
    revealTextScramble(selected);
    
    // 5. Защита от дребезга кнопки
    delay(2000);
  }
}
