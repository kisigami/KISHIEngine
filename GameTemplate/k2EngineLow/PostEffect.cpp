#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		//�u���[���̏�����
		m_bloom.Init(mainRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�u���[���̕`��
		m_bloom.Render(rc, mainRenderTarget);
	}
}