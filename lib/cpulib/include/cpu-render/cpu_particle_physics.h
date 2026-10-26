#pragma once

struct cpu_particle_physics
{
public:
	float gx;
	float gy;
	float gz;
	float drag;					// 0.. (ex: 1..5)
	float maxSpeed;				// 0 = no clamp

	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;
	bool useBounds;
	float bounce;				// 0..1

public:
	cpu_particle_physics();
};
