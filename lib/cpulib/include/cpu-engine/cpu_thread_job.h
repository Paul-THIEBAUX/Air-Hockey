#pragma once

class cpu_thread_job : public cpu_thread
{
public:
	~cpu_thread_job();

	void Create(int count);
	void Stop();
	void PostStartEvent(cpu_job* pJob);
	void PostEndEvent();
	void WaitStartEvent();
	void WaitEndEvent();
	bool IsWorking() { return m_isWorking; }
	void OnCallback() override;

protected:
	int m_count;
	HANDLE m_hEventStart;
	HANDLE m_hEventEnd;
	cpu_job* m_pJob;
	bool m_isWorking;
};
