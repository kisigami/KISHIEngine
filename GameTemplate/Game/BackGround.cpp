#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
}

bool BackGround::Start()
{
	m_modelRender.Init("Assets/modelData/bg/bg.tkm");
	m_modelRender.Update();
	return true;
}

void BackGround::Update()
{
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
