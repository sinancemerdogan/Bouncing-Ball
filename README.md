# bouncing-ball
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
  <title>Assignment 1</title>
  <meta content="Microsoft FrontPage 6.0" name="GENERATOR">
  <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
</head>
<body bgcolor="#ffffff">
  <h3 style="font-family: Arial;">
    <big></big>
    <center>
      <big><b><font size="4"><big>COMP 410/510, Computer Graphics, Spring 2023</big></font></b></big>
    </center>
    <big></big>
  </h3>
  <h3 style="text-align: center;">
    <img style="width: 376px; height: 177px;" alt="" src="teaser.jpg">
  </h3>
  <h3>
    <center>
      <b><font size="4" color="#000000"><br></font></b>
    </center>
  </h3>
  <h3 style="font-family: Arial;">
    <font color="#ff0000">Due Date: Sunday, April 2, 2023</font>
  </h3>
  <h3 style="font-family: Arial;">Problem Description and Learning Objectives:</h3>
  <span style="font-family: Arial;">
    In this assignment, you will design and implement a basic graphics application that animates a ball bouncing horizontally on the screen (as shown above). This will be an interactive program that allows the user to select an object to draw (from a choice of two objects), to choose various drawing attributes for the object (such as color and polygon mode). Each time the user selects one of the drawing options, the image on the screen will be redrawn with the new choice. To develop this program you will need to learn how to use both <span style="font-weight: bold; font-family: Arial;">shader-based OpenGL</span> and <span style="font-family: Arial;">GLFW</span> 3D graphics libraries. The <span style="font-family: Arial;"><span style="font-weight: bold; font-family: Arial;">GLFW</span><span style="font-family: Arial;"></span> library includes functions for implementing event-driven input and display handling operations. This assignment will be an introduction to event-driven graphics programming. Your program must be developed using <span style="font-weight: bold; font-family: Arial;">shader-based</span> OpenGL and C/C++.<br>
    <br>
    <ul></ul>
    <h3 style="font-family: Arial;">Problem Specification</h3>
    <p style="font-family: Arial;">Your program will simulate and display a bouncing ball (either cubical or spherical) under gravitational force. The ball will initially be placed on the top leftmost of the display window and then will start its fall (accelerated with gravity). The initial (horizontal) velocity will be a parameter to set. The ball will move rightwards with proper acceleration until it hits the bottom of the window, and bounce back slowing down gradually with each hit until it goes out of the window or its velocity becomes zero. Bouncing will continue as such. You may reduce the velocity by a fixed factor after each bouncing. During execution, the user may change the object type, color, and drawing mode.</p>
    <span style="font-family: Arial;">You can refer to the following page for physics of a bouncing ball under gravity (Note that in this assignment we will be ignoring the deformation of the bouncing ball): </span><a style="font-family: Arial;" href="https://interestingengineering.com/what-are-the-physics-behind-bouncing-balls">bouncing ball</a><br>
    <p style="font-family: Arial;">Your program must handle user input from the keyboard and mouse, and set the drawing modes as specified below:</p>
    <p style="font-family: Arial;">Object type and drawing mode (wireframe or solid) must be set by using mouse button callback:<br></p>
    <ul style="font-family: Arial;">
      <li><b>Object type</b> -- set the current object to be drawn; switch between the following two choices, whenever right button is pressed:
        <ul>
          <li>cube</li>
          <li>sphere</li>
        </ul>
      </li>
      <li><b>Drawing mode</b> -- set how the triangles are drawn; switch between the following two choices, whenever left button is pressed:</li>
      <ul>
        <li>wireframe (i.e., as lines)</li>
        <li>solid mode</li>
      </ul>
    </ul>
    <span style="font-family: Arial;"> Object initialization, color setting, termination, and help functionalities should be controlled through keyboard callback as specified below:</span><br

