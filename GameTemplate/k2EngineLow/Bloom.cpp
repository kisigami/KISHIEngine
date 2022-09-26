#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
	void Bloom::Init()
	{
		//���C�������_�����O�^�[�Q�b�g�쐬
		m_mainRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬
		m_luminanceRenderTarget.Create(
			1600, 
			900, 
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�P�x���o�p�̃X�v���C�g��������
		m_luminanceSpriteInitData.m_fxFilePath = "Assets/shader/sample.fx";
		m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	}
};
