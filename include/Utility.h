#include "bot.h"

void finalize()
{

	bot.index++;
	if(bot.isRecovery)
	{

		return;
	}

	while( (bot.commands[bot.index].commandType & 0x80) > 0)
	{
		bot.index++;
	}


}


int distanceToDegrees(int mm)
{
	return (mm*180)/(M_PI*RADIUS_OF_WHEEL);
}

float degreesToMove(int angle)
{
 	float length = RADIUS_OF_ROBOT * angle;
 	float degrees = length/RADIUS_OF_WHEEL;
 	return degrees;
}
