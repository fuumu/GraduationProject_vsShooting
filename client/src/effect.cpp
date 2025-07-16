#include "effect.h"

Effect::Effect(Pos _pos, Vec _vec)
{
    //初期位置
    m_pos = _pos;
    //大きさ
    m_width = 1;
    m_height = 1;
    //オブジェクトID
    m_id = EFFECT;

    m_pos = _pos;
    m_vec = _vec;
}

void Effect::action()
{
    // 方向に応じて移動
    m_pos.x += m_vec.x;
    m_pos.y += m_vec.y;

    //念のため一定時間後に消滅
    m_createdTime++;
    if(m_createdTime >600)
    {
        m_isAlive = false;
    }

    //画面外に出たら消滅
    if (m_pos.x + m_vec.x > SCREEN_WIDTH)
    {
        m_isAlive = false;
    }
    if (m_pos.x + m_vec.x < 0 - m_width)
    {
        m_isAlive = false;
    }
    if (m_pos.y + m_vec.y > SCREEN_HEIGHT)
    {
        m_isAlive = false;
    }
    if (m_pos.y + m_vec.y < 0 - m_height)
    {
        m_isAlive = false;
    }
}

void Effect::draw()
{
    //点
    display.drawPixel(m_pos.x, m_pos.y, WHITE);
}