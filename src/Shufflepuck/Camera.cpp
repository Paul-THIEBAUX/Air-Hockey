#include "pch.h"
#include "Camera.h"


void Camera::Init() {
	cpuEngine.GetCamera()->transform.SetPosition(20.0f, 10.0f, 0.0f);
	cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.0f, 0.0f, CPU_VEC3_UP);
}

void Camera::Update() {
}

