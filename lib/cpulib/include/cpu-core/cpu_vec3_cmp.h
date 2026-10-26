#pragma once

struct cpu_vec3_cmp
{
	bool operator()(const XMFLOAT3& a, const XMFLOAT3& b) const
	{
		const float eps = 0.001f;
		if ( fabsf(a.x-b.x)>eps )
			return a.x<b.x;
		if ( fabsf(a.y-b.y)>eps )
			return a.y<b.y;
		return a.z<(b.z-eps);
	}
};
