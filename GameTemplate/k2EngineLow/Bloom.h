#pragma once

namespace nsK2EngineLow
{
    class Bloom
    {
    public:
        void Init(RenderTarget& mainRenderTarget);
        void Render(RenderContext& rc,RenderTarget& mainRenderTarget);

  
        void InitLight();
        void InitLuminanceRenderTarget();
        void InitLuminanceSprite(RenderTarget& mainRenderTarget);
        void InitGaussianBlur();
        void InitFinalSprite();

        void TakeLuminanceSprite(RenderContext& rc);
        void ExcuteGaussianBlur(RenderContext& rc);
        void PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget);

    private:
        RenderTarget   m_luminanceRenderTarget;
        Sprite         m_luminanceSprite;
        Sprite         m_finalSprite;
        Sprite         m_copyToFrameBufferSprite;
        GaussianBlur   m_gaussianBlur[4];
    };
    extern Bloom g_bloom;
};