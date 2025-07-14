#include "SoundManager.h"

SoundManager::SoundManager()
    : buzzerPin(-1), currentMelody(nullptr), currentDurations(nullptr),
      melodyLength(0), currentNote(0), noteStartTime(0), noteDuration(0) {}

void SoundManager::begin(int pin)
{
  buzzerPin = pin;
}

void SoundManager::playSound(SoundType type)
{
  setMelody(type);
  currentNote = 0;
  noteStartTime = millis();
}

void SoundManager::update()
{
  // nullptrなら何もしない
  if (currentMelody == nullptr || currentDurations == nullptr)
    return;

  unsigned long now = millis();

  // 再生中
  if (currentNote < melodyLength)
  {
    // 次の音を再生する時間になったら再生
    if (now - noteStartTime >= noteDuration)
    {
      int note = currentMelody[currentNote];
      int duration = 1000 / currentDurations[currentNote];
      noteDuration = duration * 1.2;

      // 休符以外は再生
      if (note > 0)
      {
        tone(buzzerPin, note, duration);
      }
      else
      {
        // 休符なら再生を止める
        noTone(buzzerPin);
      }

      noteStartTime = now;
      currentNote++;
    }
  }
  // 再生終了（最後の noteDuration 経過後に止める）
  else if (now - noteStartTime >= noteDuration)
  {
    noTone(buzzerPin);
    currentMelody = nullptr;
  }
}

void SoundManager::setMelody(SoundType type)
{
  // 再生するサウンドの設定
  switch (type)
  {
    // タイトルジングル
  case SOUND_STARTUP:
  {
    static int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
    static int durations[] = {4, 8, 8, 4, 4, 4, 4, 4};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  case SOUND_AHO:
  {
    static int melody[] = {NOTE_A1, NOTE_A2};
    static int durations[] = {4, 4};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  // ビープ音
  case SOUND_BEEP:
  {
    static int melody[] = {NOTE_C3};
    static int durations[] = {16};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  // ショパンの悲しいやつ
  case SOUND_LOSS:
  {
    static int melody[] = {
        NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3,
        NOTE_D4, NOTE_CS4, NOTE_CS4, NOTE_B3,
        NOTE_B3, NOTE_AS3, NOTE_B3};
    static int durations[] = {
        2, 2, 8, 2,
        2, 8, 2, 8,
        2, 8, 2};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //カウントダウン
  case SOUND_COUNTDOWN_TICK:
  {
    static int melody[] = {NOTE_B5,};
    static int durations[] = {4};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //カウントダウン終了
  case SOUND_COUNTDOWN_END:
  {
    static int melody[] = {NOTE_B6,};
    static int durations[] = {2};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //カーソル移動音
  case SOUND_CURSOR_MOVE:
  {
    static int melody[] = {NOTE_FS4,};
    static int durations[] = {32};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //弾を撃つ
  case SOUND_SHOT:
  {
    static int melody[] = {NOTE_C3};
    static int durations[] = {16};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //パワーアップ音
  case SOUND_POWER_UP:
  {
    static int melody[] = {NOTE_C5, NOTE_CS5, NOTE_D5};
    static int durations[] = {16, 16, 16};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //ダメージ音
  case SOUND_DAMAGE:
  {
    static int melody[] = {NOTE_FS2, NOTE_F2};
    static int durations[] = {16, 16};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  default:
  {
    currentMelody = nullptr;
    currentDurations = nullptr;
    melodyLength = 0;
    break;
  }
  }
}