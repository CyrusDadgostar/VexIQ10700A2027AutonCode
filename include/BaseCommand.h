#include <cassert>

#ifndef VERIFY
#define VERIFY(condition) do { if (!(condition)) { assert(#condition && false); } } while (0)
#endif

enum CommandType
{
	NoOperation = 0x00,
	MoveByDegrees = 0x01,
	AttachMotor = 0x02,
	TurnByGyro = 0x03,
	AttachMotorRec = 0x81,
	MoveByDegreesRec = 0x82,
	StopRecovery = 0x84,
	Stop = 0x05,
};

struct BaseCommand
{
	CommandType commandType;
	bool isConfigured;
	uint8_t commandBuffer[14];
};
