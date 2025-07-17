#pragma once
#include <Arduino.h>
#include "pitches.h"

enum SoundType {
  SOUND_BEEP,//ビープ音
  SOUND_ERROR,//エラー音
  SOUND_COUNTDOWN_TICK,//カウントダウン
  SOUND_COUNTDOWN_END,//カウントダウン終了
  SOUND_WAIT,//待機中のBGM
  SOUND_SHOW,//メニュー表示
  SOUND_STARTUP,//スタート音
  SOUND_LOSS,//敗北BGM
  SOUND_WIN,//勝利BGM
  SOUND_POWER_UP,//パワーアップ音
  SOUND_DAMAGE,//ダメージ音
  SOUND_HIT,//相手に弾が当たった音
  SOUND_SHOT,//弾を撃つ
  SOUND_CURSOR_MOVE,//カーソル移動音
  SOUND_NONE,
};

class SoundManager {
public:
  SoundManager();

  //初期化
  void begin(int pin);

  //サウンドを再生
  void playSound(SoundType type, bool loop = false);

  //サウンドを停止
  void stopSound();

  // メロディ再生中に呼び出す必要あり
  void update();

private:
  int buzzerPin;
  int* currentMelody;
  int* currentDurations;
  int melodyLength;
  int currentNote;
  unsigned long noteStartTime;
  unsigned long noteDuration;

  // ループ再生フラグ
  bool loopPlayback;

  //優先順位比較用変数
  SoundType currentSoundType = SOUND_NONE;

  //再生するサウンドを設定
  void setMelody(SoundType type);
};