# 2025年卒業制作

## 概要

Arduino UNO R4 Wifiを用いた、重さの検知と指紋認証でロック解除できる貯金箱

### 主な機能

ロードセルを使用した、貨幣の重さを検知  
LCDタッチパネルへの重量と金額の表示  
指紋認証モジュールとサーボモーターでの施錠管理

# 仕様

## 使用パーツ

|パーツ|個数|用途|
|-|-|-|
|Arduino UNO R4 Wifi|1個|制御|
|Arduino UNO用シールド基盤|1個|ブレッドボードの代用  規模縮小のため|
|ロードセル|1個|重さセンサー|
|ロードセル用ADコンバーター|1個|ロードセルの抵抗値を測定し変換する  精度向上のため|
|ジャンパーワイヤー|沢山|配線|
|LEDタッチパネル|1個|表示|
|レベルコンバータ|1個|arduinoとLEDを接続するため|
|指紋認証モジュール|1個|指紋認証|
|サーボ|1個|扉の開閉制御|

## 簡単なフローチャート

全体の流れ

``` mermaid
graph TD;
    A[モニターに金額、重量、取り出しボタンを表示] --> B[取り出しボタンを押す]
    B --指紋認証を促す--> C[指紋認証をする]
    C --成功--> D[扉が開く]
    D --解放中と表示--> E[お金を取り出す]
    E --扉を閉じる--> F[施錠完了と表示]
    F --> G[重量と金額の差を計算し結果を表示]
    G --通常時に戻る--> A
    C --失敗--> I[認証に失敗しましたと表示]
    I --通常時に戻る--> A
```

## ブレッドボード図

## 回路図
<img width="2823" height="1335" alt="sotugyouseisaku_" src="https://github.com/user-attachments/assets/d2b9adc6-d5d4-4f7c-a100-40833769b5bd" />

![IMG_5851](https://github.com/user-attachments/assets/26883ad9-94da-4552-a5e0-63aa94eec9c7)


## ソフトウェア

### 標準ライブラリ

Arduino.h  
Servo.h

### 追加ライブラリ

指紋認証モジュール  
Adafruit_Fingerprint.h

画面表示  
SPI.h  
Adafruit_GFX.h  
Adafruit_ILI9341.h

タッチパネル  
SPI.h  
XPT2046_Touchscreen.h  

ロードセル  
HX711.h

## 参考サイト

- ロードセル
  - [webサイト](https://deviceplus.jp/arduino/arduino-dezitaru-keisoku-2/)
  - [解説動画](https://youtu.be/1BfKZ6SDnD0?si=p0DVoaR5B4i8J-f0)
  - [キャリブレーションの解説](htts://youtu.be/sxzoAGf1kOo?si=oG0q9NBSkf5GrbU3)

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
