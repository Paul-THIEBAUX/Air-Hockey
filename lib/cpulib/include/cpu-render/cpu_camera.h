#pragma once

struct cpu_camera
{
public:
	bool perspective;
	float fov;					// perspective
	float aspectRatio;			// perspective
	float width;				// ortho
	float height;				// ortho
	float near;
	float far;
	XMFLOAT4X4 matView;
	XMFLOAT4X4 matProj;
	XMFLOAT4X4 matViewProj;
	cpu_transform transform;
	cpu_frustum frustum;

public:
	cpu_camera();

	void UpdateProjection();
	void Update();
};
