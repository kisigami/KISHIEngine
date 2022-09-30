#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		//ブルームの初期化
		m_bloom.Init(mainRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//ブルームの描画
		m_bloom.Render(rc, mainRenderTarget);
	}
}