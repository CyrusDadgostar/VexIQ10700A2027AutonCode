
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
	initTrapezoidalProfileData(bot.left, data->leftDuration, data->leftState);
	initTrapezoidalProfileData(bot.right, data->rightDuration, data->rightState);
	motorDataChecksAndProcedures(bot.left);
	motorDataChecksAndProcedures(bot.right);
	resetCurrentPosition(bot.left);
	resetCurrentPosition(bot.right);

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(RobotDrivetrainData));
}

bool hasRan(RobotDrivetrainData* data)
{
	bool isLeftDone = TrapezoidalProfileMoveByDegrees(bot.left);
	bool isRightDone = TrapezoidalProfileMoveByDegrees(bot.right);

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
