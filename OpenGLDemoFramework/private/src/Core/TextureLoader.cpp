#include <Core/TextureLoader.hpp>
#include <Core/TextureFactory.hpp>
#include <FreeImage.h>

TextureLoader::TextureLoader()
{
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif
}


TextureLoader::~TextureLoader()
{
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif
}

Texture* TextureLoader::loadTexture(const std::string path)
{
	Texture* tex = 0;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0), bpp(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(path.c_str(), 0);

	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());

	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, path.c_str());

	//if the image failed to load, return failure
	if (!dib)
		return tex;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	bpp = FreeImage_GetBPP(dib);

	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0) || (bpp == 0))
	{
		//Error
	}
	else
	{
		tex = TextureFactory::createTexture(width, height, bpp / 8, bits);
		FreeImage_Unload(dib);
	}

	return tex;
}

TextureCubemap* TextureLoader::loadTextureCubemap(
	const std::string pathFront,
	const std::string pathBack,
	const std::string pathTop,
	const std::string pathBottom,
	const std::string pathLeft,
	const std::string pathRight)
{
	Texture* texFront  = loadTexture(pathFront);
	Texture* texBack   = loadTexture(pathBack);
	Texture* texTop    = loadTexture(pathTop);
	Texture* texBottom = loadTexture(pathBottom);
	Texture* texLeft   = loadTexture(pathLeft);
	Texture* texRight  = loadTexture(pathRight);

	return TextureFactory::createTextureCubemap(
			texFront,
			texBack,
			texTop,
			texBottom,
			texLeft,
			texRight
		);
}