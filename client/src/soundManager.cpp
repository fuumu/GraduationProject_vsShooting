#include "SoundManager.h"

SoundManager::SoundManager()
    : buzzerPin(-1), currentMelody(nullptr), currentDurations(nullptr),
      melodyLength(0), currentNote(0), noteStartTime(0), noteDuration(0),
       currentSoundType(SOUND_NONE),loopPlayback(false) {}

void SoundManager::begin(int pin)
{
  buzzerPin = pin;
}

void SoundManager::playSound(SoundType type, bool loop)
{
  //enumを元に優先順位が低いサウンドは無視する
  if(currentMelody != nullptr && currentSoundType != SOUND_NONE)
  {
    if(type > currentSoundType)
    {
      return;
    }
  }

  setMelody(type);
  currentNote = 0;
  noteStartTime = millis();
  loopPlayback = loop;
}

void SoundManager::stopSound()
{
  noTone(buzzerPin);
  currentMelody = nullptr;
  currentDurations = nullptr;
  melodyLength = 0;
  currentNote = 0;
  noteDuration = 0;
  currentSoundType = SOUND_NONE;
  loopPlayback = false;
}

void SoundManager::update()
{
  // nullptrなら何もしない
  if (currentMelody == nullptr || currentDurations == nullptr)
  {
    return;
  }

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
    if (loopPlayback)
    {
      currentNote = 0;
      noteStartTime = now;
    }
    else
    {
      stopSound();
    }
  }
}

void SoundManager::setMelody(SoundType type)
{
  //サウンドタイプを保存
  currentSoundType = type;

  // 再生するサウンドの設定
  switch (type)
  {
    // タイトルジングル
  case SOUND_STARTUP:
  {
    static int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
    static int durations[] = {8, 16, 16, 8, 8, 8, 8, 8};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  case SOUND_SHOW:
  {
    static int melody[] = {NOTE_G5, NOTE_C6 , NOTE_E6};
    static int durations[] = {16, 16, 16};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  // ショパン
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
  // 表彰式
  case SOUND_WIN:
  {
    static int melody[] = {
        NOTE_C5, NOTE_A4, NOTE_AS4, NOTE_C5, NOTE_F4, 
        NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4,};
    static int durations[] = {
        2, 2, 8, 2, 2, 
        8, 8, 8, 8, 4, 4, 1,};

    currentMelody = melody;
    currentDurations = durations;
    melodyLength = sizeof(melody) / sizeof(melody[0]);
    break;
  }
  //保留音
  case SOUND_WAIT:
  {
    static int melody[] = {
        NOTE_GS5, NOTE_B4, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_B4,
        NOTE_GS5, NOTE_C5, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_REST, NOTE_G5,
        NOTE_GS5, NOTE_B4, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_CS6, NOTE_CS6, NOTE_CS6,
        NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_CS5, NOTE_DS5, NOTE_E5, NOTE_REST};
    
    static int durations[] = {
        2, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2,
        2, 4, 4, 4, 4, 4, 4, 2, 2, 2, 4, 4,
        2, 4, 4, 4, 4, 4, 4, 2, 2, 2,
        4, 4, 2, 4, 4, 2, 2, 1, 2,};

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
  //相手に弾が当たった音
  case SOUND_HIT:
  {
    static int melody[] = {NOTE_FS5, NOTE_B5};
    static int durations[] = {16, 16};

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
  // エラー音
  case SOUND_ERROR:
  {
    static int melody[] = {NOTE_E2,NOTE_DS2,NOTE_D2,NOTE_CS2,NOTE_C2};
    static int durations[] = {16,16,16,16,16};

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
  case SOUND_NONE:
  {
    currentMelody = nullptr;
    currentDurations = nullptr;
    melodyLength = 0;
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