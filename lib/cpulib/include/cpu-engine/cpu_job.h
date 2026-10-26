#pragma once

class cpu_job
{
public:
	void Create(cpu_thread_job* pThread);

	cpu_thread_job* GetThread() { return m_pThread; }

	virtual void OnJob(int iTile) = 0;

protected:
	cpu_thread_job* m_pThread;
};

class cpu_job_entity : public cpu_job
{
public:
	void OnJob(int iTile) override;
};

class cpu_job_particle_physics : public cpu_job
{
public:
	void OnJob(int iTile) override;
};

class cpu_job_particle_space : public cpu_job
{
public:
	void OnJob(int iTile) override;
};

class cpu_job_particle_render : public cpu_job
{
public:
	void OnJob(int iTile) override;
};
