#include "Ball.h"
#include "Library/resourceLoader.h"

Ball::Ball()
{
	Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/lizard.mv1"));
}

Ball::~Ball()
{
}

void Ball::Update()
{
	Object3D::Update();
}

void Ball::Draw()
{
	Object3D::Draw();
}
