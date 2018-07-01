<h2>2018永進専門大学 コンピューター情報系列 ウェブデータベース専攻 3年生 3チーム マイグミ 卒業プロジェクト</h2>
webSite : <http://52.78.122.145/>
<hr />
ハードウェアパーツ　：　Arduino, Bluetooth module, LED Strip, 木材、鉄材、オルゴール、釣り糸など

![Alt text](/readme/nagare.png)

このような流れです。

Bluetoothデータの規約

文字に対する音

| 音  |      Value     | ASCII  |
| --- |:---:| ----:|
| ド | A | 65 |
| ㇾ | C | 67 |
| ソル | H | 72 |
| ラ | J | 74 |
| シ | L | 76 |
| ド | M | 77 |
| ㇾ | O | 79 |
| ミ | Q | 81 |
| ファ | R | 82 |
| ファ# | S | 83 |
| ソル | T | 84 |
| ソル# | U | 85 |
| ラ | V | 86 |
| ラ# | W | 87 |
| シ | X | 88 |
| ド | Y | 89 |
| ド# | Z | 90 |
| ㇾ | [ | 91 |
| ㇾ# | \ | 92 |
| ミ | ] | 93 |
| ファ | ^ | 94 |
| ファ＃ | _ | 95 |
| ソル | ` | 96 |
| ソル＃ | a | 97 |
| ラ | b | 98 |
| ラ＃ | c | 99 |
| シ | d | 100 |
| ド | e | 101 |
| ㇾ | g | 103 |
| ミ | i | 105 |

motorバージョン（１）と既存のプロトタイプバージョン（２）があります。

（1）

サーボモーターとLEDを使ってカスタムオルゴールを作りました。
![Alt text](/readme/make2.jpg)
![Alt text](/readme/make1.jpg)
![Alt text](/readme/motor2.jpg)
![Alt text](/readme/motor1.jpg)

PWMservoDriverを使ってArduino Digital Pin2つで全部操作することができます。

Function の説明

| function        |      Description      | returnValue  |
| ------------- |:-------------:| -----:|
| setup()      | モーター、LED、Bluetoothを初期化します | NULL |
| loop()      | 文字列による演奏ができます。     |   NULL |
| pulseToAngle(int) | Pulseをサーボモーターの角度に変換してreturnします。     |    int |
| moveServoMotor(int)      | パラメータに合わせるオルゴールの音を演奏します。      |   NULL |
| BTRate() | モバイルからのBluetoothを文字列をゲットします      |    NULL |
| ledSetColor(int, int, int, int, int)      | LEDの色を交わします。     |   NULL |

（2）

ソレノイドとRELAYModule,Bluetoothを使ってオルゴールを作りました。

![Alt text](/readme/12.jpg)

Function の説明
| function        |      Description      | returnValue  |
| ------------- |:-------------:| -----:|
| setup()      | RELAY、Bluetoothを初期化します | NULL |
| loop()      | 文字列による演奏ができます。     |   NULL |
| moveSolenoidByChar(int) | パラメータに合わせるRELAYがONされオルゴール音を演奏します。 |　NULL |
| stopSolenoidByChar(int) | パラメータに合わせるRELAYがOFFされます。| NULL |
| allSolenoidStop() | すべてのRELAYがOFFされます | 103 |
| BTRate() | モバイルからのBluetoothを文字列をゲットします | 105 |

動画：<https://www.youtube.com/watch?v=F9jokGUb19k#t=9m05s>

