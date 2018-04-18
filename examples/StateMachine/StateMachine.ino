#include "StateMachineLib.h"

// State Alias
enum State
{
	PosicionA = 0,
	PosicionB = 1,
	PosicionC = 2,
	PosicionD = 3
};

// Input Alias
enum Input
{
	Reset = 0,
	Forward = 1,
	Backward = 2,
	Unknown = 3,
};

// Create new StateMachine
StateMachine stateMachine(4, 9);

// Stores last user input
Input input;

// Setup the State Machine
void setupStateMachine()
{
	// Add transitions
	stateMachine.AddTransition(PosicionA, PosicionB, []() { return input == Forward; });

	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionB, PosicionC, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionC, PosicionB, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionC, PosicionD, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionC, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionD, PosicionC, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionD, PosicionA, []() { return input == Reset; });

	// Add actions
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

	// Initial state
	stateMachine.SetState(PosicionA, false, true);
}

void loop() 
{
	// Read user input
	input = static_cast<Input>(readInput());

	// Update State Machine
	stateMachine.Update();
}

// Auxiliar function that reads the user input
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

// Auxiliar output functions that show the state debug
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
