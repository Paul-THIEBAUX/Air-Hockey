#pragma once

struct cpu_ray
{
public:
	XMFLOAT3 pos;
	XMFLOAT3 dir;

public:
	cpu_ray();

	void Identity();
	void XM_CALLCONV ToLocal(cpu_ray& out, FXMMATRIX invWorld);
};
