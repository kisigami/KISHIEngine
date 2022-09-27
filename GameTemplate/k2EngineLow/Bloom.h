#pragma once

namespace nsK2EngineLow
{
    class Bloom
    {
    public:

        void Init(RenderTarget& mainRenderTarget);
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

    private:

        struct SSamplingLuminanceCB1 {
            int isTonemap;      // トーンマップが有効？
            float middlegray;   // ミドルグレー。
            float threshold = 1.0f;
        };

        RenderTarget m_luminanceRenderTarget;	//輝度抽出用のレンダリングターゲット
        Sprite m_luminanceSprite;				//輝度抽出用のスプライト
        GaussianBlur m_gaussianBlur[4];			//ガウシアンブラー
        Sprite m_finalSprite;					//最終合成用のスプライト
        SSamplingLuminanceCB1 m_samplingLuminanceCB1;
    };
};