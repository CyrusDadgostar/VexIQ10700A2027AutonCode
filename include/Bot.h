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

	vexMotorGroup() : firstMotor(), secondMotor() {}
	vexMotorGroup(vex::motor firstMotor, vex::motor secondMotor)
		: firstMotor(firstMotor), secondMotor(secondMotor)
	{
	}

	void stopMotors()
	{
		firstMotor.stop();
		secondMotor.stop();
	}
	void stop()
	{
		stopMotors();
	}
	void spinMotors(int power)
	{
		firstMotor.spin((vex::directionType)forward, power, pct);
		secondMotor.spin((vex::directionType)forward, power, pct);
	}
	void spin(int power)
	{
		spinMotors(power);
	}
	float positionOfMotors()
	{
		return (firstMotor.position(degrees) + secondMotor.position(degrees))/2;
	}
	float position()
	{
		return positionOfMotors();
	}
	float rotation()
	{
		return positionOfMotors();
	}
	void setPositionOfMotors(int position)
	{
		firstMotor.setPosition(position, degrees);
		secondMotor.setPosition(position, degrees);
	}
	void setPosition(int position)
	{
		setPositionOfMotors(position);
	}
};

struct StallData
{
	int previousTime;
	bool isInitiated;
	bool isPassed;
};

struct TrapData
{
	int previousPosition;
	short accDuration;
	short dccDuration;
	short totalDuration;
	short maxPower;
	TrapezoidalStates state;
	bool isConfigured;
};

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
