#include <Core/TextureCubemap.hpp>
#include <cstring>

TextureCubemap::TextureCubemap(
	const unsigned int id, 
	const Texture* texFront,
	const Texture* texBack, 
	const Texture* texTop, 
	const Texture* texBottom, 
	const Texture* texLeft, 
	const Texture* texRight
) :
id(id),
texFront(texFront),
texBack(texBack),
texTop(texTop),
texBottom(texBottom),
texLeft(texLeft),
texRight(texRight)
{

}

TextureCubemap::~TextureCubemap()
{
	delete texFront;
	delete texBack;
	delete texTop;
	delete texBottom;
	delete texLeft;
	delete texRight;
}

unsigned int TextureCubemap::getId() const
{
	return id;
}

const Texture* TextureCubemap::getTexFront() const
{								 
	return texFront;			 
}								 
								 
const Texture* TextureCubemap::getTexBack() const
{								 
	return texBack;				 
}								 
								 
const Texture* TextureCubemap::getTexTop() const
{								 
	return texTop;				 
}								 
								 
const Texture* TextureCubemap::getTexBottom() const
{								  
	return texBottom;			  
}								  
const Texture* TextureCubemap::getTexLeft() const
{								 
	return texLeft;				 
}								 
								 
const Texture* TextureCubemap::getTexRight() const
{
	return texRight;
}

