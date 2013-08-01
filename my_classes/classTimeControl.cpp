#include "classTimeControl.h"
#include<windows.h>
classTimeControl::classTimeControl(bool start)
{
	if (start)
	{
		old_time_mark=GetCurTime();
	}
	else old_time_mark=0;
	run=start;
}
void classTimeControl::RunReset()
{
	run=true;
	SetDifferenceTimeZero();
}
double classTimeControl::GetChangeDifferenceTime()
{
	double s_sys_time=GetCurTime();
	double outTime;
	if (!run)
	{
		run=true;
		outTime=0;
	}
	else 
	{
		outTime=s_sys_time-old_time_mark;
	}
	old_time_mark=s_sys_time;
	return outTime;
}
double classTimeControl::GetChangeSafeDifferenceTime(double safeTime)
{
	double outTime=GetChangeDifferenceTime();
	if (outTime<=0) outTime=safeTime;
	return outTime;
}
double classTimeControl::GetDifferenceTime()
{
	double s_sys_time=GetCurTime();
	if (!run)
		return 0;
	return s_sys_time-old_time_mark;
}

void classTimeControl::SetDifferenceTimeZero()
{
	if (run) old_time_mark=GetCurTime();
}
void classTimeControl::SetDifferenceTime(double timeSec)
{
	if (timeSec<0) timeSec=0;
	old_time_mark=timeSec;
}
classTimeControl::~classTimeControl(void)
{
}
double classTimeControl::GetCurTime()
{
	LPSYSTEMTIME systime=new _SYSTEMTIME() ;
	GetSystemTime(systime);
	double s_sys_time=((double)(systime->wMilliseconds))/1000+systime->wSecond+systime->wMinute*60;
	return s_sys_time;
}