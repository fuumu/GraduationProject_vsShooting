#include "types.h"
#include "sceneManager.h" // シーンマネージャー
#include "scene_title.h"  // タイトル画面 最初に呼び出すシーン
#include "EEPROM.h"
#include "FspTimer.h" // 割り込み処理で必要なライブラリ

//==============================================================================
// グローバル変数
//==============================================================================

// シーンマネージャーのインスタンス
SceneManager *sceneManager = nullptr;

// タイマー割り込み関連の変数
FspTimer _timer;
volatile bool shouldDraw = false;
volatile bool gameState = GAME_RUNNING;

// OLEDのインスタンス
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// ブザーのインスタンス
SoundManager sound;

//==============================================================================
// セットアップ関数
//==============================================================================
void setup()
{
  // シリアル通信の初期化
  Serial.begin(115200);
  while (!Serial)
  {
    ; // シリアルポートの接続を待機（ネイティブUSBポートのみ必要）
  }

  // 入力ピンの設定
  pinMode(STICK_X, INPUT);
  pinMode(STICK_Y, INPUT);
  pinMode(BUTTON_A, INPUT);
  pinMode(12, INPUT_PULLUP); // ハイスコアクリア用ピン

  // 起動時に12番ピンがLOWならハイスコアを消去
  if (digitalRead(12) == LOW)
  {
    EEPROM.write(EEPROM_ADDR_HISCORE, 0);
  }

  // OLEDディスプレイの初期化
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); // ディスプレイをクリア

  // ブザーの初期化
  sound.begin(BUZZER_PIN);

  // ゲームの初期化
  sceneManager = new SceneManager;
  sceneManager->currentScene = new Title(sceneManager);

  // タイマー割り込みの設定
  uint8_t type;
  int8_t ch = FspTimer::get_available_timer(type);
  if (ch < 0)
  {
    return;
  }
  _timer.begin(TIMER_MODE_PERIODIC, type, ch, 60.0f, 50.0f, callbackfunc, nullptr);
  _timer.setup_overflow_irq(15, nullptr);
  _timer.open();
  _timer.start();
}

//==============================================================================
// メインループ関数
//==============================================================================
void loop()
{
  // 描画フラグが立っていたらゲームの描画を行う
  if (shouldDraw)
  {
    display.clearDisplay(); // ディスプレイをクリア
    display.setTextColor(WHITE);
    sceneManager->drawScene(); // シーンの描画
    display.display();         // 画面を更新
    shouldDraw = false;
  }
  //サウンド更新処理
  sound.update();

  // 通信処理（通信しないシーンなら何も起きない）
  sceneManager->communicateScene();
}

//==============================================================================
// タイマー割り込みコールバック関数
//==============================================================================
void callbackfunc(timer_callback_args_t *arg)
{
  if (gameState == GAME_RUNNING)
  {
    gameState = sceneManager->updateScene(); // ゲームシーンを更新
  }
  shouldDraw = true;
}