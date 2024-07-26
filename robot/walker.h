#pragma once
#include <vector>
#include <string>
#include "servo.h"
#include "basic.h"
#include "mission.h"

class TaPoint{
    public:
        double t, a;
        TaPoint(){}
        TaPoint(double t, double a):t(t), a(a){}
};

class Joint{
    private:
        double t, tSum;
        double a0, a;
        int id;
        bool _paused;
        std::vector<TaPoint> curve;

        double getA(double t);
        static const int STOP_SPEED = 1000;
    public:
        // a0: initial a
        // id: servo id
        Joint(int a0, int id):a0(a0), id(id), _paused(true), t(0){
            Servo::setMode(id, Servo::MODE_ANGLE);
        }
        Joint(){}
        void move(){
            _paused = false;
        }
        void stop(){
            _paused = true;
            Servo::setPos(id, a0, STOP_SPEED);
            t = 0;
        }
        void pause(){
            _paused = true;
            Servo::setPos(id, Servo::getPos(id), STOP_SPEED);
        }
        void setCurve(const std::vector<TaPoint>& curve);

        void step(double dt);
};

class _Leg2{
    private:
        double t;
    public:
        Joint jnt1, jnt2;
        // a0: initial a
        int a01, a02;
        _Leg2(){}
        _Leg2(int a01, int a02, int id1, int id2);
        void step(double dt){
            t += dt;
            jnt1.step(dt);
            jnt2.step(dt);
        }
        void stop(){
            jnt1.stop();
            jnt2.stop();
        }
        void pause(){
            jnt1.pause();
            jnt2.pause();
        }
        void move(){
            jnt1.move();
            jnt2.move();
        }
};

class Walk: public Mission{
    private:
        double speed; // within [0, 1]
        double w;
        _Leg2 legs[4];
        bool end;
        double t;
        MissionManager subMissions;
        bool normal;
    public:
        Walk():Mission("walking mission"), subMissions("managerInWalk"){}
        double getSpeed()const{ return speed;}
        // speed in [0, 1]
        void setSpeed(double speed, const double *vs);
        // left: dir=-1; right: dir=1; back: dir=0; front: dir=2
        void turn(double speed, int dir);
        // direction in [-90, 90] in
        void setAngleSpeed(double w);
        void init();
        void step(double dt);
        void stop();
        bool isEnd();
};

class TurnAfter: public Mission{
    private:
        double t;
        double delay;
        bool finished;
        int dir;
        Walk* pWk;
    public:
        TurnAfter(){}
        TurnAfter(Walk& wk, double delay, int dir):
            pWk(&wk), delay(delay), finished(false), t(0), dir(dir), Mission("turnAfterMission"){
            }
        void init(){}
        bool isEnd(){
            return finished;
        }
        void step(double dt){
            if(finished) return;
            if(sgn(t-delay) > 0){
                printf("turn actived at t = %.2lf\n", t);
                pWk->turn(1, dir);
                finished = true;
            }else{
                t += dt;
            }
        }
};
