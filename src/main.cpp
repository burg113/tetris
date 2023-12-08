#include <bits/stdc++.h>

#include <SDL.h>

/* Sets constants */
#define WIDTH 800
#define HEIGHT 600
#define DELAY 3000

using namespace std;

bool dedicatedServer = false;

void noGui(string s){

}

void debug(string s){

}

void server(string s){
    dedicatedServer = true;
}

struct CmdLineArg{
    int priority;
    function<void(string)> execute;

    CmdLineArg(): priority(-1) {}

    CmdLineArg(int priority, const function<void(string)> &execute) : priority(priority), execute(execute) {}

    bool operator<(const CmdLineArg &other) const {
        return priority < other.priority;
    }
};

// priority and function to call
map<string, CmdLineArg> commandLineArguments;

int32_t main(int argc, char* argv[]) {
    commandLineArguments["-debug"]=CmdLineArg(0, debug);
    commandLineArguments["-nogui"]=CmdLineArg(1, noGui);
    commandLineArguments["-server"]=CmdLineArg(10, server);


    priority_queue<pair<CmdLineArg,string>> args;

    for(int i = 1;i<argc;i++) {
        string s = argv[i];
        args.emplace(commandLineArguments[s],s);
    }


/* Initialises data */
    SDL_Window *window = NULL;

    /*
    * Initialises the SDL video subsystem (as well as the events subsystem).
    * Returns 0 on success or a negative error code on failure using SDL_GetError().
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    /* Creates a SDL window */
    window = SDL_CreateWindow("SDL Example", /* Title of the SDL window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                              WIDTH, /* Width of the window in pixels */
                              HEIGHT, /* Height of the window in pixels */
                              0); /* Additional flag(s) */

    /* Checks if window has been created; if not, exits program */
    if (window == NULL) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Delay(DELAY);

    /* Frees memory */
    SDL_DestroyWindow(window);

    /* Shuts down all SDL subsystems */
    SDL_Quit();

    return 0;
}
