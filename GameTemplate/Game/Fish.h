#pragma once
class Fish:public IGameObject
{
public:
	Fish();
	~Fish();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;

};

