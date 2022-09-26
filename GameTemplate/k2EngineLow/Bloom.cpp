#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	void Bloom::Init()
	{
		//メインレンダリングターゲット作成
		m_mainRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//輝度抽出用のレンダリングターゲットを作成
		m_luminanceRenderTarget.Create(
			1600, 
			900, 
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//輝度抽出用のスプライトを初期化
		m_luminanceSpriteInitData.m_fxFilePath = "Assets/shader/sample.fx";
		m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	}
};
