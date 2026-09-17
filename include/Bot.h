#include "BaseCommand.h"
#include "vex.h"

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
	
	void initVexMotorGroup(vex::motor firstPort, vex::motor secondPort)
	{
		firstMotor = firstPort;
		secondMotor = secondPort;
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
		firstMotor.setVelocity(power, vex::percent);
		secondMotor.setVelocity(power, vex::percent);
		firstMotor.spin((vex::directionType)vex::forward);
		secondMotor.spin((vex::directionType)vex::forward);
	}
	void spin(int power)
	{
		spinMotors(power);
	}
	float positionOfMotors()
	{
		return (firstMotor.position(vex::degrees) + secondMotor.position(vex::degrees))/2;
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
		firstMotor.setPosition(position, vex::degrees);
		secondMotor.setPosition(position, vex::degrees);
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

vex::timer time1 = vex::timer();
vex::timer time2 = vex::timer();
vex::timer time3 = vex::timer();

Bot bot;
