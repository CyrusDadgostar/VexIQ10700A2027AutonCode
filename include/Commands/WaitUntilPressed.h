#pragma once
struct WaitUntilLedPressedData
{
	CommandType commandType;
	bool isConfigured;
	vex::color Color;
	uint8_t brightness;
};

void initWaitUntilPressedData(vex::color Color, uint8_t brightness)
{
	VERIFY(sizeof(WaitUntilLedPressedData) <= sizeof(BaseCommand));

	WaitUntilLedPressedData data;

    data.Color = Color;
    data.brightness = brightness;

	memcpy(&bot.commands[bot.index], &data, sizeof(WaitUntilLedPressedData));

	bot.index++;

}

void configure(WaitUntilLedPressedData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));
}

bool hasRan(WaitUntilLedPressedData* data)
{
	while(!bot.button.Led.pressing())
    {
        return false;
    }
    bot.button.Led.setBrightness(data->brightness);
    bot.button.Led.setColor(data->Color);


	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));
	return true;
}

void cleanup(WaitUntilLedPressedData* data)
{

}
