// main.cpp
// Author  : yujinh@126.com
// Address : CUG @ Wuhan
// Date    : 2022-12-26
#include <Arduino.h>
#include "ndtCS1237.hpp"
#include "ndtStimuAB.hpp"

HardwareSerial &pc = Serial; //给默认串口取一个别名

ndtCS1237 a1237(14, 12);
ndtCS1237 u1237(13, 15);
StimuAB ab(16, 17); //用于分压电阻的供电
void setup()
{
    u1237.powerdown();
    a1237.powerdown();

    pc.begin(115200);

    delay(1); // cs1237 will power down in 100us

    u1237.poweron();
    a1237.poweron();
    delay(1);

    a1237.writeConfig(0, 0, 0, 0); //set pga = 1 to avoid outflow
    u1237.writeConfig(0, 0, 0, 0); //set pga = 1 to avoid outflow
    ab.b2a();
}


void loop()
{
    delay(120);
    int u = u1237.read();
    int a = a1237.read();

    pc.printf("u = 0x%08x,%f",u,u*0.5*3.3/0x7FFFFF);
    pc.printf(",a = 0x%08x,%f\n",a,a*0.5*3.3/0x7FFFFF);
}

