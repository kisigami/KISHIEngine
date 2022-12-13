#include "stdafx.h"
#include "Fish.h"

Fish::Fish()
{
}

Fish::~Fish()
{
}

bool Fish::Start()
{
	m_modelRender.Init("Assets/modelData/fish/fish.tkm");
	m_position = Vector3::Zero;
	m_scale = {100.0f,100.0f,100.0f};

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	return true;
}

void Fish::Update()
{
	m_modelRender.Update();
}

void Fish::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}