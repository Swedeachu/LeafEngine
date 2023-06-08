#ifndef DIRECT_WRAPPER_H
#define DIRECT_WRAPPER_H

#include "..\stdafx.h"
#include <unordered_map>

#include "..\LeafMath\Vector2D.h"

namespace Graphics
{

	class DirectWrapper
	{

	public:

		struct ConstantBufferData
		{
			// world view matrix starts as the 4x4 indentiy 
			DirectX::XMFLOAT4X4 worldViewMatrix{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
			// starts as all zeroes
			DirectX::XMFLOAT4X4 transformMatrix{
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			};
			DirectX::XMFLOAT4 tintColor{ 0.0f, 0.0f, 0.0f, 0.0f }; // rgba, starts as all zeroes for default no tint color
			DirectX::XMFLOAT2 texOffset{ 0.0f, 0.0f }; // basically a 2D vector for X and Y cords of how to offset textures
			float alpha{ 1.0f }; // default 1.0 fully visibile value with no transparency
			float padding{ 0.0f }; // IGNORE ME : we have to use this extra padding variable to make the struct an acceptable size for a CB
		};

		// Calls the needed functions to initialize directX and also gets the current world matrix
		void InitializeDirectX(HWND hwnd, int width, int height);

		// Releases all our directX objects
		void ReleaseDirectX();

		// Frame updater
		void BeginFrame();
		void EndFrame();

		// Set the blend mode to use on the next draw
		// void SetBlendMode(BlendMode mode);
		void InitBlendMode();

		// Set the sampler state to use on the next draw
		// void SetSamplerState(TextureSampleMode sampleMode, TextureAddressMode addressMode);
		void InitSamplerState();

		// Update the DirectX constant buffer with the current stored data
		void UpdateConstantBuffer();

		// Set the transformation matrix for the constant buffer
		void SetConstantBufferTransform(const DirectX::XMFLOAT4X4* matrix);

		// Plug in the data to create a matrix transform via composition
		void SetConstantBufferTransformData(const LeafMath::Vector2D& position, const LeafMath::Vector2D& scale, float rotationRadians);

		// Set the texture offset for the constant buffer
		void SetConstantBufferTextureOffset(const DirectX::XMFLOAT2& offset);

		// Set the alpha value for the constant buffer,
		// default is 1.0f alpha (ranges between 0.0f - 1.0f)
		void SetConstantBufferAlpha(float alpha);

		// Set the tint color for the constant buffer,
		// default tint color is RGBA {0, 0, 0, 0}
		void SetConstantBufferTintColor(const DirectX::XMFLOAT4* color);

		// Set the world matrix for the constant buffer
		void SetWorldViewMatrix(const DirectX::XMFLOAT4X4& matrix);

		// Deals with the window size changing
		void HandleWindowResize(HWND hwnd);

		// getters for what a client might need to access (mesh.cpp for example would need to access the device)
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		IDXGISwapChain* GetSwapChain() const;
		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11VertexShader* GetVertexShader() const;
		ID3D11PixelShader* GetPixelShader() const;

		// Get constant buffer
		ID3D11Buffer* GetConstantBuffer() const;

		// Get input layout
		ID3D11InputLayout* GetInputLayout() const;

	private:

		ConstantBufferData constantBuffer;
		ID3D11Buffer* perObjectBuffer;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11InputLayout* inputLayout;

		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11PixelShader* pixelTextureShader;

		ID3DBlob* vertexShaderBlob;
		ID3DBlob* pixelShaderBlob;
		ID3DBlob* pixelTexShaderBlob;

		// Map of enums for D3D blend state objects
		enum class BlendStates { None, Transparent, Add, Multiply };
		std::unordered_map<BlendStates, ID3D11BlendState*> blendStateMap;

		// sample modes
		enum class SampleModes { Linear, Point };
		// texture address modes
		enum class TextureAddressModes { Wrap, Mirror, Clamp, Mirror_Once };
		// sample mode values to maps of texture address modes to D3D sampler state objects
		std::unordered_map <SampleModes, std::unordered_map<TextureAddressModes, ID3D11SamplerState*>> samplerStateMap;

		// Creates the directX device and the swap chain
		void CreateDeviceAndSwapChain(HWND hwnd);

		// Creates the render target view from the swap chain
		void CreateRenderTargetView();

		// Creates our rasterizer state
		void CreateRasterizerState();

		// Creates the blend state for all the possible blend settings
		void CreateBlendStates();

		// Create the directX constant buffer we write to
		void CreateConstatBuffer();

		// Sets the view port on the device context
		void SetViewPort(HWND hwnd);

		// Creates sampler states for all sample and texture address modes
		void CreateSamplers();

		// Load the shaders from cso files
		// This will create our required vertex shader and the two pixel shaders for drawing shapes, colors, and textures
		void LoadShaders();

	};

} // Graphics

#endif // DIRECT_WRAPPER_H
