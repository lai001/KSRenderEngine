#include "D3D11/D3D11Shader.hpp"
#include <functional>
#include <d3dcompiler.h>

namespace ks
{
	D3D11Shader::~D3D11Shader()
	{
		D3D11RenderEngineInfo engineInfo;

		assert(vertexShaderBlob);
		assert(vertexShader);
		assert(pixelShaderBlob);
		assert(pixelShader);
		assert(inputLayout);

		vertexShaderBlob->Release();
		vertexShader->Release();
		pixelShaderBlob->Release();
		pixelShader->Release();
		inputLayout->Release();
	}

	D3D11Shader * D3D11Shader::create(const std::string & vertexShaderSource,
		const std::string & fragmentShaderSource,
		const std::vector<UniformInfo> createInfos,
		const ks::VertexBufferLayout& layout,
		const D3D11RenderEngineInfo & engineInfo)
	{
		ID3DBlob* vertexShaderBlob = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;
		ID3DBlob* pixelShaderBlob = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr;
		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11DeviceContext *context = engineInfo.context;
		assert(d3dDevice);
		assert(context);

		std::function<void()> cleanBlock = [&]()
		{
			if (vertexShaderBlob) { vertexShaderBlob->Release(); }
			if (vertexShader) { vertexShader->Release(); }
			if (pixelShaderBlob) { pixelShaderBlob->Release(); }
			if (pixelShader) { pixelShader->Release(); }
			if (inputLayout) { inputLayout->Release(); }
		};
		defer { cleanBlock(); };

		ID3DBlob* errorMesg = nullptr;
		HRESULT status = S_OK;

		status = D3DCompile(vertexShaderSource.c_str(), vertexShaderSource.size(), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vertexShaderBlob, &errorMesg);
		if (status != S_OK) { return nullptr; }
		status = d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
		if (status != S_OK) { return nullptr; }
		if (errorMesg != nullptr) { return nullptr; }

		status = D3DCompile(fragmentShaderSource.c_str(), fragmentShaderSource.size(), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &pixelShaderBlob, &errorMesg);
		if (status != S_OK) { return nullptr; }
		status = d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
		if (status != S_OK) { return nullptr; }
		if (errorMesg != nullptr) { return nullptr; }

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs = getInputElementDescs(layout);

		assert(vertexShaderBlob);
		status = d3dDevice->CreateInputLayout(inputElementDescs.data(), inputElementDescs.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
		if (status != S_OK) { return nullptr; }

		D3D11Shader * shader = new D3D11Shader();
		shader->engineInfo = engineInfo;
		shader->inputLayout = inputLayout;
		shader->pixelShader = pixelShader;
		shader->pixelShaderBlob = pixelShaderBlob;
		shader->vertexShader = vertexShader;
		shader->vertexShaderBlob = vertexShaderBlob;
		cleanBlock = []() { };
		return shader;
	}

	void D3D11Shader::bind() const
	{
		ID3D11DeviceContext *context = engineInfo.context;
		assert(context);
		assert(inputLayout);
		assert(vertexShader);
		assert(pixelShader);
		context->IASetInputLayout(inputLayout);
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);
	}

	void D3D11Shader::unbind() const
	{
		ID3D11DeviceContext *context = engineInfo.context;
		assert(context);
		context->IASetInputLayout(nullptr);
		context->VSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
	}

	void D3D11Shader::setUniform(const std::string & name, const UniformValue & value)
	{
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> D3D11Shader::getInputElementDescs(const VertexBufferLayout & layout)
	{
		typedef VertexBufferElement::Semantics Semantics;
		std::unordered_map<Semantics, char*> semanticsNameDic;
		semanticsNameDic[Semantics::position] = "POSITION";
		semanticsNameDic[Semantics::color] = "COLOR";

		std::unordered_map<VertexBufferElement::Type, std::string> typeNameDic;
		typeNameDic[VertexBufferElement::Type::f32] = "f32.";

		std::unordered_map<std::string, DXGI_FORMAT> dxgiFormatDic;
		dxgiFormatDic[std::string("f32.2")] = DXGI_FORMAT_R32G32_FLOAT;
		dxgiFormatDic[std::string("f32.3")] = DXGI_FORMAT_R32G32B32_FLOAT;
		dxgiFormatDic[std::string("f32.4")] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;

		const std::vector<ks::VertexBufferElement> elements = layout.getElements();
		unsigned int offset = 0;
		D3D11_INPUT_ELEMENT_DESC desc;
		for (size_t i = 0; i < elements.size(); i++)
		{
			const ks::VertexBufferElement element = elements[i];
			assert(semanticsNameDic.find(element.semantics) != semanticsNameDic.end());
			assert(typeNameDic.find(element.type) != typeNameDic.end());
			std::string typeName = typeNameDic.at(element.type);
			assert(dxgiFormatDic.find(typeName + std::to_string(element.count)) != dxgiFormatDic.end());
			DXGI_FORMAT format = dxgiFormatDic.at(typeName + std::to_string(element.count));
			desc.Format = format;
			desc.SemanticIndex = element.semanticsIndex;
			desc.SemanticName = semanticsNameDic.at(element.semantics);
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = offset;
			desc.InstanceDataStepRate = 0;
			inputElementDescs.push_back(desc);
			offset = element.count * ks::VertexBufferElement::getSize(element.type);
		}

		return inputElementDescs;
	}
}