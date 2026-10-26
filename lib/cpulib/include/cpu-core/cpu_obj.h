#pragma once

struct cpu_mesh;

namespace cpu_obj
{
	struct obj_index
	{
		int v  = 0;
		int vt = 0;
		int vn = 0;

		bool hasVT = false;
		bool hasVN = false;
	};

	int ResolveIndex(int index, int count);
	bool ParseIndex(const std::string& str, obj_index& result);
	bool Load(cstr path, cpu_mesh* pMesh);
	bool Load(byte* file, int size, cpu_mesh* pMesh);
	std::string GetLine(byte* stream, int& offset, int size);
}
