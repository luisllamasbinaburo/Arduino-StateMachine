
# Librería Arduino StateMachine

La librería StateMachine implementa una máquina de estados que puede ejecutarse en un procesador como Arduino.

Más información https://www.luisllamas.es/libreria-arduino-statemachine/

## Instrucciones de uso

La máquina de estados se inicializa indicando el número de estados y transiciones. Ambos, estados y transiciones, están identificados por un entero de 8bits (0 a 255).
Los estados simbolizan las distintas configuraciones en las que puede estar la máquina. Las transiciones vinculan dos estados, uno de salida y uno de llegada, y tienen asociada una condición de disparo.

La máquina de estados tiene un estado actual. Para actualizar el estado debemos llamar frecuentemente a la función Update(), que comprueba las transiciones que tienen como entrada el estado actual, y las condiciones asociadas.

Si alguna de las transiciones asociadas al estado actual cumple la condición de disparo, la maquina para a estar en el estado final de la transicion activada.

Las transiciones pueden, opcionalmente, ejecutar una función de callback asociada en el momento del disparo. Por otro lado, los estados pueden tener una acción de callback al entrar o abandonar el estado.

Para configurar la máquina debemos configurar correctamente las transiciones. Para ello podemos emplear las funciones SetTransition(...), indicando explícitamente el número de la transición de configurar. También podemos usar las funciones AddTransition(...), que incrementan un contador interno 0 a numTransitions-1, lo cuál la hace más sencilla de emplear para la configuración inicial de la máquina.

En el uso, conviene crear enumeraciones para los Estados, y es posible que para las transiciones.

### Constructor

```c++
StateMachine(uint8_t numStates, uint8_t numTransitions);
```
### Métodos generales
```c++
// Configurar una transición
void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition);
void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);
	
// Configurar una transición, usa un contador interno. Util para inicialización inicial.
uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition);
uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);

// Deshabilitar una transición
void RemoveTransition(uint8_t transition);
	
// Configurar acciones entrada y salida de un estado
void SetOnEntering(uint8_t state, StateMachineAction action);
void SetOnLeaving(uint8_t state, StateMachineAction action);
void ClearOnEntering(uint8_t state);
void ClearOnLeaving(uint8_t state);
	
// Cambiar la maquina a un estado
void SetState(uint8_t state, bool launchLeaving, bool launchEntering);
uint8_t GetState() const;
	
// Actualizar el estado de la maquina
// Esta función tiene que ser llamada frecuentemente desde el bucle principal
bool Update();
```

## Ejemplos
La librería StateMachine incluye los siguientes ejemplos para ilustrar su uso.

* StateMachine: Ejemplo que muestra el uso de StateMachine.
```c++
#include "StateMachineLib.h"

// Enumeraciones para facilitar el uso
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

// Sacado como función independiente por claridad
void setupStateMachine()
{
	// Configurar transiciones
	// Ejemplo con funciones lambda
	stateMachine.AddTransition(PosicionA, PosicionB, []() { return input == Forward; });

	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionB, PosicionC, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionB, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionC, PosicionB, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionC, PosicionD, []() { return input == Forward; });
	stateMachine.AddTransition(PosicionC, PosicionA, []() { return input == Reset; });

	stateMachine.AddTransition(PosicionD, PosicionC, []() { return input == Backward; });
	stateMachine.AddTransition(PosicionD, PosicionA, []() { return input == Reset; });

	// Configurar eventos de estado
	// Ejemplo con funciones normales
	stateMachine.SetOnEntering(PosicionA, outputA);
	stateMachine.SetOnEntering(PosicionB, outputB);
	stateMachine.SetOnEntering(PosicionC, outputC);
	stateMachine.SetOnEntering(PosicionD, outputD);

	// Ejemplo con funciones lambda
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
	// Recibir un comando por puerto serie
	input = static_cast<Input>(readInput());
	
	// Actualizar el estado de la maquina
	stateMachine.Update();
}

// Funcion auxiliar que simula la recepcion de un evento
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

// Acciones de estado
// Visualizan el estado del ejemplo
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
```

* Herencia: Ejemplo que muestra el uso de StateMachine mediante una clase derivada.
```c++
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

Input input;

class MyStateMachine : public StateMachine
{
public:
	MyStateMachine() : StateMachine(4, 9)
	{
		setupStateMachine();
	}

	void setupStateMachine()
	{
		AddTransition(PosicionA, PosicionB, []() { return input == Forward; });

		AddTransition(PosicionB, PosicionA, []() { return input == Backward; });
		AddTransition(PosicionB, PosicionC, []() { return input == Forward; });
		AddTransition(PosicionB, PosicionA, []() { return input == Reset; });

		AddTransition(PosicionC, PosicionB, []() { return input == Backward; });
		AddTransition(PosicionC, PosicionD, []() { return input == Forward; });
		AddTransition(PosicionC, PosicionA, []() { return input == Reset; });

		AddTransition(PosicionD, PosicionC, []() { return input == Backward; });
		AddTransition(PosicionD, PosicionA, []() { return input == Reset; });

		SetOnEntering(PosicionA, []()
		{
			Serial.println("A   B   C   D");
			Serial.println("X            ");
			Serial.println();
		});

		SetOnEntering(PosicionB, []()
		{
			Serial.println("A   B   C   D");
			Serial.println("X            ");
			Serial.println();
		});
		
		SetOnEntering(PosicionC, []()
		{
			Serial.println("A   B   C   D");
			Serial.println("        X    ");
			Serial.println();
		});

		SetOnEntering(PosicionD, []()
		{
			Serial.println("A   B   C   D");
			Serial.println("            X");
			Serial.println();
		});

		SetOnLeaving(PosicionA, []() {Serial.println("Leaving A"); });
		SetOnLeaving(PosicionB, []() {Serial.println("Leaving B"); });
		SetOnLeaving(PosicionC, []() {Serial.println("Leaving C"); });
		SetOnLeaving(PosicionD, []() {Serial.println("Leaving D"); });
	}
};

MyStateMachine myStateMachine;

void setup() 
{
	Serial.begin(9600);

	myStateMachine.SetState(State::PosicionA, false, true);
}

void loop() 
{
	input = static_cast<Input>(readInput());
	myStateMachine.Update();
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
```
