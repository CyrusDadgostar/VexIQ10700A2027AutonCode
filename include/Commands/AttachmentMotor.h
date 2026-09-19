#pragma once
struct MoveAttachMotorData
{
	CommandType commandType;
	bool isConfigured;
	uint8_t power;
	unsigned short duration;
};

void initMoveAttachMotorData(uint8_t power, unsigned short duration, bool isRecovery = false)
{
	VERIFY(sizeof(MoveAttachMotorData) <= sizeof(BaseCommand));

	MoveAttachMotorData data;
	data.commandType = isRecovery ? (CommandType)AttachMotorRec : (CommandType)AttachMotor;
	data.isConfigured = false;

	data.power = power;
	data.duration = duration;

	memcpy(&bot.commands[bot.index], &data, sizeof(MoveAttachMotorData));

	bot.index++;

}

void configure(MoveAttachMotorData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));

	bot.center.motorPort.setPosition(0);
}

bool hasRan(MoveAttachMotorData* data)
{
	if((unsigned)abs(bot.center.motorPort.position()) >= data->duration)
	{
		return true;
	}
	bot.center.motorPort.spin((double)data->power);


	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));
	return false;
}

void cleanup(MoveAttachMotorData* data)
{
	bot.center.motorPort.stop();
}
