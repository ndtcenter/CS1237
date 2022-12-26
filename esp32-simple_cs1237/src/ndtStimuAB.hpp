// ndtStimuAB.hpp
// 
// 用于控制H桥的输出，以对分压电阻进行供电
//
// Author  : yujinh@126.com
// Address : CUG @ Wuhan
// Date    : 2022-12-26

#include <Arduino.h>

class StimuAB
{
public:
    StimuAB(int _pinA, int _pinB) : A(_pinA), B(_pinB)
    {
        pinMode(A, OUTPUT);
        pinMode(B, OUTPUT);

        off();
    }
    void a2b()
    {
        digitalWrite(A, HIGH);
        digitalWrite(B, LOW);
    }
    void b2a()
    {
        digitalWrite(A, LOW);
        digitalWrite(B, HIGH);
    }

    void off()
    {
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
    }

    void toggle()
    {
        digitalWrite(A, digitalRead(A) == LOW ? HIGH : LOW);
        digitalWrite(B, digitalRead(B) == LOW ? HIGH : LOW);
    }

private:
    int A;
    int B;
};