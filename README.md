# About
This project represents the interactive data portion of a game engine, allowing a developer to configure the behavior and data of a game object through C++ and JSON reflection.

# Details
Datum: A homogoneous, runtime-polymorphic data container. A single dataum can wrap one of multiple supported data types, currently: integers, floats, strings, GLM(OpenGL Mathematics) Vector4s, GLM Matrices, or Scopes. Datums are resizable much like a std vector, and can either wrap an object itself (establishing ownership), or a reference to that object.

Scope - A class that contains Datums mapped to string names. Ex: "Damage" - Datum(float), or "NumberOfAttacksPerTurn" - Datum(integer). This Scope - Datum relationship is how we describe every Game Object in this engine.

Attributed - An abstract subclass of Scope that allows Attributed subclasses to create named Datums that wrap their member variables. Ex: An Attributed subclass, Dragon, contains various member variables like _damage (float), or _NumAttacksPerTurn (integer). The Dragon class on construction will create Datums that wrap these member variables with string names via the Dragon class's static attributes. This system establishes two-way binding between these member variables and their associated Datums.

GameObject - A subclass of Attributed that contains transform variables, child GameObjects, child Actions, and a virtual Update method that will recursively call the Update method on each of its child GameObjects and Actions. 

Actions - Another Attributed subclass that defines modular units of behavior that can be added to GameObjects. These Actions model behavior by operating on Datums owned by the Action's parent GameObject. 

ParseCoordinator - This class uses a 3rd-party JSON parsing library - JsonCPP.Windows ver. 1.9.2 - to parse through JSON files or streams to instantiate GameObjects, and their respective Actions and initial state. Note that this system does require that the objects, actions, and their atttributes have already been defined in their respective C++ classes. Ex. A Dragon GameObject class must have already been defined before that class can be parsed and instantiated from JSON.
