#pragma once
#include <src/util/object3D/Object3D.h>

class Ball;
class Chara;

/// <summary>
/// ギミックの基底インターフェース
/// </summary>
class GimmickBase : public Object3D
{
public:
    GimmickBase()                                       {}
    virtual ~GimmickBase()                              {}
    virtual void Update() override                      {}
    virtual void Draw() override                        {}
    virtual void Initialize()                           {}
    virtual void Reset()                                {}
    virtual void OnBallHit(Ball* pBall)           {}
    virtual void OnCharaContact(Chara* pChara)    {}
};