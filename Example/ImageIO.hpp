#ifndef KSRenderEngine_Example_ImageIO_hpp
#define KSRenderEngine_Example_ImageIO_hpp

#include <string>
#include <memory>
#include <Foundation/Foundation.hpp>
#include <OpenImageIO/imageio.h>

class ImageIO
{
public:
	static std::unique_ptr<ks::PixelBuffer> readImageFromFilePath2(const std::string& path);
	static bool saveImage(const ks::PixelBuffer& buffer, const std::string& targetPath);
};

#endif // !KSRenderEngine_Example_ImageIO_hpp