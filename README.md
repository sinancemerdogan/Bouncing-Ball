# Bouncing Ball Animation

![Bouncing Ball](teaser.jpg)

**Table of Contents**
- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Implementation Details](#implementation-details)

## Introduction

Welcome to the Bouncing Ball Animation project! This program is a basic graphics application that creates an interactive animation of a ball bouncing horizontally on the screen. You'll have the opportunity to select the object to draw, choose various drawing attributes, and control the animation.

![Bouncing Ball Gif](bouncing-ball-animation.gif)

## Features

- **Interactive Animation**: A bouncing ball is simulated and displayed on the screen.
- **Object Selection**: Choose between two object types: cube and sphere (bonus: bunny model).
- **Drawing Modes**: Switch between wireframe and solid rendering modes.
- **User Controls**: Use keyboard and mouse input to control the animation and drawing options.
- **Shader-Based OpenGL**: Utilize shader-based OpenGL for rendering.
- **Resizable Window**: The program handles window resizing events properly.

## Getting Started

1. Clone this repository to your local machine.
2. Open the project using your preferred development environment.
3. Build and run the application.
4. Explore the bouncing ball animation and experiment with different drawing options.

## Usage

- Use the **Right Mouse Button** to switch between object types (cube and sphere).
- Use the **Left Mouse Button** to switch between drawing modes (wireframe and solid).
- Use the **i** key to initialize the ball's position.
- Use the **c** key to toggle between two colors used for drawing lines or triangles.
- Use the **h** key for help to view explanations of input controls in the command line.
- Use the **q** key to quit and exit the program.

## Implementation Details

- The animation is driven by physics principles, including gravity and bouncing.
- Shader-based OpenGL is used for rendering, and GLFW handles user input and window management.
- The program uses vertex arrays and glPolygonMode() to switch between wireframe and solid rendering.
- Projection and transformations are implemented in the vertex shader.
- Proper event handling and callback functions ensure smooth user interaction.
