#pragma once

struct cpu_sprite : public cpu_object
{
public:
	cpu_texture* pTexture;
	int x;
	int y;
	int z;
	int anchorX;
	int anchorY;
	bool visible;

public:
	cpu_sprite();

	void CenterAnchor();
};
