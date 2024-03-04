#include <bits/stdc++.h>

#include <SDL.h>
#include "application/io/output/sdl/Window.h"
#include "application/client/Tetris.h"
#include "application/io/input/SDLInputAdapter.h"

#include "../examples/Networking.h"
#include "networking/BinarySerialize.h"
#include "networking/ServerHelper.h"
#include "application/server/MirrorServer.h"
#include "application/server/MultiplayerServer.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

/* Sets constants */
#define WIDTH 800
#define HEIGHT 1000
#define DELAY 3000

using namespace std;

bool dedicatedServer = false;
bool openWindow = true;
short debugLevel = -1;
string host, service;

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

void debug(const string &s) { // todo: debug level is currently unused
    debugLevel = 0;

    debugLevel = (short) getInt(s);
    debugLevel = max(debugLevel, (short) 0);

    SPDLOG_INFO("debug level set to {}", debugLevel);
}

void server(const string &s) {
    dedicatedServer = true;
}

void hostOption(const string &s){
    host = s;
}

void serviceOption(const string &s){
    service = s;
}

struct CmdLineArg {
    int priority;
    function<void(const string &)> func;
    string payload;

    CmdLineArg() : priority(-1) {}

    CmdLineArg(int priority, const function<void(const string &)> &execute) : priority(priority), func(execute) {}

    bool operator<(const CmdLineArg &other) const {
        return priority < other.priority;
    }

    void execute() const {
        func(payload);
    }
};

// priority and function to call
map<string, CmdLineArg> commandLineArguments;

void initLogger(){
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());

    auto time = chrono::high_resolution_clock::now().time_since_epoch();
    string logFile = "logs/log-" + to_string(time.count() / 10000000) + ".txt";
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(logFile));
    auto combined_logger = std::make_shared<spdlog::logger>("name", begin(sinks), end(sinks));
    //register it if you need to access it globally
    spdlog::register_logger(combined_logger);

    spdlog::set_default_logger(combined_logger);
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_pattern("[%T] [%^%l%$] [%s %#] %v");
    spdlog::set_level(spdlog::level::trace);
}

int32_t main(int argc, char *argv[]) {
//    testNetworking();
//    return 0;

    commandLineArguments["-debug"] = CmdLineArg(0, debug);
    commandLineArguments["-nogui"] = CmdLineArg(1, noGui);
    commandLineArguments["-server"] = CmdLineArg(10, server);
    commandLineArguments["-host"] = CmdLineArg(7, hostOption);
    commandLineArguments["-service"] = CmdLineArg(7, serviceOption);

    initLogger();

    priority_queue<CmdLineArg> args;

    for (int i = 1; i < argc; i++) {
        string s = argv[i];
        if (commandLineArguments.count(s) == 1) {
            CmdLineArg cmdLineArg = commandLineArguments[s];
            if (i + 1 < argc && argv[i + 1][0] != '-') cmdLineArg.payload = argv[++i];
            args.emplace(cmdLineArg);
        } else {
            SPDLOG_WARN("Invalid argument \"{}\"", s);
        }
    }

    while (!args.empty()) {
        CmdLineArg cmdLineArg = args.top();
        args.pop();

        cmdLineArg.execute();
    }

    if (dedicatedServer) {
        if (!openWindow) {
            SPDLOG_CRITICAL("cannot run mirror server without window! exiting");
            return 1;
        }
        if (openWindow) {
            Window window(WIDTH, HEIGHT, "Mirror Server");
            if (!window.valid) {
                SPDLOG_CRITICAL("failed to create window! exiting");
                return 1;
            }

            asio::io_service ioService;
            ServerHelper server(ioService, 2024);

//            MirrorServer mirrorServer(&server, &window);
//            mirrorServer.run();

            MultiplayerServer multiplayerServer(&server, &window);
            multiplayerServer.run();
        }


    } else {
        if (!openWindow) {
            SPDLOG_CRITICAL("cannot run game without window! exiting");
            return 1;
        }
        if (openWindow) {
            Window window(WIDTH, HEIGHT, "Tetris");
            if (!window.valid) {
                SPDLOG_CRITICAL("failed to create window! exiting");
                return 1;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            asio::io_service ioService;
            SocketWrapper socket(ioService);
            socket.connect(host, service);
            socket.startListening();

            InputAdapter *inputAdapter = SDLInputAdapter::get();
            Tetris tetris(&window, inputAdapter, &socket);
            tetris.play();
        }
    }


    return 0;
}
