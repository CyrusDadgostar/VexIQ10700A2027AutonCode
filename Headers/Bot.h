#include "BaseCommand.h"

int sgn(float val) {
    return (0 < val) - (val < 0);
}

enum TrapezoidalStates
{
	Acc,
	Coast,
	Dcc
};

struct vexMotorGroup
{
	vex::motor firstMotor;
	vex::motor secondMotor;
	void spin(int power)
	{
		firstMotor.spin(forward, power);
		secondMotor.spin(forward, power);
	}
	float position()
	{
		return (firstMotor.position() + secondMotor.position())/2
	}
	void setPosition(int position)
	{
		firstMotor.setPosition(position);
		secondMotor.setPosition(position);
	}
}

struct StallData
{
	int previousTime;
	bool isInitiated;
	bool isPassed;
}

struct TrapData
{
	int previousPosition;
	short accDuration;
	short dccDuration;
	short totalDuration;
	byte maxPower;
	TrapezoidalStates state;
	bool isConfigured;
}

struct MotorData
{
	vexMotorGroup motorPort;
	TrapData profile;
	int previousEncoderValue;
	float velocity;
	int previousTime;
	StallData stallData;
};

struct Bot
{
	int index;
	bool isRecovery;
	bool isCheckForRecovery;
	CommandType currentBotIndexCommand;
	BaseCommand commands[MAX_COMMAND_LENGTH];
	MotorData left;
	MotorData right;
	MotorData center;
};

timer time1 = timer();
timer time2 = timer();
timer time3 = timer();

Bot bot;
