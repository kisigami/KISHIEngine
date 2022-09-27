#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow
{
    void Bloom::Init(RenderTarget& mainRenderTarget)
    {
        m_luminanceRenderTarget.Create(
            mainRenderTarget.GetWidth(),   // 解像度はメインレンダリングターゲットと同じ
            mainRenderTarget.GetHeight(),  // 解像度はメインレンダリングターゲットと同じ
            1,
            1,
            mainRenderTarget.GetColorBufferFormat(),
            DXGI_FORMAT_D32_FLOAT
        );
    }

    void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        
    };
};
