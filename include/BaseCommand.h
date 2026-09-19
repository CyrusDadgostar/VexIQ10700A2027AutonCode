#include <cassert>

#ifndef VERIFY
#define VERIFY(condition) do { if (!(condition)) { assert(#condition && false); } } while (0)
#endif

enum CommandType
{
	MoveByDegrees = 0x00,
	AttachMotor = 0x01,
	AttachMotorRec = 0x81,
	NoOperation = 0x03,
	Trapezoidal = 0x04,
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
