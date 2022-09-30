#pragma once

namespace nsK2EngineLow
{
    /// <summary>
    /// ブルームクラス
    /// </summary>
    class Bloom
    {
    public:
        /// <summary>
        /// 初期化
        /// </summary>
        /// <param name="mainRenderTarget">メインレンダリングターゲット</param>
        void Init(RenderTarget& mainRenderTarget);
        /// <summary>
        /// 描画
        /// </summary>
        /// <param name="rc">レンダリングコンテキスト</param>
        /// <param name="mainRenderTarget">メインレンダリングターゲット</param>
        void Render(RenderContext& rc,RenderTarget& mainRenderTarget);

    private:
        /// <summary>
        /// 輝度抽出用のレンダリングターゲットの初期化
        /// </summary>
        void InitLuminanceRenderTarget();
        /// <summary>
        /// 輝度抽出用のスプライトの初期化
        /// </summary>
        /// <param name="mainRenderTarget">メインレンダリングターゲット</param>
        void InitLuminanceSprite(RenderTarget& mainRenderTarget);
        /// <summary>
        /// ガウシアンブラーの初期化
        /// </summary>
        void InitGaussianBlur();
        /// <summary>
        /// ボケ画像加算合成スプライトの初期化
        /// </summary>
        void InitPlusBokeSprite();
        /// <summary>
        /// メインレンダリングターゲットに描画された絵をフレームバッファにコピーするためのスプライトの初期化
        /// </summary>
        void InitCopyToFrameBufferSprite(RenderTarget& mainRenderTarget);

        /// <summary>
        /// 輝度抽出
        /// </summary>
        /// <param name="rc"></param>
        void TakeLuminanceSprite(RenderContext& rc);
        /// <summary>
        /// ガウシアンブラー
        /// </summary>
        /// <param name="rc"></param>
        void ExcuteGaussianBlur(RenderContext& rc);
        /// <summary>
        /// ボケ画像とメインレンダリングターゲットの加算合成
        /// </summary>
        /// <param name="rc"></param>
        /// <param name="mainRenderTarget"></param>
        void PulsBokeSprite(RenderContext& rc,RenderTarget& mainRenderTarget);
        /// <summary>
        /// メインレンダリングターゲットの絵をフレームバッファにコピー
        /// </summary>
        /// <param name="rc">レンダリングコンテキスト</param>
        void CopyToFrameBufferSprite(RenderContext& rc);

        RenderTarget   m_luminanceRenderTarget;
        Sprite         m_luminanceSprite;
        Sprite         m_finalSprite;
        Sprite         m_copyToFrameBufferSprite;
        GaussianBlur   m_gaussianBlur[4];
    };
};