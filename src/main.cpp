#include <bits/stdc++.h>

#include <SDL.h>
#include "game/io/Window.h"
#include "game/Tetris.h"

#include "networking/Networking.h"

/* Sets constants */
#define WIDTH 800
#define HEIGHT 1000
#define DELAY 3000

using namespace std;

bool dedicatedServer = false;
bool openWindow = true;
short debugLevel = -1;

// save get int
int getInt(const string &s) {
    if (s.empty() ||
        std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) != s.end())
        return -1;
    return stoi(s);
}

void noGui(const string &s) {
    openWindow = false;
}

void debug(const string &s) {
    debugLevel = 0;

    debugLevel = (short) getInt(s);
    debugLevel = max(debugLevel, (short) 0);

    cerr << "debug level set to " << debugLevel << endl;
}

void server(const string &s) {
    dedicatedServer = true;
}

struct CmdLineArg {
    int priority;
    function<void(const string&)> func;
    string payload;

    CmdLineArg() : priority(-1) {}

    CmdLineArg(int priority, const function<void(const string&)> &execute) : priority(priority), func(execute) {}

    bool operator<(const CmdLineArg &other) const {
        return priority < other.priority;
    }

    void execute() const {
        func(payload);
    }
};

// priority and function to call
map<string, CmdLineArg> commandLineArguments;

int32_t main(int argc, char *argv[]) {
    //testNetworking();

    commandLineArguments["-debug"] = CmdLineArg(0, debug);
    commandLineArguments["-nogui"] = CmdLineArg(1, noGui);
    commandLineArguments["-server"] = CmdLineArg(10, server);

    priority_queue<CmdLineArg> args;

    for (int i = 1; i < argc; i++) {
        string s = argv[i];
        if (commandLineArguments.count(s) == 1) {
            CmdLineArg cmdLineArg = commandLineArguments[s];
            if (i + 1 < argc && argv[i + 1][0] != '-') cmdLineArg.payload = argv[++i];
            args.emplace(cmdLineArg);
        } else {
            cerr << "Invalid argument \"" << s << "\" ignored" << endl;
        }
    }

    while (!args.empty()) {
        CmdLineArg cmdLineArg = args.top();
        args.pop();

        cmdLineArg.execute();
    }

    if (dedicatedServer) {
        if (openWindow) {
            Window window(WIDTH,HEIGHT);
        }
        // todo: implement server


    } else {
        if (!openWindow) {
            cerr << "cannot run game without window! exiting" << endl;
            return 1;
        }
        if (openWindow) {
            Window window(WIDTH,HEIGHT);
            if (!window.valid) {
                cerr << "failed to create window! exiting" << endl;
                return 1;
            }
            Tetris tetris(&window);
            tetris.play();
        }
    }


    return 0;
}
