#pragma once
#include "mission.h"
#include "servo.h"

const int JOINT_NUM = 8;
const int JOINT_IDS[JOINT_NUM] = {11, 2, 10, 3, 4, 13, 14, 12};

class PrintPos:public Mission{
    public:
        void init(){
            t = 0;
            for(int i = 0; i < JOINT_NUM; i++){
                Servo::setMode(JOINT_IDS[i], Servo::MODE_ANGLE);
                // Servo::setPos(JOINT_IDS[i], 522, 512);
            }
            Servo::action();
        }
        void step(double dt){
            if(t > 0.2){
                printf("joints' state, num=%d: \n", JOINT_NUM);
                for(int i = 0; i < JOINT_NUM; i++){
                    int pos, id;
                    id = JOINT_IDS[i];
                    pos = Servo::getPos(JOINT_IDS[i]);
                    printf("id=%d, pos=%d\n", id, pos);
                }
            }
            t += dt;
        }
};
