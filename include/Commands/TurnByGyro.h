#include "bot.h"
#pragma once
#define MIN_POWER 5
struct TurnByGyroData
{
	CommandType commandType;
	bool isConfigured;
    unsigned short gyroState;
	short angle;
};

void changeState(unsigned short& state, unsigned short newState)
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

	bot.Gyro.setRotation(0, rotationUnits::deg);
}

bool hasRan(TurnByGyroData* data)
{
    int gyroRotation = bot.Gyro.rotation();
    printf("Gyro rotation: %d, angle: %d\n", gyroRotation, data->angle);
    switch(data->gyroState)
    {
        case 0: //make enum state for this instead of int
        {
            float powerPercentage = (float)abs(bot.Gyro.rotation())/abs(ACCELERATION_DURATION);

            int power = (powerPercentage*(100-MIN_POWER))+MIN_POWER;

            power = abs(power) * sgn(data->angle);

            bot.left.motorPort.spin(-power);

            bot.right.motorPort.spin(power);
           
            if(abs(bot.Gyro.rotation()) <= abs(data->angle)/2) return false;
            
            changeState(data->gyroState, 1);
           
        }
        break;

        case 1:
        {
           
            int adjustedPosition = (abs(data->angle) - data->angle/2) - abs(bot.Gyro.rotation());
            int target = (abs(data->angle)-data->angle/2);
            int percentageValue = 100 - MIN_POWER;

            int power = ((1-abs(adjustedPosition)/(float)DECELERATION_DURATION)*percentageValue)+MIN_POWER;

            int direction = sgn(data->angle);

            power = abs(power) * direction;

            bot.left.motorPort.spin(-power);

            bot.right.motorPort.spin(power);
            if(abs(bot.Gyro.rotation()) <= abs(data->angle)) return false;

            return true;
        }
        break;
    }


    // if(bot.Gyro.rotation() > data->angle) return true;
    
        
    // bot.left.motorPort.spin(MIN_POWER);
    // bot.right.motorPort.spin(-MIN_POWER);
    // return false;
    
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
