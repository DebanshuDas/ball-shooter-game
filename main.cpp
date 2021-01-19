#include <string>
#include <iostream>
#include <SDL.h>
//#include <SDL_ttf.h>//something is wrong here!!!linking maybe

#ifndef EVENTS_H
    #define EVENTS_H
    #include "Events.h"
#endif // EVENTS_H

#ifndef MISC_CLASSES_H
    #define MISC_CLASSES_H
    #include "misc_classes.h"
#endif // MISC_CLASSES_H

#define DEG90 1.5708

#ifndef RING_H
    #define RING_H
    #include "Ring.h"
#endif // RING_H

#ifndef _WINDOWS_
    #include <windows.h>
#endif


/*note to the fool who thought this piece of code worth reading
If you open this file in the Windows 10 desktop pc named DD_COOL1, then i must warn you that any attempt to alter
this code may result in your premature exit from the mortal plane. If not, then this is licensed under CC
*/

/*
This program is written by Debanshu Das
*/

void initialize();//function to initialize app
void terminate();//function to terminate app
void main_loop();//the main game loop function

MyWindow GWindow;//the global window, the main window
MyTexture background;//the background of GWindow
TimeKeeper GTime;//the global timekeeper, we get delta from here

int main(int argc,char* args[]){//default config for SDL
    initialize();//calling functions in order
    main_loop();
    terminate();
    return 0;//return
}

void initialize(){
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);//initialize SDL systems of video and events, audio not yet
    //TTF_Init();
    GWindow.create("Test",Coordinates(900,600));//create the window; args title and height and width; see class def
}

void terminate(){
    GWindow.terminate();//terminate window
    SDL_Quit();//quit all SDL sections
}

void main_loop(){
    EventHandler my_events;//the event handler
    background.load("background.bmp",GWindow.renderer);//load image "background.bmp" into texture; args filename and renderer; see def
    bool quit=false;//set this boolean quit to false; when this is true then we quit
    CannonBall shot(GWindow.renderer);

    Block BallCount(GWindow.renderer);
    BallCount.init();

    Ring MyRing(GWindow.renderer);
    MyRing.init();

    while(!quit){//if quit is true then we stop looping
        while(my_events.update()){//update event handler to get new events
            if(my_events.is_quit())//if quit event
                quit=true;//set to true
            if(my_events.is_keypress()){
                if(my_events.which_key()==ENTER){
                    shot.fire();
                    BallCount.howmanytodisplay--;
                }
                if(my_events.which_key()==RANDOMIZE){
                    MyRing.reshuffle();
                    shot.base.randomize();
                }
            }
        }
        GWindow.cls();//clear the screen
        GTime.update();//update the time
        MyRing.rotation();

        MyRing.display();



        shot.update();

        if(MyRing.balls_hit>=NUM_BALLS)//have we hit all balls
            break;//then quit
        //will complement this if cond with more

        CollisionData colldata=MyRing.collision_handling(shot.base);
        if(colldata.is_collision){
            shot.reset();
            if(!colldata.is_collider_same_col)
                BallCount.howmanytodisplay-=5;
            else
                BallCount.howmanytodisplay++;

        }

        if(BallCount.howmanytodisplay<=0)
            quit=true;

        if(!quit)
            BallCount.display();

        GWindow.present();//present the screen

        if(quit){
            if(MyRing.ballsremaining>0&&BallCount.howmanytodisplay<=0)
                MessageBox(NULL,"Sorry! You failed to hit all balls. Better luck next time!","You lost",MB_OK);
            else if(!BallCount.howmanytodisplay>0)
                MessageBox(NULL,"Good Work! You successfully hit all balls. You have won!","Congratulations!",MB_OK);

        }

    }
    return;

}

