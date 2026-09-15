#pragma once
struct MoveAttachMotorData
{
	CommandType commandType;
	bool isConfigured;
	byte power;
	unsigned short duration;
};

void initMoveAttachMotorData(byte power, unsigned short duration, bool isRecovery = false)
{
	VERIFY(sizeof(MoveAttachMotorData) <= sizeof(BaseCommand));

	MoveAttachMotorData data;
	data.commandType = isRecovery ? AttachMotorRec : AttachMotor;
	writeDebugStreamLine("data: %i", data.commandType);
	data.isConfigured = false;

	data.power = power;
	data.duration = duration;

	memcpy(&bot.commands[bot.index], &data, sizeof(MoveAttachMotorData));

	bot.index++;
	writeDebugStreamLine("NextCommand: %i CommandType: %i", bot.index, bot.commands[bot.index].commandType);

}

void configure(MoveAttachMotorData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = &bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));

	bot.center.motorPort.setPosition(0);
}

bool hasRan(MoveAttachMotorData* data)
{
	if((unsigned)abs(bot.center.motorPort.position()) >= data->duration)
	{
		return true;
	}
	bot.center.motorPort.spin(forward, data->power);


	BaseCommand* command = &bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));
	return false;
}

void cleanup(MoveAttachMotorData* data)
{
	bot.center.motorPort.stop();
}
