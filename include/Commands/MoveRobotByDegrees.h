#define ACCELERATION_DURATION 100
#define DECELERATION_DURATION 100
#include "TrapezoidalProfile.h"
struct RobotDrivetrainData
{
	CommandType commandType;
	bool isConfigured;
	TrapezoidalStates leftState;
	TrapezoidalStates rightState;
	short leftDuration;
	short rightDuration;
};
const int commandSize = sizeof(RobotDrivetrainData);
void initMoveByDegreeData(short leftDuration, short rightDuration, TrapezoidalStates leftState, TrapezoidalStates rightState, bool isRecovery = false)
{
	VERIFY(commandSize <= sizeof(BaseCommand));

	RobotDrivetrainData data;
	data.commandType = isRecovery ? (CommandType)MoveByDegreesRec : (CommandType)MoveByDegrees;
	data.isConfigured = false;
	data.leftDuration = leftDuration;
	data.rightDuration = rightDuration;
	data.leftState = leftState;
	data.rightState = rightState;

	memcpy(&bot.commands[bot.index], &data, commandSize);

	bot.index++;
}

void configure(RobotDrivetrainData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	initTrapezoidalProfileData((MotorData&)bot.left, data->leftDuration, data->leftState, ACCELERATION_DURATION, DECELERATION_DURATION, bot.left.motorPort.position());
	initTrapezoidalProfileData((MotorData&)bot.right, data->rightDuration, data->rightState, ACCELERATION_DURATION, DECELERATION_DURATION, bot.right.motorPort.position());
	motorDataChecksAndProcedures((MotorData&)bot.left);
	motorDataChecksAndProcedures((MotorData&)bot.right);
	resetCurrentPosition((MotorData&)bot.left, bot.left.motorPort.position());
	resetCurrentPosition((MotorData&)bot.right, bot.right.motorPort.position());

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(RobotDrivetrainData));
}

bool hasRan(RobotDrivetrainData* data)
{
	bool isLeftDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.left, bot.left.motorPort.position());
	bool isRightDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.right, bot.right.motorPort.position());

	if(isLeftDone && isRightDone)
	{
		return true;
	}

	return false;
}

void cleanup(RobotDrivetrainData* data)
{
	bot.left.motorPort.stop();
	bot.right.motorPort.stop();
}
