# ball-shooter-game

This game involves two circles with same centre(different radius) each with 10 colored balls in it. The user has one colored ball which can be "fired". The objective of the game is to hit a ball in any one of the two rings which is of the same color as the firing ball. The firing ball's color changes randomly and the color of each ball in the ring is randomized at initialization. The stricken ball then is destroyed. The user has to likewise destroy all balls in the ring. He gets a certain number of strikeballs to do so. If the user hits a ball which is not of the same color, then the number of strikeballs remaining reduces by 5, and if he does not hit any ball, then the number of strikeballs remaining reduces by 1. If he hits it successfully, then 0 number of strikeballs is reduced.

The user presses the enter key to fire the ball and 'r' to reshuffle the entire ring along with the striking ball. This is done when no ball remaining in the ring is of the same color as the striking ball. Note that in reshuffling the number of balls remaining in the ring and their positions are not affected.

Note as to the code:
This game is written in C++ 20 with SDL 2.0 (without SDL_ttf or any such extension libraries)
The game is divided into many classes and objects. The main loop and init and exit functions are defined in the main.cpp file. The event handler class is housed in Events.h. The Ring,Block and CannonBall classes are defined in Ring.h. All other clases are defined in misc_classes.h

The code is commented wherever neccessary. Note that the commentation is not yet fully done as of now.

Thank you.
