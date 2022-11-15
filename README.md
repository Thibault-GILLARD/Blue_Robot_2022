<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://www.massey.ac.nz/about/colleges-schools-and-institutes/college-of-sciences/school-of-food-and-advanced-technology/">
    <img src="http://sites.massey.ac.nz/scran/wp-content/uploads/sites/102/2022/02/Massey-Logo.png" alt="Logo">
  </a>

  <h2 align="center">SCARA-Robot, Blue_Robot_2022</h3>

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
    <img src="https://electricalworkbook.com/wp-content/uploads/2021/07/SCARA-Robot.png" alt="Logo" width="251" height="265">
  </a>
</div>

This is the second version of the source code developed for the robot.

#### This one fulfils two main needs: :
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
the arduino ide or platformIO on VScode (recommended).

The SCARA robot, which stands for Selective Compliance Assembly Robot Arm, is a type of articulated robot arm that is commonly used in industrial and manufacturing settings. 

https://user-images.githubusercontent.com/117235512/201827715-498f13ce-a90b-482b-a64e-b92b25c59737.mp4



