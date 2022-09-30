#pragma once

namespace nsK2EngineLow
{
    /// <summary>
    /// �u���[���N���X
    /// </summary>
    class Bloom
    {
    public:
        /// <summary>
        /// ������
        /// </summary>
        /// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
        void Init(RenderTarget& mainRenderTarget);
        /// <summary>
        /// �`��
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        /// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
        void Render(RenderContext& rc,RenderTarget& mainRenderTarget);

    private:
        /// <summary>
        /// �P�x���o�p�̃����_�����O�^�[�Q�b�g�̏�����
        /// </summary>
        void InitLuminanceRenderTarget();
        /// <summary>
        /// �P�x���o�p�̃X�v���C�g�̏�����
        /// </summary>
        /// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
        void InitLuminanceSprite(RenderTarget& mainRenderTarget);
        /// <summary>
        /// �K�E�V�A���u���[�̏�����
        /// </summary>
        void InitGaussianBlur();
        /// <summary>
        /// �{�P�摜���Z�����X�v���C�g�̏�����
        /// </summary>
        void InitPlusBokeSprite();
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�ɕ`�悳�ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏�����
        /// </summary>
        void InitCopyToFrameBufferSprite(RenderTarget& mainRenderTarget);

        /// <summary>
        /// �P�x���o
        /// </summary>
        /// <param name="rc"></param>
        void TakeLuminanceSprite(RenderContext& rc);
        /// <summary>
        /// �K�E�V�A���u���[
        /// </summary>
        /// <param name="rc"></param>
        void ExcuteGaussianBlur(RenderContext& rc);
        /// <summary>
        /// �{�P�摜�ƃ��C�������_�����O�^�[�Q�b�g�̉��Z����
        /// </summary>
        /// <param name="rc"></param>
        /// <param name="mainRenderTarget"></param>
        void PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget);
        /// <summary>
        /// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
        /// </summary>
        /// <param name="rc">�����_�����O�R���e�L�X�g</param>
        void CopyToFrameBufferSprite(RenderContext& rc);

        RenderTarget   m_luminanceRenderTarget;
        Sprite         m_luminanceSprite;
        Sprite         m_finalSprite;
        Sprite         m_copyToFrameBufferSprite;
        GaussianBlur   m_gaussianBlur[4];
    };
};