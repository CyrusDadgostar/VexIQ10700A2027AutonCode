#define ACCELERATION_DURATION 15
#define DECELERATION_DURATION 15
#include "TrapezoidalProfile.h"
struct TurnByGyroData
{
	CommandType commandType;
	bool isConfigured;
	TrapezoidalStates leftState;
	TrapezoidalStates rightState;
	short angle;
};

void initTurnByGyroData(short angle, TrapezoidalStates leftState, TrapezoidalStates rightState, bool isRecovery = false)
{
	VERIFY(commandSize <= sizeof(BaseCommand));

	TurnByGyroData data;
	data.commandType = isRecovery ? (CommandType)MoveByDegreesRec : (CommandType)MoveByDegrees;
	data.isConfigured = false;
	data.angle = angle;
	data.leftState = leftState;
	data.rightState = rightState;

	memcpy(&bot.commands[bot.index], &data, sizeof(TurnByGyroData));

	bot.index++;
}

void configure(TurnByGyroData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	initTrapezoidalProfileData((MotorData&)bot.left, data->angle, data->leftState, ACCELERATION_DURATION, DECELERATION_DURATION, bot.gyro.rotation(degrees));
	initTrapezoidalProfileData((MotorData&)bot.right, data->angle, data->rightState, ACCELERATION_DURATION, DECELERATION_DURATION, bot.gyro.rotation(degrees));
	motorDataChecksAndProcedures((MotorData&)bot.left);
	motorDataChecksAndProcedures((MotorData&)bot.right);
	resetCurrentPosition((MotorData&)bot.left, bot.gyro.rotation(degrees));
	resetCurrentPosition((MotorData&)bot.right, bot.gyro.rotation(degrees));

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));
}

bool hasRan(TurnByGyroData* data)
{
	bool isLeftDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.left, bot.gyro.rotation(degrees));
	bool isRightDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.right, bot.gyro.rotation(degrees));

	if(isLeftDone && isRightDone)
	{
		return true;
	}

	return false;
}

void cleanup(TurnByGyroData* data)
{
	bot.left.motorPort.stop();
	bot.right.motorPort.stop();
}
