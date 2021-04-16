#include <iostream>
#include "asio.hpp"
//#include "spdlog/spdlog.h"
#include "CLI11.hpp"
#include <chrono>

using namespace std;
using namespace asio::ip;
using namespace CLI;

//constexpr char EMPTY_MSG[] = "";


double get_time(int min, int max) {
    double time = min +(rand() % (max-min + 1));
    return time;
}

double get_speed(int dis, double time) {
    return (dis/time)*3.6;
}



int main(int argc, char* argv[]){
    //argv[1] = distance int meters between the section Controller
    //argv[2] = the minimum time in seconds the car needs to drive the distance
    //argv[3] = the minimum time in seconds the car needs to drive the distance
    //argv[4] = the sum of cars which should drive on the way
    

    
        

    App app{"Section Controll client"};
    unsigned int distance{}, min_time{}, max_time{}, cars{};
    app.add_option("-d, --distance", distance, "distance between Controller")->required();
    app.add_option("-s, --minTime", min_time, "minimum time")->required();
    app.add_option("-k, --maxTime", max_time, "maximum Time")->required();
    app.add_option("-c, --cars", cars, "sum of cars which should drive trough")->required();

    CLI11_PARSE(app, argc, argv);
    
    tcp::iostream strm{"localhost", "9999"};
    if(min_time > max_time) {
        cerr << "The Minimum Time should be less than the Maximum time";
    }
    
    if(strm){
        
        for(;;) {
            double t = get_time(min_time, max_time);
            double s = get_speed(distance, t);
            cout << t << " " << s << endl;
            if(s > 133) {
                strm << to_string(s) << endl;
            }

            this_thread::sleep_for(3s);
            
            
        }
    }
    else {
        cout << "no connection" << endl;
    }
    
    
}


