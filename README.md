<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h2 align="center">SCARA-Robot, Blue_Robot_2022</h3>
  <a href="https://www.massey.ac.nz/about/colleges-schools-and-institutes/college-of-sciences/school-of-food-and-advanced-technology/">
    <img src="http://sites.massey.ac.nz/scran/wp-content/uploads/sites/102/2022/02/Massey-Logo.png" alt="Logo" width="554" height="137">
  </a>
  <p align="center">
    Source code of a Scara-Robot developed for the School of Food and Advanced Technology at Massey University.
    <br />
    <a href="https://github.com/Thibault-GILLARD/Blue_Robot_2022"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/Examples_Tests">View Demo</a>
    ·
    <a href="https://github.com/Thibault-GILLARD/Blue_Robot_2022/issues">Report Bug</a>
    ·
    <a href="https://github.com/Thibault-GILLARD/Blue_Robot_2022/issues">Request Feature</a>
  </p>
  <a href="https://en.wikipedia.org/wiki/SCARA">
    <img src="https://electricalworkbook.com/wp-content/uploads/2021/07/SCARA-Robot.png" alt="Logo" width="188" height="198">
  </a>
</div>

The SCARA robot, which stands for Selective Compliance Assembly Robot Arm, is a type of articulated robot arm that is commonly used in industrial and manufacturing settings. 

<!-- BADGE -->
</p>
<p align="center">
  <img alt="GitHub All Releases" src="https://img.shields.io/github/downloads/Thibault-GILLARD/Blue_Robot_2022/total?logo=GitHub">
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Thibault-GILLARD/Blue_Robot_2022?logo=GitHub&style">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Thibault-GILLARD/Blue_Robot_2022?logo=GitHub">
</p>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#code-explanations-microscope">Code explanations</a></li>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#file-organisation">File organisation</a></li>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#arduino-board">Arduino Board</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage-examples">Usage Examples</a>
      <ul>
        <li><a href="#basic-test">Basic Test:</a></li>
        <li><a href="#simple-applications">Simple applications:</a></li>
          <ul>
            <li><a href="#gui-to-control-the-robot-desktop_computer">GUI To Control The Robot</a></li>
            <li><a href="#draw-your-design-crayon">Draw your design</a></li>
          </ul>
      </ul>
    </li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project

This is the second version of the source code developed for the robot.

#### This one fulfils two main needs: 
- Switch to a more accessible, manipulable, and documented code for students/users :+1: 
- Easy and inexpensive replacement of the robot's microcontroller :+1:

This new version of the code allows a transition from a custom board controlled by an 
[ATMEL AT32UC3B1256 32bit](https://www.microchip.com/en-us/product/AT32UC3B1256) 
to a simple arduino board (ideally, an [arduino DUE board](https://store-usa.arduino.cc/products/arduino-due)).

On the hardware side, this allows the use of cheaper and much easier 
to obtain boards if they have to be replaced.

On the other hand, working on an arduino allows working in an environment 
that can be considered as simple and much more documented (common problem 
solution easily available) compare to the more technical work required 
when operating on the AVR UC3 Software Framework. 
Additionally, the arduino libraries are widely used and regularly improved. 

The modification of the new code can be done on simple tools such as 
the [arduino IDE](https://www.arduino.cc/en/software) or [platformIO on VScode](https://platformio.org/install/ide?install=vscode) (recommended).

<!-- Code explanations -->
## Code explanations :microscope:
Each part of the code is strongly explained and commented to allow a quick understanding of it. 
* Ex :	  
  ```text 
  /*
  void speed_adapt(int max_mvmnt, int to_adapte_1, int to_adapte_2):
  1. First, we calculate the ratio between the axis to adapt and the axis of maximum movement (max_mvmnt).
  2. Next, we calculate the maximum and acceleration speeds for the axis of maximum movement (max_mvmnt),
   and for the other two axes (to_adapte_1 and to_adapte_2).
  3. We multiply the speed and acceleration ratios by the ratio calculated in step 1, and we set the new
   speed and acceleration for the two axes to adapt.
  */

  ```
Also, several **flowcharts** have been created to describe the different functions of the code. 
The .drawio files are also available, so you can modify or adapt them for your project.

:eyes:  [Blue_Robot_2022/Flow charts/](https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/Flow%20charts)
<div align="center">
  <a href="https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/Flow%20charts">
    <img src="https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/Flow%20charts/read_and_extract().png" width="197" height="411">
  </a>
</div>

<!-- GETTING STARTED -->
## Getting Started

### File organisation

The folder can be used directly on **platformIO**.
Here are the important files of the source code:

The implementation file is located in the **"src"** folder:
- [src](https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/src)

  -[main_Blue_Robot.cpp](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/src/main_Blue_Robot.cpp)   

The Header files are located in the **"include"** folder:
- [include](https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/include)

  -[main_BR.hpp](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/include/main_BR.hpp)
  
  -[Steppers_BR.hpp](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/include/Steppers_BR.hpp)
  
  -[Pin_Define_BR.hpp](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/include/Pin_Define_BR.hpp)
  
  -[Robot_Define_BR.hpp](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/include/Robot_Define_BR.hpp)
  
  -[USB_device_R.h](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/include/USB_device_R.h), (normally not used)



### Prerequisites

You just need to install the library called [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/).
  ```cpp
  #include <AccelStepper.h>
  ```
AccelStepper is a library included in the standard Arduino IDE that allows the control of stepper motors and motor drivers (with acceleration and deceleration).

To better understand how it works you can refer to these [equations](https://www.embedded.com/generate-stepper-motor-speed-profiles-in-real-time/) on which the calculations are based.:gear:

### Arduino Board
Originally the project was programmed for an arduino DUE to take advantage of the Native Port (faster) directly connected to the Atmel SAM3X8.

With simple modifications, the program can be adapted to hundreds of Arduino-compatible boards that use a USB-to-serial converter:
This only requires that the 'SerialUSB' object be replaced with the 'Serial' object. So replace all 'SerialUSB' with 'Serial'.

-Ex :
  ```cpp
  SerialUSB.begin(115200);
  ```
  becomes
  ```cpp
  Serial.begin(115200);
  ```

<!-- USAGE EXAMPLES -->
## Usage Examples

### Basic Test

You can start the first serial communication using the serial monitor of the arduino IDE, with simple commands like:

```text
PING
```
Pings the controller to test communications. You should receive a reply 'PONG'.
Then you can try the homing
```text
HOME
```
And after the homing you will be able to send MOVE command...

![com_arduino](https://user-images.githubusercontent.com/117235512/202085686-4673ef6e-fd74-4ce8-b883-b64c55b404b6.gif)

Software such as YAT or Serial Port Monitor can also be used for initial testing.
Just configure the COM to match the table below :

<div align="center">
  <a href="">
    <img src="https://user-images.githubusercontent.com/117235512/202087771-dd00b3fe-19bb-4246-998c-9c36811773ca.png">
  </a>
</div>

![GITHUB 2](https://user-images.githubusercontent.com/117235512/202085701-7ea6a69b-b2e2-489d-bb4e-fa0d189a10ca.gif)

Here is also a small functional test on **python** using the main commands :
  ```python
  import serial

  SerialObj = serial.Serial('COM14')
  SerialObj.baudrate = 115200
  SerialObj.bytesize = 8
  SerialObj.parity = serial.PARITY_NONE
  SerialObj.stopbits = 1

  def print_out(message):
      msg = message
      mdg_str_encode = str(msg).encode()
      SerialObj.write(mdg_str_encode)
      RecivedStr = SerialObj.readline()
      print(RecivedStr)

  command = ['PING','HOME','MOVE,-300,-200,0','MOVE,-300,0,0','AIR,D,2000','HOME','AIR,U']

  for element in command:
      print_out(element)
  ```
You can find the same test in different languages in [Examples_Tests/Small_Test.txt](https://github.com/Thibault-GILLARD/Blue_Robot_2022/blob/develop/Examples_Tests/Small_Test.txt).

### Simple applications

#### GUI To Control The Robot :desktop_computer:

A graphical user interface has been created in python to get to grips with the robot and quickly understand the different functions.

TheGUI is based on the kivy library which you need to add to your environment:
* with pip
  ```cmd
  pip install Kivy
  ```
* with conda
  ```python
  conda install -c conda-forge kivy
  ```

https://user-images.githubusercontent.com/117235512/202077130-3262997f-9de9-42b1-afd4-c415903a2e67.mp4


#### Draw your design :crayon:

Another very simple and quick to code application is presented in [Examples_Tests](https://github.com/Thibault-GILLARD/Blue_Robot_2022/tree/develop/Examples_Tests)

This application allows the robot to draw what you have drawn on [GEOGEBRA](https://www.geogebra.org/classic).

Here's a tutorial:

https://user-images.githubusercontent.com/117235512/202069179-de6312d3-ee59-4c6c-9dbe-a9ff7f8828bb.mp4







