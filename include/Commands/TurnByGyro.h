#include "bot.h"
#pragma once
#define MIN_POWER 15

enum TriangleState
{
    AccT,
    DccT
};

struct TurnByGyroData
{
	CommandType commandType;
	bool isConfigured;
    TriangleState gyroState;
	short angle;
};

void changeState(TriangleState& state, TriangleState newState)
{
    printf("CurrState: %d, NewState: %d\n", state, newState);
    state = newState;
}

void initTurnByGyroData( unsigned short angle, bool isRecovery = false)
{
	VERIFY(sizeof(TurnByGyroData) <= sizeof(BaseCommand));

	TurnByGyroData data;
	data.commandType = (CommandType)TurnByGyro;
	data.isConfigured = false;
	data.gyroState = AccT;
	data.angle = sgn(angle) == -1 ? angle-10 : angle+10;

	memcpy(&bot.commands[bot.index], &data, sizeof(TurnByGyroData));

	bot.index++;

}

void configure(TurnByGyroData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));

	bot.Gyro.setRotation(0, rotationUnits::deg);
}

bool hasRan(TurnByGyroData* data)
{
    int gyroRotation = bot.Gyro.rotation();
    printf("Gyro rotation: %d, angle: %d\n", gyroRotation, data->angle);
    // switch(data->gyroState)
    // {
    //     case AccT:
    //     {
    //         int power = MIN_POWER;

    //         power = abs(power) * sgn(data->angle);

    //         bot.left.motorPort.spin(-power);

    //         bot.right.motorPort.spin(power);
           
    //         if(abs(bot.Gyro.rotation()) <= abs(data->angle)/2) return false;
            
    //         changeState(data->gyroState, DccT);
           
    //     }
    //     break;

    //     case DccT:
    //     {
           
    //         int power;

    //         int direction = sgn(data->angle);

    //         power = abs(power) * direction;

    //         bot.left.motorPort.spin(-power);

    //         bot.right.motorPort.spin(power);
    //         if(abs(bot.Gyro.rotation()) <= abs(data->angle)) return false;

    //         return true;
    //     }
    //     break;
    // }


    if(abs(bot.Gyro.rotation()) > abs(data->angle)) return true;
    
    int direction = sgn(data->angle);    

    bot.left.motorPort.spin(direction * MIN_POWER);
    bot.right.motorPort.spin(-direction * MIN_POWER);

    
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
