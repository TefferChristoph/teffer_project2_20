
#include "asio.hpp"
#include "CLI11.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <mutex>
#include <chrono>
//#include "spdlog/spdlog.h"

using namespace std;
using namespace asio::ip;
using namespace CLI;

vector<tuple<double, double>> records;
mutex mut;

double get_way(double speed){
    
    return 180 * (speed/3.6);
    //3minutes after the signal the police will arrive at the place where the car should be
}


void push_to_vector(double speed, double way) {
    lock_guard<mutex> lock(mut);
    records.push_back(make_tuple(speed, way));
}
void police_signal() {
    this_thread::sleep_for(180s);
    lock_guard<mutex> lock(mut);
    for (auto &val: records){
        double speed = get<0>(val); 
        double way =  get<1>(val);
        cout << "Speed: " << speed<< " km/h" << endl;
        cout << "Way: " << way << " meters" << endl;
        cout <<"----------------------------------" << endl;
    }
    
}
void get_to_car(tcp::iostream client) {
    while(client) {
        int speed{};
        string help = "";
        getline (client,help);
        std::istringstream ss(help); ss >> speed;
        double way = get_way(speed);
        push_to_vector(speed, way);
        
    }
    
}

int main(int argc, char* argv[]) {
    
   App app("Section Controll Server");
   CLI11_PARSE(app, argc, argv);
   thread t{police_signal};
   t.detach();


   asio::io_context ctx;
   tcp::endpoint end(tcp::v4(), 9999);
   tcp::acceptor acceptor{ctx, end};
   acceptor.listen();
   
   while(true) {
        tcp::iostream client;   
        acceptor.accept(client.socket());
        thread(get_to_car, move(client)).detach();
        
   }
   
   
   

}


