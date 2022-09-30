#pragma once
#include "Bloom.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// �|�X�g�G�t�F�N�g�N���X
	/// </summary>
	class PostEffect
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="mainRenderTarget">�����_�����O�^�[�Q�b�g</param>
		void Init(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">�����_�����O�^�[�Q�b�g</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		Bloom    m_bloom;  //�u���[��
	};
};