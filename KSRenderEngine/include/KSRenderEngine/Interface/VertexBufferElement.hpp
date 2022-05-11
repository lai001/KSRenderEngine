#ifndef KSRenderEngineVertexBufferElement
#define KSRenderEngineVertexBufferElement



namespace ks
{
	struct VertexBufferElement
	{
		enum class Type
		{
			byte,
			ubyte,
			int16,
			uint16,
			uint32,
			int32,
			f32,
			f64,
		};

		VertexBufferElement::Type type;
		unsigned int count;
		bool isNormalized;

		static unsigned int getSize(const VertexBufferElement::Type& type);

		explicit VertexBufferElement(const VertexBufferElement::Type type, const unsigned int count, const bool isNormalized);
	};
}


#endif // !KSRenderEngineVertexBufferElement
