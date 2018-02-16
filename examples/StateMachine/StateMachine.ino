#include "StateMachineLib.h"

enum State
{
	PosicionA = 0,
	PosicionB = 1,
	PosicionC = 2,
	PosicionD = 3
};

enum Input
{
	Reset = 0,
	Forward = 1,
	Backward = 2,
	Unknown = 3,
};

StateMachine stateMachine(4, 9);
Input input;

void setupStateMachine()
{
	stateMachine.AddTransition(PosicionA, PosicionB, []() { return input == Forward; });

	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionB, PosicionC, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionC, PosicionB, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionC, PosicionD, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionC, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionD, PosicionC, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionD, PosicionA, []() { return input == Reset; });

	stateMachine.SetOnEntering(PosicionA, outputA);
	stateMachine.SetOnEntering(PosicionB, outputB);
	stateMachine.SetOnEntering(PosicionC, outputC);
	stateMachine.SetOnEntering(PosicionD, outputD);

	stateMachine.SetOnLeaving(PosicionA, []() {Serial.println("Leaving A"); });
	stateMachine.SetOnLeaving(PosicionB, []() {Serial.println("Leaving B"); });
	stateMachine.SetOnLeaving(PosicionC, []() {Serial.println("Leaving C"); });
	stateMachine.SetOnLeaving(PosicionD, []() {Serial.println("Leaving D"); });
}

void setup() 
{
	Serial.begin(9600);

	Serial.println("Starting State Machine...");
	setupStateMachine();	
	Serial.println("Start Machine Started");

	stateMachine.SetState(PosicionA, false, true);
}

void loop() 
{
	input = static_cast<Input>(readInput());

	stateMachine.Update();
}

int readInput()
{
	Input currentInput = Input::Unknown;
	if (Serial.available())
	{
		char incomingChar = Serial.read();

		switch (incomingChar)
		{
			case 'R': currentInput = Input::Reset; 	break;
			case 'A': currentInput = Input::Backward; break;
			case 'D': currentInput = Input::Forward; break;
			default: break;
		}
	}

	return currentInput;
}

void outputA()
{
	Serial.println("A   B   C   D");
	Serial.println("X            ");
	Serial.println();
}

void outputB()
{
	Serial.println("A   B   C   D");
	Serial.println("    X        ");
	Serial.println();
}

void outputC()
{
	Serial.println("A   B   C   D");
	Serial.println("        X    ");
	Serial.println();
}

void outputD()
{
	Serial.println("A   B   C   D");
	Serial.println("            X");
	Serial.println();
}