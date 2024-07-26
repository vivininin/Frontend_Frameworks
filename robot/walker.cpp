#include "walker.h"
#include <algorithm>

// #define PRINT

const int JOINT_NUM = 8;
const int JOINT_IDS[JOINT_NUM] = {11, 2, 10, 3, 4, 13, 14, 12};
// const int JOINT_A0S[JOINT_NUM] = {723, 512, 512, 642, 562, 483, 500, 498};
const int JOINT_A0S[JOINT_NUM] = {723, 476, 512, 628, 562, 456, 500, 457};
const int JOINT_DIRS[JOINT_NUM] = {1, 1, -1, 1, -1, 1, 1, 1};
const double ROTA_SPEED_K = 0.12;

_Leg2::_Leg2(int a01, int a02, int id1, int id2):
    t(0),
    jnt1(a01, id1),
    jnt2(a02, id2){
}

void Joint::setCurve(const std::vector<TaPoint>& curve){
    this->curve = curve;
    tSum = curve.back().t - curve[0].t;
#ifdef FAKE
    int i;
    printf("curve: ");
    for(i = 0; i < curve.size(); i++){
        printf("(%.2lf, %.2lf)-", curve[i].t, curve[i].a);
    }
    printf("\n");
#endif
}

double Joint::getA(double t){
    int i, j, k, n;
    n = curve.size();
    /* let T = tSum
     * suppose k T + t[0] <= t < k T + t[-1]
     * then (t - t[-1])/T < k <= (t - t[0])/T
     *
     * t[0] <= t1 = t - k T < t[-1]
     *
     */
    k = (t - curve[0].t) / tSum;
    // if(k < 0) return curve[0].a;
    double t1 = t - k * tSum;
    for(i = 1; i < n and curve[i].t <= t1; i++);
    // curve[i-1].t <= t1 < curve[i].t
    j = i - 1;
#ifdef PRINT
    // printf("at %d-%d (%.2lf, %.2lf)-(%.2lf, %.2lf)\n", j, i, curve[j].t, curve[j].a, curve[i].t, curve[i].a);
#endif
    double a = curve[j].a + 
        (curve[j].a - curve[i].a)/(curve[j].t - curve[i].t) * (t1 - curve[j].t);
    return a;
}

void Joint::step(double dt){
    if(not _paused){
        double a0 = Servo::getPos(id);
        a = getA(t);
        int speed = abs(ROTA_SPEED_K * (a - a0) / dt);
        Servo::setPos(id, a, speed);

#ifdef PRINT
#ifdef FAKE
        printf("Joint id=%d, set pos=%d, speed=%d\n",
                id, (int)a, speed);
#else
        printf("Joint id=%d, set from %d to pos=%d, speed=%d\n", id, (int)a0, (int)a, (int)(a - a0));
#endif
#endif
    }
    t += dt;
}

const double VS_DEFAULT[4] = {1, 1, 1, 1};

void Walk::setSpeed(double speed, const double *vs=NULL){
#define PU1(t_, a_) curve.push_back(TaPoint(offset+(t_), (a_)*JOINT_DIRS[i*2]*vs[i] + JOINT_A0S[i*2]));
#define PU2(t_, a_) curve.push_back(TaPoint(offset+(t_), (a_)*JOINT_DIRS[i*2+1] + JOINT_A0S[i*2+1]));
    this->speed = speed;
    std::vector<TaPoint> curve;
    double offset = 0;
    int i;
    if(vs == NULL) vs = VS_DEFAULT;

    if(sgn(speed) == 0){
        for(i = 0; i < 4; i++){
            legs[i].pause();
        }
        return;
    }
    double scale1 = std::min(0.5, 0.5/speed); // 1/4 T of joint 1
    double scale2 = 160; // range of joint 1
    double scale3 = 0.2; // up time of joint 2/scale1
    double scale4 = 65; // up range of joint 2
    double scale5 = 120; // high up range of joint 2
    // too noisy, but fast
    double offsets[4] = {0, scale3*scale1, 2*scale1, (scale3+2)*scale1};
    // double offsets[4] = {0, 0, 2*scale1, 2*scale1};
    //
    // slow
    // double offsets[4] = {0, scale1, 2*scale1, 3*scale1};
    //

    for(i = 0; i < 4; i++){
        offset = offsets[i];
        // build curve 1
        curve.clear();
        PU1(0, 0); 
        PU1(scale1, scale2);
        PU1(scale1 * 2, 0);
        PU1(scale1 * 3, -scale2);
        PU1(scale1 * 4, 0);
        legs[i].jnt1.setCurve(curve);
        // build curve 2
        curve.clear();
        PU2(0, scale5);
        // PU2(scale1*(1 - scale3), scale5);
        PU2(scale1, scale4);
        PU2(scale1*(1+scale3), 0);
        PU2(scale1*2, 0);
        PU2(scale1* 3, scale5);
        PU2(scale1* 4, scale5);
        legs[i].jnt2.setCurve(curve);

        legs[i].move();
    }
}

void Walk::turn(double speed, int dir){
    if(dir == -1){
        double vs[4] = {1, -1, -1, 1};
        setSpeed(speed, vs);
    }else if(dir == 1){
        double vs[4] = {-1, 1, 1, -1};
        setSpeed(speed, vs);
    }else if(dir == 0){
        double vs[4] = {-1, -1, -1, -1};
        setSpeed(speed, vs);
    }else if(dir == 2){
        double vs[4] = {1, 1, 1, 1};
        setSpeed(speed, vs);
    }
};

void Walk::init(){
    int i;
    for(i = 0; i < 4; i++){
        legs[i] = _Leg2(JOINT_A0S[i*2], JOINT_A0S[i*2+1],
                JOINT_IDS[i*2], JOINT_IDS[i*2+1]);
    }
    stop();
    end = false;
    normal = false;
    t = 0;
}

bool Walk::isEnd(){
    return end;
}

void Walk::stop(){
    int i;
    for(i = 0; i < 4; i++){
        legs[i].stop();
    }
    Servo::action();
}

void Walk::step(double dt){
    int i;
    if(not end){
        for(i = 0; i < 4; i++)
            legs[i].step(dt);
        if(MFGetDigiInput(0) == 0){
            end = true;
            stop();
        }
        if(subMissions.isEnd()){
            subMissions.init();
            bool frontObs = (MFGetDigiInput(2) == 0);
            bool leftObs = (MFGetDigiInput(3) == 0);
            bool rightObs = (MFGetDigiInput(1) == 0);
            bool downLeftEmpty = (MFGetDigiInput(5) == 1);
            bool downRightEmpty = (MFGetDigiInput(4) == 1);
            if(downLeftEmpty or downRightEmpty){
                // walk back
                turn(0.5, 0);
                subMissions.add(new Timer(3, "keepWalkBack"));
                // and turn
                subMissions.add(new TurnAfter(*this, 3, downLeftEmpty?1:-1));
                subMissions.add(new Timer(6, "keepTurn"));

                normal = false;
            }else if(frontObs or leftObs or rightObs){
                if(rightObs and not leftObs){
                    // turn left
                    turn(0.7, -1);
                }else if(leftObs and not rightObs){
                    // turn right 
                    turn(0.7, 1);
                }else{
                    // turn back
                    turn(0.5, 0);
                }
                subMissions.add(new Timer(3, "keepTurn"));
                normal = false;
            }else{
                if(not normal){
                    turn(1, 2);
                    normal = true;
                }
            }
        }else{
            subMissions.step(dt);
        }
        Servo::action();
    }
    t += dt;
}

