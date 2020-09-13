#pragma once


struct ExposeInfo
{
	int time;           // Exposure time (ms)
	int binning;        // Binning mode
	int center;         // Center area(full=0,256=1,512=2,1024=3,2048=4)
	int trigger;        // Trigger mode(not used=0,synchronize=2,exposure=3)
	int mode;           // A/D(12bit=ON,16bit=OFF) & Continue
	int emgain;         // EM-Gain(x1=0,x2=1,x5=3,x10=4,x20=5)
	int gainup;         // Gain-up mode(OFF=0,ON=1, When there is not a function is -1)
	int fandelay;       // Fan mode(Delay time(ms) > 0, When there is not a function is -1)
	bool abort;         // If you stop it, become true
};
