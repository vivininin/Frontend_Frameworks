#pragma once
#include <string>
#include <vector>
#include <sstream>

class Mission{
    protected:
        bool end;
        double t;
    public:
        std::string name;
        Mission(std::string misson_name="unknownMission"):
            name(misson_name), end(false), t(0){};
        virtual ~Mission(){};
        // call before the mission start
        virtual void init()=0;
        // make progress on the mission dt -> deltaTime
        virtual void step(double dt)=0;
        // test if the mission if finished
        virtual bool isEnd(){return end;};
};

class MissionManager:public Mission{
    private:
        std::vector<Mission*> missions;
    public:
        MissionManager(std::string name=""){
            if(name.size() == 0){
                std::stringstream ss;
                ss << "missionManager(" << (void*)this << ")";
            }else{
                this->name = name;
            }
        }
        void add(Mission* newMission);
        void init();
        void step(double dt);
};

class Timer:public Mission{
    private:
        double time;
    public:
        Timer(double time):time(time), Mission("timerMission"){}
        Timer(double time, std::string name):time(time), Mission(name){}
        void init(){ }
        void step(double dt){
            if(t > time){
                end = true;
            }
            t += dt;
        }
};

/* example usage:
 *
 * #include "mission.h"
 *
 * class MyMission:public Mission{
 *      private:
 *          double time;
 *      public:
 *
 *          void init() {time = 0}
 *          void step(double dt){
 *              time += dt;
 *          }
 *          void isEnd(){
 *              return time > 2;
 *          }
 * };
 *
 * int main(){
 *      MyMission mm;
 *      mm.init();
 *      while(not mm.isEnd()){
 *          mm.step(0.1);
 *      }
 * }
 *
 */
