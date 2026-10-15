#include "bot.h"
#pragma once
#define MIN_POWER 10
struct TurnByGyroData
{
	CommandType commandType;
	bool isConfigured;
    unsigned short gyroState;
	unsigned short angle;
};

void initTurnByGyroData( unsigned short angle, bool isRecovery = false)
{
	VERIFY(sizeof(TurnByGyroData) <= sizeof(BaseCommand));

	TurnByGyroData data;
	data.commandType = (CommandType)TurnByGyro;
	data.isConfigured = false;
	data.gyroState = 0;
	data.angle = angle;

	memcpy(&bot.commands[bot.index], &data, sizeof(TurnByGyroData));

	bot.index++;

}

void configure(TurnByGyroData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));

	bot.center.motorPort.setPosition(0);
}

bool hasRan(TurnByGyroData* data)
{
    switch(data->gyroState)
    {
        case 0:
        {
            bot.right.motorPort.spin(-bot.Gyro.rotation()-MIN_POWER);
            bot.left.motorPort.spin(bot.Gyro.rotation()+MIN_POWER);
            if(bot.Gyro.rotation() < data->angle/2) return false;

            data->gyroState = 1;
        }
        break;

        case 1:
        {
            int error = data->angle - bot.Gyro.rotation();
            bot.left.motorPort.spin(error+MIN_POWER);
            bot.right.motorPort.spin(-error-MIN_POWER);
            if(bot.center.motorPort.position() < data->angle) return false;

            return true;
        }
        break;
    }

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));
	return false;
}

void cleanup(TurnByGyroData* data)
{
	bot.left.motorPort.stop();
    bot.right.motorPort.stop();
    bot.center.motorPort.stop();
}
