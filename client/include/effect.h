#pragma once
#include "objBase.h"

// 弾構造体------------------------------------------------
struct Effect : public ObjBase
{
public:
  Effect(Pos _pos, Vec _vec);
  void action();
  void draw();
  ~Effect() {}

private:
  // 弾が生成された時間(自然消滅用)
  unsigned long m_createdTime = 0;
};