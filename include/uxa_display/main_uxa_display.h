#ifndef MAINSENSORHUB_H
#define MAINSENSORHUB_H


/*
 * =========== timer variable ===========
 */
#define LOOP_RATE_1000Hz_ 1000

/*
 * COUNT is defined for timer 1ms
 */
#define COUNT_50_HZ_	20
#define COUNT_100_HZ_	10
#define COUNT_125_HZ_   8
#define COUNT_25_HZ_ 40
struct TimerCountType{
    unsigned char Hz_100;
    unsigned char Hz_50;
    unsigned char Hz_125;
    unsigned char Hz_25;
}Timer_Count;

struct FlagTimerType{
    unsigned char Hz_50:1;
    unsigned char Hz_100:1;
    unsigned char Hz_125:1;
    unsigned char Hz_25:1;
}FlagTimer;
//======timer handler run in 1000Hz=========
void Timer_handler(){
    Timer_Count.Hz_50++;
    Timer_Count.Hz_100++;
    Timer_Count.Hz_125++;
    Timer_Count.Hz_25++;
    if(Timer_Count.Hz_50==COUNT_50_HZ_){
        Timer_Count.Hz_50=0;
        FlagTimer.Hz_50=1;
    }

    if(Timer_Count.Hz_25==COUNT_25_HZ_){
        Timer_Count.Hz_25=0;
        FlagTimer.Hz_25=1;
    }
    if(Timer_Count.Hz_100==COUNT_100_HZ_)
    {
        Timer_Count.Hz_100=0;
        FlagTimer.Hz_100=1;
    }
    if(Timer_Count.Hz_125==COUNT_125_HZ_)
    {
        Timer_Count.Hz_125=0;
        FlagTimer.Hz_125=1;
    }
}


struct Sys_flag_struct{
    unsigned char display_sam_feedback:1;
    unsigned char display_sam_control:1;
    unsigned char display_sam_control_upper:1;
    unsigned char display_sensor:1;
    unsigned char sam_control_avail:1;
    unsigned char sam_feedback_avail:1;
}sys_flag;



/*
 * =========== sam variable ===========
 */
const double pos12bitTorad=0.083*M_PI/180;
const int samPos12_hardware[25]={1620,1680,2050,2060,663,3325,1260,2910,2730,2173,1237,2370,
                                 2025,2050,2050,2050,2050,2050,3100,940,0,0,2170,1500,2050};// zero of the real system 12bits

unsigned char samPos12Avail[25];
unsigned int samPos12[25];
double samDegree[25];
unsigned char samMode[25];
double angle[25];
const int angle_sign_rviz[25]={1,1,-1,1,-1,-1,1,-1,1,1,1,1,
                    1,-1,-1,-1,1,-1,1,1,1,1,-1,-1,1};

#endif // MAINSENSORHUB_H
