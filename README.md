# 2025年卒業制作3期

## 概要

Arduino UNO R4 Wifiを用いた、重さの検知と指紋認証でロック解除できる500円玉貯金箱

![alt text](picture/entirety.jpg)

## 主な機能

ロードセルを使用した、貨幣の重さを検知  
LCDタッチパネルへの重量と金額の表示  
指紋認証モジュールとサーボモーターでの施錠管理

## 仕様書

### 使用モジュール

|部品|個数|用途|接続ピン|
|-|-|-|-|
|Arduino UNO R4 Wifi|1個|制御|USBケーブル|
|Arduino UNO用シールド基盤|1個|制御  ブレッドボードの代用  規模縮小のため|Arduino UNO R4 Wifi|
|ロードセル|1個|重さセンサー|ch2<br>1.AVDD：1<br>2.GND：2<br>ch3<br>1.INNA：1<br>2.INPA：2<br>|
|ロードセル用ADコンバーター|1個|ロードセルの抵抗値を測定し変換する|VCC：5V<br>GND：GND<br>DAT：4<br>CLK：5|
|ジャンパーワイヤー|沢山|配線|各ピン|
|LCDタッチパネル※1|1個|表示|VCC/LED：3.3V<br>GND：GND<br>CS：10<br>RESET：8<br>DC：9<br>SDI(mosi)/T_DIN(mosi)：11<br>SCK/T_CLK：13<br>T_CS：7<br>T_DO(miso)：12<br>T_IRQ：2|
|レベルコンバータ|3個|arduinoとLCD、指紋認証モジュールを接続するため|各モジュールピン|
|指紋認証モジュール※1|1個|指紋認証|VCC：3.3V<br>GND：GND<br>TX：0<br>RX：1|
|サーボ|1個|扉の開閉制御|VCC：5V<br>GND：GND<br>SERVO：6|

※1：Arduino UNO R4 Wifiは5V、LCDタッチパネルと指紋認証モジュールは3.3Vで動作するため、レベルコンバーターを介し電圧を下げてからピンに接続する必要がある

### ブレッドボード図

![alt text](picture/breadboard.png)

### 回路図

![alt text](picture/schematic.png)

### 簡単なフローチャート

``` mermaid
graph TD;
    A[モニターに金額、重量、取り出しボタンを表示] --> B[取り出しボタンを押す]
    B --指紋認証を促す--> C[指紋認証をする]
    C --成功--> D[扉が開く]
    D --解放中と表示--> E[お金を取り出す]
    E --扉を閉じる--> A
    C --失敗--> I[認証に失敗しましたと表示]
    I --通常時に戻る--> A
```

### 画面遷移

成功時の流れ
[](movie/mov1.mp4)

失敗時の流れ
[](movie/mov2.mp4)

通常時
![alt text](picture/normal.jpg)

指紋認証待機
![alt text](picture/confirming.jpg)

認証成功
![alt text](picture/success.jpg)

認証失敗
![alt text](picture/Failure.jpg)

### ソフトウェア

#### 標準ライブラリ

Arduino.h  
Servo.h

#### 追加ライブラリ

指紋認証モジュール  
Adafruit_Fingerprint.h  

画面表示  
SPI.h  
Adafruit_GFX.h  
Adafruit_ILI9341.h  

タッチパネル  
SPI.h  
XPT2046_Touchscreen.h

### 参考サイト

- ロードセル
  - [webサイト](https://deviceplus.jp/arduino/arduino-dezitaru-keisoku-2/)
  - [解説動画](https://youtu.be/1BfKZ6SDnD0?si=p0DVoaR5B4i8J-f0)
  - [キャリブレーションの解説](https://youtu.be/sxzoAGf1kOo?si=oG0q9NBSkf5GrbU3)

- 貨幣の重さ
  - [財務省](https://www.mof.go.jp/policy/currency/coin/general_coin/list.htm)

- LCDタッチパネル
  - [動画](https://www.youtube.com/watch?v=UAqyy7OqpZY)
  - [webサイト](http://jh7ubc.web.fc2.com/Arduino_R4/Arduino_UNO_R4_TFT.html)

- 指紋認証モジュール
  - [解説動画](https://www.youtube.com/shorts/sTgih5Lm5V8)
  - [webサイト](https://www.adafruit.com/product/4750)

- サーボ
  - [Arduino基本プロジェクト](https://docs.sunfounder.com/projects/elite-explorer-kit/ja/latest/basic_projects/27_basic_servo.html)
