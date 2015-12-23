#pragma once

#include <Core/Texture.hpp>

class TextureCubemap
{
public:
	TextureCubemap(
		const unsigned int id,
		const Texture* texFront,
		const Texture* texBack,
		const Texture* texTop,
		const Texture* texBottom,
		const Texture* texLeft,
		const Texture* texRight
	);

	~TextureCubemap();

	unsigned int getId() const;
	const Texture* getTexFront() const;
	const Texture* getTexBack() const;
	const Texture* getTexTop() const;
	const Texture* getTexBottom() const;
	const Texture* getTexLeft() const;
	const Texture* getTexRight() const;

private:
	unsigned int id;
	const Texture* texFront;
	const Texture* texBack;
	const Texture* texTop;
	const Texture* texBottom;
	const Texture* texLeft;
	const Texture* texRight;
};