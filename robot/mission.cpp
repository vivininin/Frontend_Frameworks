#include "mission.h"
#define For(i, n) for(int i = 0; i < (int)(n); i++)

void MissionManager::add(Mission* newMission){
    missions.push_back(newMission);
    newMission->init();
    end = false;
    printf("add %s to %s\n", newMission->name.c_str(), this->name.c_str());
}

void MissionManager::init(){
    missions.clear();
    end = false;
}

void MissionManager::step(double dt){
    end = true;
    For(i, missions.size()){
        if(not missions[i]->isEnd()){
            end = false;
            missions[i]->step(dt);
            if(missions[i]->isEnd()){
                printf("%s ended.\n", missions[i]->name.c_str());
            }
        }
    }
}
