#include "D3D11/D3D11Shader.hpp"
#include <assert.h>
#include <functional>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Foundation/Foundation.hpp>
#include "Common/ShaderConductorHelper.hpp"
#include "Common/ShaderReflection.hpp"

namespace ks
{
	void D3D11Shader::initConstantBuffer(const std::vector<UniformInfo>& uniformInfos)
	{
		const PackingRules packingRules = getPackingRules();

		ID3D11Device *d3dDevice = engineInfo.device;
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		assert(d3dDevice);
		assert(d3dDeviceContext);

		HRESULT status = S_OK;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = caclSize(uniformInfos, packingRules);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		status = d3dDevice->CreateBuffer(&desc, nullptr, &constantBuffer);
		assert(status == S_OK);
	}

	size_t D3D11Shader::caclSize(const std::vector<UniformInfo>& uniformInfos, const PackingRules& packingRules)
	{
		size_t size = 0;
		for (size_t i = 0; i < uniformInfos.size(); i++)
		{
			const UniformValue::Type dataType = uniformInfos[i].type;
			const size_t dataTypeSize = getSize(dataType);
			const size_t forward = size % packingRules.boundary;
			if (forward + dataTypeSize > packingRules.boundary)
			{
				size = alignment(size, packingRules.boundary);
				size = size + dataTypeSize;
			}
			else
			{
				size = size + dataTypeSize;
			}
		}
		size = alignment(size, packingRules.boundary);
		return size;
	}

	size_t D3D11Shader::getOffset(const std::vector<UniformInfo>& uniformInfos, const PackingRules & packingRules, const UniformInfo & uniformInfo)
	{
		size_t size = 0;
		size_t offset;
		for (size_t i = 0; i < uniformInfos.size(); i++)
		{
			const UniformValue::Type dataType = uniformInfos[i].type;
			const size_t dataTypeSize = getSize(dataType);
			const size_t forward = size % packingRules.boundary;
			if (forward + dataTypeSize > packingRules.boundary)
			{
				size = alignment(size, packingRules.boundary);
				size = size + dataTypeSize;
			}
			else
			{
				size = size + dataTypeSize;
			}
			if (uniformInfo.name == uniformInfos[i].name)
			{
				offset = size - dataTypeSize;
				break;
			}
		}
		size = alignment(size, packingRules.boundary);
		return offset;
	}

	size_t D3D11Shader::getSize(const UniformValue::Type dataType)
	{
		std::unordered_map<UniformValue::Type, size_t> dic;
		dic[UniformValue::Type::i32] = 4;
		dic[UniformValue::Type::f32] = 4;
		dic[UniformValue::Type::vec2] = 2 * 4;
		dic[UniformValue::Type::vec3] = 4 * 3;
		dic[UniformValue::Type::vec4] = 4 * 4;
		dic[UniformValue::Type::mat3] = 3 * 3 * 4;
		dic[UniformValue::Type::mat4] = 4 * 4 * 4;
		assert(dic.end() != dic.find(dataType));
		const size_t dataTypeSize = dic.at(dataType);
		return dataTypeSize;
	}

	PackingRules D3D11Shader::getPackingRules() const noexcept
	{
		PackingRules packingRules;
		packingRules.pack = 4;
		packingRules.boundary = 16;
		return packingRules;
	}

	D3D11Shader::~D3D11Shader()
	{
		assert(vertexShaderBlob);
		assert(vertexShader);
		assert(pixelShaderBlob);
		assert(pixelShader);
		assert(inputLayout);
		assert(constantBuffer);

		vertexShaderBlob->Release();
		vertexShader->Release();
		pixelShaderBlob->Release();
		pixelShader->Release();
		inputLayout->Release();
		constantBuffer->Release();
	}

	D3D11Shader * D3D11Shader::create(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource,
		const std::vector<UniformBufferInfo> uniformBuffers,
		const std::vector<ShaderTexture2DInfo> texture2DInfos,
		const ks::VertexBufferLayout& layout,
		const D3D11RenderEngineInfo& engineInfo)
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
		defer{ cleanBlock(); };

		ID3DBlob* errorMesg = nullptr;
		HRESULT status = S_OK;

		status = D3DCompile(vertexShaderSource.c_str(), vertexShaderSource.size(), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vertexShaderBlob, &errorMesg);
		if (SUCCEEDED(status) == false)
		{
			const std::string errorInfo = std::string((const char*)errorMesg->GetBufferPointer(), errorMesg->GetBufferSize());
			return nullptr;
		}
		status = d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
		if (SUCCEEDED(status) == false)
		{
			return nullptr;
		}

		status = D3DCompile(fragmentShaderSource.c_str(), fragmentShaderSource.size(), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &pixelShaderBlob, &errorMesg);
		if (SUCCEEDED(status) == false)
		{
			const std::string errorInfo = std::string((const char*)errorMesg->GetBufferPointer(), errorMesg->GetBufferSize());
			return nullptr;
		}
		status = d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
		if (SUCCEEDED(status) == false)
		{
			return nullptr;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs = getInputElementDescs(layout);

		assert(vertexShaderBlob);
		status = d3dDevice->CreateInputLayout(inputElementDescs.data(), inputElementDescs.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
		if (SUCCEEDED(status) == false)
		{
			return nullptr;
		}

		D3D11Shader * shader = new D3D11Shader();
		shader->engineInfo = engineInfo;
		shader->inputLayout = inputLayout;
		shader->pixelShader = pixelShader;
		shader->pixelShaderBlob = pixelShaderBlob;
		shader->vertexShader = vertexShader;
		shader->vertexShaderBlob = vertexShaderBlob;
		shader->texture2DInfos = texture2DInfos;
		if (uniformBuffers.empty() == false)
		{
			shader->uniformInfos = uniformBuffers.at(0).uniformInfos;
			shader->initConstantBuffer(uniformBuffers.at(0).uniformInfos);
		}
		if (uniformBuffers.size() > 1)
		{
			assert(false); // TODO:
		}
		cleanBlock = []() {};
		return shader;
	}

	D3D11Shader * D3D11Shader::create(const std::string & vertexShaderSource,
		const std::string & fragmentShaderSource,
		const D3D11RenderEngineInfo & engineInfo)
	{
		VertexBufferLayout layout = ShaderReflection::getBufferLayout(vertexShaderSource);
		std::vector<UniformBufferInfo> uniformBuffers = ShaderReflection::getFragUniformBuffers(fragmentShaderSource);
		std::vector<ShaderTexture2DInfo> texture2DInfos = ShaderReflection::getFragTexture2DNmaes(fragmentShaderSource);
		return create(vertexShaderSource, fragmentShaderSource, uniformBuffers, texture2DInfos, layout, engineInfo);
	}

	void D3D11Shader::bind() const
	{
		ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
		assert(d3dDeviceContext);
		assert(inputLayout);
		assert(vertexShader);
		assert(pixelShader);
		d3dDeviceContext->IASetInputLayout(inputLayout);
		d3dDeviceContext->VSSetShader(vertexShader, nullptr, 0);
		d3dDeviceContext->PSSetShader(pixelShader, nullptr, 0);
		d3dDeviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
		d3dDeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
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
		bool isFind = false;
		for (const UniformInfo& info : uniformInfos)
		{
			if (info.name == name)
			{
				ID3D11DeviceContext *d3dDeviceContext = engineInfo.context;
				assert(d3dDeviceContext);
				assert(constantBuffer);
				D3D11_MAPPED_SUBRESOURCE constant_resource;
				HRESULT status = d3dDeviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constant_resource);
				assert(SUCCEEDED(status));
				const size_t dataTypeSize = getSize(value.type);
				const PackingRules packingRules = getPackingRules();
				const size_t offset = getOffset(uniformInfos, packingRules, info);
				memcpy(reinterpret_cast<char*>(constant_resource.pData) + offset, value.getData(), dataTypeSize);
				d3dDeviceContext->Unmap(constantBuffer, 0);
				isFind = true;
				break;
			}
		}
		assert(isFind);
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> D3D11Shader::getInputElementDescs(const VertexBufferLayout & layout)
	{
		typedef VertexBufferElement::Semantics Semantics;
		std::unordered_map<Semantics, char*> semanticsNameDic;
		semanticsNameDic[Semantics::position] = "POSITION";
		semanticsNameDic[Semantics::color] = "COLOR";
		semanticsNameDic[Semantics::texcoord] = "TEXCOORD";
		semanticsNameDic[Semantics::normal] = "NORMAL";

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

	void ks::D3D11Shader::setTexture2D(const std::string & name, const ITexture2D & texture2D)
	{
		bool isFind = false;

		for (size_t i = 0; i < texture2DInfos.size(); i++)
		{
			if (texture2DInfos[i].name == name)
			{
				texture2D.bind(i);
				isFind = true;
				break;
			}
		}
		assert(isFind);
	}
}