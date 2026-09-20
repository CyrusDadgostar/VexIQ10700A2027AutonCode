#pragma once
#define MIN_POWER 5

#include "Bot.h"

//Every time you use nMotorEncoder, subtract the previous position so that everything work

//void resetEncoders(MotorData data)
//{
//	data.motorPort.rotation() = 0;
//}

void resetCurrentPosition(MotorData& data, int currentPosition)
{
	data.profile.previousPosition = currentPosition;
	data.profile.isConfigured = true;
}
int getcurrentPosition(MotorData& data, int currentPosition)
{
	return currentPosition - data.profile.previousPosition;
}
bool accelerate(MotorData& data, int sensorPosition)
{
	int currentPosition = getcurrentPosition(data, sensorPosition);
	if(abs(currentPosition) >= abs(data.profile.accDuration))
	{
		return true;
	}

	float powerPercentage = (float)abs(currentPosition)/abs(ACCELERATION_DURATION);

	int power = (powerPercentage*(data.profile.maxPower-MIN_POWER))+MIN_POWER;

	power = abs(power) * sgn(data.profile.totalDuration);

	data.motorPort.spin(power);



	return false;
}

bool stayAtSameSpeed(MotorData& data, int sensorPosition)
{
	int currentPosition = getcurrentPosition(data, sensorPosition);
	int trapezoidalB1 = abs(data.profile.totalDuration)-data.profile.dccDuration;
	if(abs(currentPosition) >= abs(trapezoidalB1))
	{
		return true;
	}

	return false;
}

bool decelerate(MotorData& data, int sensorPosition)
{
	int currentPosition = getcurrentPosition(data, sensorPosition);

	if(abs(currentPosition) >= abs(data.profile.totalDuration))
	{
		data.motorPort.stop();
		return true;
	}

	int maxpower = data.profile.maxPower;

	int adjustedPosition = (abs(data.profile.totalDuration) - DECELERATION_DURATION) - abs(currentPosition);
	int target = (abs(data.profile.totalDuration)-DECELERATION_DURATION);
	int percentageValue = data.profile.maxPower - MIN_POWER;

	int power = ((1-abs(adjustedPosition)/(float)DECELERATION_DURATION)*percentageValue)+MIN_POWER;


	power = abs(power) * sgn(data.profile.totalDuration);

	data.motorPort.spin(power);


	return false;
}


/*
This NOT intMotorData. It is initTrapezoidalProfile.

Trap profile is very disorganized.
pull this into it's own program.
*/

bool initTrapezoidalProfileData(MotorData& data, short totalDuration, TrapezoidalStates state, short accDuration, short dccDuration, int prevPosition)
{
	data.profile.state = state;
	data.profile.totalDuration = totalDuration;
	data.profile.accDuration = accDuration;
	data.profile.dccDuration = dccDuration;
	data.profile.maxPower = 100; // why not a define? 
	data.profile.previousPosition = prevPosition;
	data.profile.isConfigured = true;
	return true;
}

void motorDataChecksAndProcedures(MotorData& data)
{
	if(abs(data.profile.totalDuration) < data.profile.accDuration + data.profile.dccDuration)
	{
		float totalWindow = (float)(ACCELERATION_DURATION + DECELERATION_DURATION);
		data.profile.accDuration = (short)(ACCELERATION_DURATION * (abs(data.profile.totalDuration) / totalWindow));
		data.profile.dccDuration = (short)(abs(data.profile.totalDuration) - data.profile.accDuration);
	}
}

bool runMotorData(MotorData& data, short currentPosition)
{
	switch(data.profile.state)
	{
		case Acc:
		{
			bool isDone = accelerate(data, currentPosition);
			if(isDone)
			{
				data.profile.state = (TrapezoidalStates)Coast;
			}
		}
			break;

		case Coast:
		{
			bool isDone = stayAtSameSpeed(data, currentPosition);
			if(isDone)
			{
				data.profile.state = (TrapezoidalStates)Dcc;
			}
		}
			break;

		case Dcc:
		{
			bool isDone = decelerate(data, currentPosition);
			if(isDone)
			{
				data.stallData.isInitiated = false;
				return true;
			}
		}
			break;
	}
	return false;
}

bool TrapezoidalProfileMoveByDegrees(MotorData& data, short currentPosition)
{
	bool isDone = runMotorData(data, currentPosition);
	if(isDone)
	{
		return true;
	}
	return false;
}
