#include <Arduino.h> // 標準ライブラリ
#include <SPI.h> // LCD表示ライブラリ
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h> // タッチパネルライブラリ
#include <Adafruit_Fingerprint.h> // 指紋認証モジュールライブラリ
#include "HX711.h" // ロードセルライブラリ
#include <Servo.h> // サーボライブラリ

// LCD表示
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); 
// タッチパネルのキャリブレーション値
#define TS_MINX 336
#define TS_MAXX 3942
#define TS_MINY 257
#define TS_MAXY 3862

// LCDタッチパネル
#define T_CS  7
#define T_IRQ  2 // 割り込み用
XPT2046_Touchscreen ts(T_CS, T_IRQ);

// 指紋認証モジュール
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(0, 1);
#else
#define mySerial Serial1
#endif
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int RegistrationFingerID = 2; // 事前に登録したID

// ロードセル
const int DOUT = 5;
const int SCK_P = 4;
HX711 scale;
// サーボ
#define pin_servo 6
Servo servo; // サーボライブラリ内の関数を使うための定義

volatile bool isTouched = false; // 割り込み判定
volatile bool detection = false; // 通常画面遷移判定
int touchX = 0; // X座標の判定変数
int touchY = 0; // Y座標の判定変数

// 初期設定
void setup() {

  Serial.begin(9600); // arduinoのポーレート
  finger.begin(57600); // 指紋認証のポーレート

  // LCD
  tft.begin(); // 表示の設定
  ts.begin(); // タッチパネルの設定
  ts.setRotation(1); // 液晶の向きに合わせる
  tft.fillScreen(ILI9341_BLACK); // 画面の初期化(ノイズ除去のため)

  // 液晶側のCSをHIGHにして通信を黙らせる
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH); 

  // ロードセル
  scale.begin(DOUT, SCK_P);
  scale.set_scale(1866.f); // 校正値の設定
  delay(3000); // 値が安定させるため待機
  scale.tare(); // 現在の重さを0gとする

  // サーボ
  servo.attach(pin_servo);
  servo.write(90); // サーボの角度を指定

  // 割り込み処理
  pinMode(T_IRQ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), touchISR, FALLING);

}

void loop() {

  // ボタンが押されていなければ通常表示をする
  if(!detection) {
    displayNormal();
  }

  // もしボタンがタッチされたら
  if(isTouched) {
    TS_Point p = ts.getPoint(); // 座標の変換、取得
    touchX = map(p.y, 257, 3862, 240, 0); 
    touchY = map(p.x, 336, 3942, 0, 320);
    if ((touchX >= 15 && touchX <= 320) && (touchY >= 0 && touchY <= 250)) {
      detection = true;
      displayTouch(); // 画面切り替え
    }
  }

}

// 指紋認証
uint8_t getFingerprintID() {
  uint8_t p = -1;
  while(p != FINGERPRINT_OK) { // 指が置かれるまでループ
    p = finger.getImage();
  }

  p = finger.image2Tz(); // データ変換
  if(p != FINGERPRINT_OK) return p;

  p = finger.fingerSearch(); // ID検索
  return p;
}

// 通常時
void displayNormal() {

  int weight = 0;

  // 重さ検知
  if(scale.is_ready()) {
    delay(1000); // 重さを安定させるため
    weight = scale.get_units(10);
  }
  // グラムの表示
  tft.setCursor(15,60); // テキストの位置
  tft.setTextSize(3); // テキストサイズ
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // テキストカラー
  tft.print("Weight: "); // LCDに表示するテキスト
  tft.print(weight);
  tft.println(" g  ");
  tft.println("Total money:");
  tft.print((weight / 7) * 500); // 総金額の計算(メモリ節約)
  tft.print(" yen  ");
  // ボタンの表示
  tft.drawRect(15, 170, 210, 80, ILI9341_WHITE); // 四角の描写
  tft.setCursor(30,200); // 四角の位置
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Withdrawal");
}

// 指紋認証中
void displayTouch() {
  tft.fillScreen(ILI9341_BLACK); // 画面をリセット
  tft.setCursor(1, 150);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("confirming...");

  getFingerprintID(); // 指紋の取得
  if(finger.fingerID == RegistrationFingerID) { // 指紋の照合
    displaysuccess();
  } else { // 認証失敗
    displayFailure();
  }

}

// 認証成功
void displaysuccess() {
  tft.fillScreen(ILI9341_BLACK); // 画面をリセット
  tft.setCursor(1, 50);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Open the Door");
  tft.drawRect(15, 120, 210, 80, ILI9341_WHITE);   // Closeボタンの描画
  tft.setCursor(80, 150);
  tft.print("Close");

  servo.write(0); // サーボの角度を0度に設定

  isTouched = false;
  while(true) {
    if(isTouched) {
      // 座標の再変換、取得
      TS_Point p = ts.getPoint();
      touchX = map(p.y, 257, 3862, 240, 0); 
      touchY = map(p.x, 336, 3942, 0, 320);
      if((touchX >= 15 && touchX <= 225) && (touchY >= 120 && touchY <= 200)) { //　closeが押されたら
        break;
      }
    }
  }

  servo.write(90); //サーボの角度を90度に戻す
  detection = false; // フラグを戻す
  isTouched = false;
  tft.fillScreen(ILI9341_BLACK); // 画面をリセット
}

// 認証失敗
void displayFailure() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(1, 150);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_RED);
  tft.println("AccessFailure");
  detection = false;
  isTouched = false;
  delay(1000); // 表示のため1秒待機
  tft.fillScreen(ILI9341_BLACK);
}

// 割り込み用関数
// ここではボタンがタッチされたかどうかの判定のみで処理はloopで行う
void touchISR() {
  isTouched = true;
}