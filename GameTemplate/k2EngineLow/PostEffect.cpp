#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
    void PostEffect::Init(RenderTarget& mainRenderTarget)
    {
        m_bloom.Init(mainRenderTarget);
       
    }
    void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        m_bloom.Render(rc, mainRenderTarget);
    }
}