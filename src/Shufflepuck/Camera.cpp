#include "pch.h"
#include "Camera.h"


void Camera::Init() {
	cpuEngine.GetCamera()->transform.SetPosition(50.0f, 10.0f, 0.0f);
	cpuEngine.GetCamera()->transform.LookAt(45.0f, 0.0f, 0.0f, CPU_VEC3_UP);

}

/* Debug */
void Camera::Update() {

	float dt = cpuTime.delta;

	if (cpuInput.vi.IsKeyPressed(VK_RETURN)) {
		cpuEngine.GetCamera()->transform.SetPosition(20.0f, 10.0f, 0.0f);
		cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	}

#ifdef _DEBUG 
	if (cpuInput.IsUp()) {
		cpuEngine.GetCamera()->transform.SetPosition(15.0f, 5.0f, 0.0f);
		cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	}
	if (cpuInput.IsLeft()) {
		cpuEngine.GetCamera()->transform.SetPosition(0.1f, 15.0f, 0.0f);
		cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	}
	if (cpuInput.IsUp()) {
		cpuEngine.GetCamera()->transform.SetPosition(15.0f, 5.0f, 15.0f);
		cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	}
	if (cpuInput.IsRight()) {
		cpuEngine.GetCamera()->transform.SetPosition(15.0f, 15.0f, 15.0f);
		cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
	}
#endif

	XMFLOAT3 v = cpuEngine.GetCamera()->transform.pos;
	int a = 0;
}

