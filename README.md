# Eternal Engine

# Abstract
This project consists of 3 parts. At the core lies the Eternal Engine, containing core features to create interactive applications. On top of that is the Doomed Editor, a basic game engine editor to easily use all these features at hand to apply them to a corresponding game level. Tthis editor makes the third and most important part, the game Project Quack possible. Project Quack is a 2D Jump 'n' Run in which you control a duck egg, helping to reach a safe place to hatch.

![Doomed-Editor](Documentation/Doomed-Editor.png) 
*Doomed Editor Layout and Look*

# How to setup the project correctly:
1. Clone the repository from the master branch <br>
  -> Use `git clone --recursive https://github.com/BrandnerKasper/Eternal`<br>
  -> We use `--recursive` to initalize and update the submodules needed
2. Open the `GenerateProject.bat` file in the project root directory <br>
  -> This should create the Micrsoft Visual Studio 2019 solution file `Eternal.sln`. It should look like the following.<br>

![Project-structure](Documentation/Projectstructure.png) 
*Project structure of Eternal*

  -> Open the `Eternal.sln` and press F5 to run the startup project `Doomed_Editor`! (You can easily switch to `ProjectQuack` if you feel more like playing the game immediately)
3. Have fun :)

# A little manual
This is a little manual to use the Doomed Editor and the game Project Quack:

# Doomed Editor

You start in an empty scene to add objects right click in the scene hierachy panel (see image below).
![addobj](Documentation/scenehierachy.png) 
*Add a object to the Scene by right clicking in the Scene Hierachy panel*

After adding a object cklick on it in the Scene Hierachy, this enables the properties panel.
![propPanel](Documentation/property-panel.png) 
*Enabled Properties Panel of a clicked game object*
Add a sprite component and immediately see your newly created object!
Add physics, scripts and audio and just play around!

You can test your level by clicking the play button above the scene viewport panel.
![play](Documentation/play.png) 
*Every Scene has two states an editing phase and a play phase!*

One last hint: You have to add a camera to one of your game objects and set it as a primary, else nothing gets rendered when pressing the play button.
Oh and don't forget to safe your level! And look into one of mine to see how they are built!

# Project Quack - The Game
Project Quack is a little 2D Jump 'n' Run in which you controll a duck egg with the typicall (W A S D) and (space) to jump.
![Titlescreen](Documentation/Titlescreen.png) 
*TitelScreen of Project Quack, press any key to start Level 1-1!*

Be carefull around water! This duck can't swim yet :P

![Level1-1](Documentation/level1-1.png) 
*Somewhere in Level 1-1. As you might imagine, every texture was drawn by me! Therefore I am sorry :D*

Finishing the level reveals a nice easter egg. Let's see if you can make it!
Good luck and have fun!
