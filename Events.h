#ifndef ENTER
    #define ENTER 0//enter key press will be given number 0
#endif

#ifndef RANDOMIZE
    #define RANDOMIZE 1
#endif // RANDOMIZE


class EventHandler{//this is the event handler class
public:
    SDL_Event event;//base event class

    int update(){//update the class, get new event messages
        return SDL_PollEvent(&event);//polling event
    }

    int is_quit(){//did the user want to quit?
        if(event.type==SDL_QUIT)//is event QUIT
            return 1;//if yes, return true
        return 0;//if not, return false
    }

    int is_keypress(){//did the user press a key?note: neccessary to do as we need to check if keypress
        //before trying to find out which key was pressed
        if(event.type==SDL_KEYDOWN)//base function call
            return 1;
        return 0;
    }

    int which_key(){//find out which key was pressed, use only if is_keypress() returns true
        switch(event.key.keysym.sym){//switch, check for required values of keys
        case SDLK_RETURN:
            return ENTER;//return char code
            break;
        case SDLK_r:
            return RANDOMIZE;
            break;
        }
        return -1;//no key identifiable? return -1
    }


};
