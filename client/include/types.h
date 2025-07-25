#pragma once
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "soundManager.h"//ブザー

#include <stdint.h>

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define STICK_X A0
#define STICK_Y A1
#define BUTTON_A 8
#define BUZZER_PIN 7

#define EEPROM_ADDR_HISCORE 0

#define SCREEN_WIDTH 128 // OLEDの横幅
#define SCREEN_HEIGHT 64 // OLEDの縦幅

// 最大オブジェクト数
constexpr uint8_t MAX_OBJ = 128;

//ディスプレイ
extern Adafruit_SSD1306 display;
//ブザー
extern SoundManager sound;

//座標
typedef struct Point {
  float x;
  float y;
} Pos;

//ベクトル
typedef struct Vector {
  float x;
  float y;
} Vec;

// 列挙-------------------------------------------------------------
enum ObjID
{
    PLAYER1,
    PLAYER2,
    ENEMY,
    P1BULLET,
    P2BULLET,
	EFFECT,
  };
enum GameResult
{
	GAME_ERROR = -1,
	GAME_RUNNING = 0,
	GAME_FINISHED,
};

// ロゴ------------------------------------------------------------
//   'logo_test', 128x64px
const unsigned char epd_bitmap_logo_test[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xee, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00,
	0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00,
	0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xde, 0x00, 0x00,
	0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8e, 0x00, 0x00,
	0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x0c,
	0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x0e,
	0x00, 0x10, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46,
	0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
	0x00, 0x1c, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x70, 0x60,
	0x00, 0x0e, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0xe0, 0x00,
	0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x01, 0xfc, 0x00,
	0x00, 0x00, 0x0f, 0x00, 0x00, 0x7e, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x70, 0x00, 0x03, 0xfe, 0x00,
	0x00, 0x00, 0x3c, 0x00, 0x00, 0xff, 0xe0, 0x03, 0xff, 0x01, 0x00, 0x30, 0x03, 0x07, 0x86, 0x00,
	0x00, 0x00, 0xf8, 0x38, 0x01, 0xc1, 0xf0, 0x01, 0x80, 0x03, 0x80, 0x00, 0x03, 0x0e, 0x06, 0x00,
	0x00, 0x03, 0xe0, 0x7c, 0x00, 0x00, 0x00, 0x03, 0x80, 0x07, 0x00, 0x00, 0x07, 0x1c, 0x06, 0x00,
	0x00, 0x0f, 0x80, 0x0c, 0x00, 0x00, 0x00, 0x07, 0x00, 0x0e, 0x00, 0x00, 0x0e, 0x38, 0x0e, 0x00,
	0x00, 0x3e, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x3c, 0x00, 0x00, 0x7c, 0x30, 0x1c, 0x00,
	0x00, 0x78, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x7c, 0x00, 0x00, 0xf8, 0x00, 0x38, 0x00,
	0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x6c, 0x00, 0x03, 0xc0, 0x00, 0x30, 0x00,
	0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x0f, 0xe0, 0x00, 0x4c, 0x00, 0x07, 0x80, 0x00, 0x70, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x0c, 0x00, 0x1e, 0x00, 0x00, 0xe0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x01, 0xfc, 0x00, 0x01, 0xc0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xf0, 0x00, 0x01, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x03, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
	0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x70, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00,
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
	0x00, 0x79, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
	0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};