#pragma once

struct cpu_callback
{
public:
	cpu_function<void()> onStart;
	cpu_function<void()> onUpdate;
	cpu_function<void()> onExit;
	cpu_function<void(int)> onRender;
};
