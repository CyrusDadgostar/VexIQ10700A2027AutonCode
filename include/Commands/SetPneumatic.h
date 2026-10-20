#include "bot.h"
#pragma once
#define MIN_POWER 15

const vex::cylinderType Claw = vex::cylinderType::cylinder1;
const vex::cylinderType Wrist = vex::cylinderType::cylinder2;


struct SetPneumaticData
{
	CommandType commandType;
	bool isConfigured;
    bool isClawExtended;
    bool isWristExtended;
};


void initSetPneumaticData(bool isClawExtended, bool isWristExtended, bool isRecovery = false)
{
	VERIFY(sizeof(SetPneumaticData) <= sizeof(BaseCommand));

	SetPneumaticData data;
	data.commandType = (CommandType)SetPneumatic;
	data.isConfigured = false;
	data.isClawExtended = isClawExtended;
	data.isWristExtended = isWristExtended;

	memcpy(&bot.commands[bot.index], &data, sizeof(SetPneumaticData));

	bot.index++;

}

void configure(SetPneumaticData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(SetPneumaticData));

}

bool hasRan(SetPneumaticData* data)
{
    if(data->isClawExtended)
    {
        
        bot.claw.clawPneumatic.extend(Claw);
    }
    else
    {
        bot.claw.clawPneumatic.retract(Claw);
    }

    if(data->isWristExtended)
    {
        bot.claw.clawPneumatic.extend(Wrist);
    }
    else
    {
        bot.claw.clawPneumatic.retract(Wrist);
    }

    
	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(SetPneumaticData));
	return true;
}

void cleanup(SetPneumaticData* data)
{
	bot.left.motorPort.stop();
    bot.right.motorPort.stop();
    bot.center.motorPort.stop();
}
