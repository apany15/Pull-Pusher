#pragma once

class classTimeControl
{
private:
	double old_time_mark;
	bool run;
public:
	classTimeControl(bool start=false);
	void RunReset();
	double GetChangeDifferenceTime();
	double GetChangeSafeDifferenceTime(double safeTime);

	double GetDifferenceTime();

	void SetDifferenceTimeZero();
	void SetDifferenceTime(double timeSec);

	~classTimeControl(void);
protected:
	double GetCurTime();
};
