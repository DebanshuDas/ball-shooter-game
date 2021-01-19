#ifndef MISC_CLASSES_H
    #include "misc_classes.h"
#endif // MISC_CLASSES_H

#define NUM_BALLS 20//define the max number of balls at any time
#define DEG1 0.0174533//define 1 degree in radians

#ifndef _INC_STDLIB
    #include <stdlib.h>
#endif // _INC_STDLIB

#ifndef _TIME_H_
    #include <time.h>
#endif // _TIME_H_

#define CLOCKWISE -1//defining directions as neg or pos
#define ANTICLOCKWISE 1



extern TimeKeeper GTime;//we get the global timekeeper here


class Ring{
public:

    ball basering[NUM_BALLS];//array of balls
    int balls_hit=0;//how many balls have we hit?
    MyRenderer renderer;//the renderer to render to
    int radius=100;//radius of rotation
    int ballsremaining=NUM_BALLS;//number of balls not hit
    Coordinates centre=Coordinates(350,150);//coordinates of centre of rotation


    Ring(MyRenderer arg1){//constructor with renderer as arg
        renderer=arg1;
        srand(time(0));//randomize the rand() sequence with time as seed
        //ensures different sequences with time
        return;
    }

    void init(){//initialize the ring
        int i=0,j=0;//counters to nill
        while(i!=NUM_BALLS){//while we have not yet looped through all balls
            basering[i].centre=centre;//set all balls' centre to the centre of rotation
            basering[i].loadrenderer(renderer);//load renderer to all
            basering[i].randomize();//randomize color of balls

            if(i%(NUM_BALLS/2)==0&&i!=0){//after half iterations
                radius+=50;//increase radius to include second circle
                j=0;//j represents the index of a ball relative to the circle
                //for example, as the second circle begins from 10, for i=11 j=1
            }

            basering[i].location.x=centre.x;//the ball will be directly over the centre of rotation now
            basering[i].location.y=centre.y-radius;//ball will be radius pixels above centre or rotation

            basering[i].rotation(j*DEG1*36);//set each ball 36 degrees apart from the previous one
            i++;
            j++;//increment all counters
        }
        return;

    }

    void reshuffle(){
        int i=0;
        while(i!=NUM_BALLS){
            if(basering[i].color!=BLACK)
                basering[i].randomize();
            i++;
        }
    }

    void display(){//display the balls
        int i=0;
        while(i!=NUM_BALLS){
            basering[i].display();//calling display function of each ball
            i++;
        }
        return;
    }


    void rotation(){
        int i=0;
        int dir=CLOCKWISE;//default direction is clockwise


        while(i!=NUM_BALLS){
            if(i>NUM_BALLS/2-1)//after half iterations(ie new ring) switch directions
                dir=ANTICLOCKWISE;
            basering[i].rotation((double)dir*GTime.delta*DEG1*75);//rotate each ball 75 degrees per second
            //delta is framerate, thus every frame rotate by speed(per second)/framerate( number per sec)

            i++;
        }
        return;
    }

    CollisionData collision_handling(ball arg){//function to detect collisions and get relevant data
        int i=0;
        CollisionData colldata;

        colldata.is_collider_same_col=false;
        colldata.is_collision=false;

        while(i!=NUM_BALLS){
            if(basering[i].texture.basetex==NULL){//the texture being null means that the ball is invisible, ie if you hit that ball you get no points
                    //since for the purpose of this function we are only looking for "valid" collisions, we reject these collisions
                    //thus no need of checking
                    //saves time :)
                i++;
                continue;//just skip this ball
            }
            if(basering[i].collision(arg).is_collision){//now, only visible balls reached here. question is: is there a collision between this ball and
                //the argument ball?
                colldata.is_collision=true;
                if(basering[i].collision(arg).is_collider_same_col){//now, are they of same colour? only then we have a valid collision
                    basering[i].texture.basetex=NULL;//if yes, then simply render this ball invisible
                    //future cannons will not collide with this ball
                    //also the user will no longer see the ball, getting the "feeling" that the ball is destroyed
                    //(which it is for all practical purposes)
                    colldata.is_collider_same_col=true;
                    ballsremaining--;//then, we decrease the number of (visible) balls remaining in the ring
                    balls_hit++;//one more ball is hit
                }
            }
            i++;
        }
        return colldata;//return colldata
    }


};
/*
Now, many people will see that this whole chunk can be easily ( and also advantageously) removed, so I have come to defend whatever remaining
respect i have in their eyes. You see, i have tried over two weeks to connect the damn ttf extension, but the bloody thing never worked.
exasperated, desperate and irritated, i tried console. sdl wont let me do even that!! so what the heck will i do!!
then i thought of this approach and realised that it will also look classy. so, here it is!

*/
class Block{
public:

    ball baseblock[NUM_BALLS];//the array of balls
    int howmanytodisplay=NUM_BALLS;//how many balls should the block display?
    MyRenderer renderer;//the renderer
    Coordinates starting_point=Coordinates(650,0);//where will the top left corner fall

    Block(MyRenderer arg){//constructor arg
        renderer=arg;//set the renderer
        return;
    }

    void init(){
        int i=0,j=0;//loop vars to nill
        while(i!=NUM_BALLS){//while we have not finished all NUM_BALLS balls
            baseblock[i].loadrenderer(renderer);//load the renderer into the ball
            baseblock[i].loadtex("whiteball.bmp");//load the texture

            if(i%5==0){
                j=0;
                starting_point.y+=50;
            }

            baseblock[i].location=starting_point;
            baseblock[i].location.x+=j*30;
            i++;
            j++;
        }
    }

    void display(){
        int i=0;
        while(i!=howmanytodisplay){
            baseblock[i].display();
            i++;
        }
    }

};

class CannonBall{
public:

    ball base;
    Coordinates velocity=Coordinates(0,0);

    CannonBall(MyRenderer renderer){
        base.loadrenderer(renderer);
        base.randomize();
        base.location=Coordinates(350,500);
        return;
    }

    void fire(){
        velocity=Coordinates(0,-25);
        return;
    }

    void update(){
        base.location.x+=velocity.x;
        base.location.y+=velocity.y;

        if(base.location.y<0){
            reset();
        }
        base.display();

    }

    void reset(){
        base.location.y=500;
        velocity=Coordinates(0,0);
        base.randomize();
    }
};
