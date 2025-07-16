#include "scene_title.h"
#include "scene_singleMode.h"
#include "scene_versusMode.h"
#include "sceneManager.h"

#define LOGO_DURATION 120

Title::Title(SceneManager *p) : SceneBase(p)
{
    sound.playSound(SOUND_STARTUP);
}

int Title::update()
{
    // しばらくロゴを表示
    if (m_timer < LOGO_DURATION)
    {
        m_timer++;
        //メニューを表示する瞬間に効果音を鳴らす
        if(m_timer == LOGO_DURATION)
        {
            sound.playSound(SOUND_SHOW);
        }
    }
    else
    {
        // スティックでモードを選択
        if (800 < analogRead(STICK_Y))
        {
            if (m_vsmode == false)
            {
                m_vsmode = true;
                sound.playSound(SOUND_CURSOR_MOVE);
            }
        }
        else if (400 > analogRead(STICK_Y))
        {
            if (m_vsmode == true)
            {
                m_vsmode = false;
                sound.playSound(SOUND_CURSOR_MOVE);
            }
        }

        // ボタンが押されたら選択中のモードで開始
        if (digitalRead(BUTTON_A) == HIGH)
        {
            if (m_vsmode)
            {
                sceneManager->deleteScene();
                sceneManager->currentScene = new VersusMode(sceneManager); // 対戦モードに移動
            }
            else
            {
                sceneManager->deleteScene();
                sceneManager->currentScene = new SingleMode(sceneManager); // シングルモードに移動
            }
        }
    }

    return GAME_RUNNING;
}

void Title::draw()
{
    // 表示するロゴ
    if (m_timer < LOGO_DURATION)
    {
        display.drawBitmap(0, 0, epd_bitmap_logo_test, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE); // ロゴ
    }
    else
    { // 選択画面
        // モード選択中に表示する
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("Please select mode");

        display.setCursor(15, 20);
        display.print("single mode");

        display.setCursor(15, 30);
        display.print("versus mode");

        // カーソルの描画位置
        if (m_vsmode)
        {
            display.setCursor(0, 30);
        }
        else
        {
            display.setCursor(0, 20);
        }
        display.print(">");
    }
}

Title::~Title()
{
}