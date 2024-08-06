#include "MMORPGRobot.h"

FMMORPGRobot::FMMORPGRobot()
{

}

void FMMORPGRobot::InitGate(const FString& Host, const int32 Port)
{
	GateRobot.Init(Host, Port);
}

void FMMORPGRobot::InitDS(const FString& Host, const int32 Port)
{
	DSRobot.Init(Host, Port);
}

void FMMORPGRobot::Tick(float DeltaTime)
{
	GateRobot.Tick(DeltaTime);
	DSRobot.Tick(DeltaTime);
}

void FMMORPGRobot::RunRobot()
{
	GateRobot.RunRobot();
}