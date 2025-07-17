#include "CharaSpawnPoint.h"

CharaSpawnPoint::CharaSpawnPoint()
    : m_InitialVelocity(Vector3::Zero)
{/* DO NOTHING */}

CharaSpawnPoint::~CharaSpawnPoint()
{/* DO NOTHING */}

void CharaSpawnPoint::Init(const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc)
{
    *transform = trs;
    m_InitialVelocity = desc.SPAWN_INITIAL_VELOCITY;
}

void CharaSpawnPoint::Draw()
{
    Object3D::Draw();
}
