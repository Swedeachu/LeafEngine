#ifndef DIRECT_WRAPPER_H
#define DIRECT_WRAPPER_H

#include "..\stdafx.h"
#include "..\LeafEngine.h"

namespace Graphics
{

	void InitializeDirectX12(Engine::LeafEngine& engine);

	void CreateRenderTargetViews(Engine::LeafEngine& engine);

	void CreateDepthStencilView(Engine::LeafEngine& engine);

}

#endif // DIRECT_WRAPPER_H
