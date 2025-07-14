#pragma once
#include <Arduino.h>
#include "pitches.h"

enum SoundType {
  SOUND_STARTUP,//スタート音
  SOUND_AHO,//
  SOUND_BEEP,//ビープ音
  SOUND_LOSS,//ショパンの悲しいやつ
  SOUND_COUNTDOWN_TICK,//カウントダウン
  SOUND_COUNTDOWN_END,//カウントダウン終了
  SOUND_SHOT,//弾を撃つ
  SOUND_CURSOR_MOVE,//カーソル移動音
  SOUND_POWER_UP,//パワーアップ音
  SOUND_DAMAGE,//ダメージ音
};

class SoundManager {
public:
  SoundManager();
  void begin(int pin);
  void playSound(SoundType type);
  void update();  // メロディ再生中に呼び出す必要あり

private:
  int buzzerPin;
  int* currentMelody;
  int* currentDurations;
  int melodyLength;
  int currentNote;
  unsigned long noteStartTime;
  unsigned long noteDuration;

  void setMelody(SoundType type);
};