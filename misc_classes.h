#ifndef _MATH_H
    #include <math.h>
#endif

#define PI 3.14157 //define pi
#define BLACK 0
#define BLUE 1
#define RED 2
#define GREEN 3


class Coordinates{//the class which acts as ordered pair of cartesian coordinates
public:
    double x,y;//actual base values

    Coordinates(double arg1,double arg2){//for those cases when we need in function dec of coordinates
        x=arg1;
        y=arg2;
        return;
    }

    Coordinates(){
        return;
    }
};

class MyRenderer{//my version of an integrated renderer
public:
    SDL_Renderer *baseren;//base class;

    void create(SDL_Window *window){//create the window
        baseren=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//create renderer for window given in arg
        //-1 stands for nothing much, just defining the function to use the first one it wants to, then we define that we want to use the
        //renderer in hardware accelerated and vsync modes
        return;
    }

    void destroy(){//deallocate some memory when not needed
        SDL_DestroyRenderer(baseren);//destroy the base renderer
    }


};

class MyWindow{
public:
    SDL_Window *basewin;//base sdl window
    MyRenderer renderer;//the renderer

    void create(std::string name,Coordinates winSize){
        basewin=SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,winSize.x,winSize.y,SDL_WINDOW_SHOWN);
        //create window with title name, at any position and of size sizex*sizey which is shown.
        renderer.create(basewin);//init the renderer with window basewin
        return;
    }

    void terminate(){
        SDL_DestroyWindow(basewin);//destroy the basewindow
        renderer.destroy();//destroy the renderer
        return;
    }

    void cls(){//clear the screen
        SDL_SetRenderDrawColor(renderer.baseren,0,0,0,255);//set the color with which the renderer will draw, does not apply to drawing textures
        //here the chosen color is black
        SDL_RenderClear(renderer.baseren);//fill the screen with the chosen color
        return;
    }

    void present(){//present or display the window
        SDL_RenderPresent(renderer.baseren);//base function call
        return;
    }

};

class MyRect{//my class as a substitute for SDL_Rect, do not refer to this by pointers in function calls
public:
    SDL_Rect baserect;//uses as a base SDL_Rect

    MyRect(Coordinates location,uint32_t h,uint32_t w){//constructor call, one of the main reasons to use this over SDL_Rect, SDL_Rect has no constructor
        //call, thus it has to be manually initialized.
        baserect.h=h;//just setting the variables, as I said, there is nothing much to this class
        baserect.w=w;
        baserect.x=location.x;
        baserect.y=location.y;
        return ;
    }


};

class MyTexture{//my texture class, incorporates many useful functions into the class
public:
    SDL_Texture *basetex=NULL;//base texture pointer

    void load(std::string relpath,MyRenderer renderer){//args are the path to img and the renderer. note that
        //relpath assumes that the path begins from the current directory as root
        SDL_Surface *basesurface=NULL;//set the base surface as provided by SDL to null, there was no specific reason to do so, just a whim.
        basesurface=SDL_LoadBMP(relpath.c_str());//load the surface with a image. LoadBMP accepts as argument the relpath as a char string
        basetex=SDL_CreateTextureFromSurface(renderer.baseren,basesurface);//create a texture from the surface. args: the SDL_Renderer and the surface
        // whose image we have to take
        SDL_FreeSurface(basesurface);//delete the surface as we no longer need it
        return;
    }

    //this method displays the texture on the screen
    //it is overloaded three times for three different usages

    //all rects are MyRects, defined in this file

    void display(MyRenderer renderer,MyRect source,MyRect destination){//version for absolute specifics
        SDL_RenderCopy(renderer.baseren,basetex,&source.baserect,&destination.baserect);//provide to func renderer,base texture,
        //the source rect and dest rect
        //the source rect defines which area of the texture to display and dest rect specify where on the screen to display
        return;
    }

    void display(MyRenderer renderer){//when you just want to fill the screen with image
        SDL_RenderCopy(renderer.baseren,basetex,NULL,NULL);
    }

    void display(MyRenderer renderer,MyRect destination){//just stick the entire image at a specific area
        SDL_RenderCopy(renderer.baseren,basetex,NULL,&destination.baserect);
    }

};

class TimeKeeper{//I use this class to get the delta values
public:

    double delta=0.0167;//the delta value is stored here
    uint32_t last_time=0;//what was the last time we update()-ed, this is also (roughly) equal to the time of the last frame

    void update(){
        delta=(SDL_GetTicks()-last_time)/1000.0f;//both SDL_GetTicks and last_time store time in ms, we divide by 1000 to get s
        //SDL_GetTicks returns the number of ms that have passed since beginning of program.
        last_time=SDL_GetTicks();//set the time of the last frame to this one, so that we can use this later
        return;
    }


};

class CollisionData{
    //this class is by NO MEANS a comprehensive one!! this only serves the purpose of this particular one, required by ball in collision method
public:

    bool is_collision=false;//have we had a collision?
    bool is_collider_same_col=false;//will this collision count as a score?
};

class ball{
public:
    MyTexture texture;
    MyRenderer renderer;
    double r=12.5;//the radius
    Coordinates location,centre;//coords of location and centre of rotation DO NOT BE CONFUSED THAT CENTRE IS THE PHYSICAL CENTRE!! IT IS NOT!!
    //IT IS THE CENTRE AROUND WHICH THE BALL ROTATES!!
    int color=BLACK;//we set the deafult color to black, note that what we set here does not matter as it is going to be randomized
    //later in class Ring

    void loadrenderer(MyRenderer arg1){
        renderer=arg1;//set the renderer
    }

    void loadtex(std::string path){
        texture.load(path,renderer);//load the texture
        return;
    }

    void rotation(double angle){//rotate the ball around the centre
        //angle*=(double)180/PI;
        float s=sin(angle);
        float c=cos(angle);
        double xnew,ynew;//the new coords of ball

        location.x-=centre.x;//make the centre of rotation the origin
        location.y-=centre.y;

        xnew=location.x*c-location.y*s;//rotation matrix application
        ynew=location.x*s + location.y*c;

        location.x=xnew + centre.x;//again make the original origin the origin
        location.y=ynew+centre.y;

        return;

    }

    void display(){
        texture.display(renderer,MyRect(location,2*r,2*r));//display the ball

    }

    void randomize(){
        int randval;//value of rand() stored here
        randval=rand();//get a random number
        if(randval%3==0){//probability = 1/3
            loadtex("redball.bmp");//get a red ball
            color=RED;//set color property for future reference
            //same for all below
        }
        else if(randval%3==1){//probability=1/3
            loadtex("blueball.bmp");
            color=BLUE;
        }
        else{
            loadtex("greenball.bmp");//probability=1/3
            color=GREEN;
        }
        //we exit with a colored ball which is randomly chosen
    }

    CollisionData collision(ball arg){//has this ball collided with another ball ? if it has
        //then send back relevant data
        double d,dxsq,dysq;//some temporary vars
        //d is distance between two balls
        //dxsq is the square of the x distance
        //dysq is the square of the y distance

        CollisionData data;//make a new collision data object , we return this object

        dxsq=pow(location.x-arg.location.x,2);//complex mathematical expression, check c++ specs for data about this
        dysq=pow(location.y-arg.location.y,2);
        d=std::sqrt(dxsq+dysq);
        //so basically we use distance formula for cartesian coordinates:
        //d=sqrt(sq(x1-x2)+sq(y1-y2))


        if(d<r){//are we less than one radius apart?
            data.is_collision=true;//yes, we have collided. set the data so
            if(arg.color==color)//is the collision valid? collisions get points only if they are of same color, else we penalize it
                data.is_collider_same_col=true;//yes colliders are of same color? set data so
        }
        return data;
    }

};

