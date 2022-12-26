// ndtCS1237.hpp
// 
// 展示CS1237应用的驱动，参考CS1237的数据手册，包含引脚的初始化
//  和24位结果的读取以及配置
//
// Author  : yujinh@126.com
// Address : CUG @ Wuhan
// Date    : 2022-12-26

#include <Arduino.h>

class ndtCS1237
{
public:
    typedef struct tag_cfg
    {
        unsigned char ch : 2;
        unsigned char pga : 2;   // 0:1 ; 1:2 ; 2:64 ; 3:128
        unsigned char speed : 2; // 0:10Hz ; 1:40Hz ; 2:640Hz ; 3:1280Hz
        unsigned char ref : 1;   // defualt 0 is on;
        unsigned char res : 1;   // default 0
    } TCFG, *PTCFG;

    ndtCS1237(int _clk, int _dio) : clk(_clk), dio(_dio)
    {
        pinMode(dio, INPUT_PULLUP);
        pinMode(clk, OUTPUT);
        digitalWrite(clk, LOW);
    }

    bool isReady()
    {
        return digitalRead(dio) == LOW;
    }

    void powerdown()
    {
        digitalWrite(clk, HIGH);
    }
    void poweron()
    {
        digitalWrite(clk, LOW);
    }

    int readbit()
    {
        int r;
        digitalWrite(clk, HIGH);
        delayMicroseconds(20);
        r = digitalRead(dio);
        delayMicroseconds(25);
        digitalWrite(clk, LOW);
        delayMicroseconds(45);

        return r;
    }
// the lower byte is packed status bits and 5 reads of bus status
//  to use the result,just divide it with 256  
    int read()
    {
        int val = 0;
        pinMode(dio, INPUT_PULLUP);

        for (int i = 0; i < 24; i++)
        {
            val <<= 1;
            val |= readbit();
        }

        //sclk 25,26,27,to make the cs1237's dio output high
        for (int i = 0; i < 3; i++)
        {
            val <<= 1;
            val |= readbit();
        }

        //simple read,no clk.just make sure the dio bus is in high status
        for (int i = 0; i < 5; i++)
        {
            val <<= 1;
            val |= digitalRead(dio);
        }

        return val/256;
        // return val;
    }

    void writebit(int b)
    {
        digitalWrite(clk, HIGH);
        digitalWrite(dio, b ? HIGH : LOW);
        digitalWrite(clk, LOW);
    }

    int writeConfig(int ch = 0, int pga = 0, int speed = 0, int ref = 0)
    {
        read();

        // SCLK 28~29
        readbit();
        readbit();

        // SCLK 30~36,write read config command
        unsigned char cmd = 0x65;
        pinMode(dio, OUTPUT);
        for (int i = 0; i < 7; i++)
        {
            cmd <<= 1;
            writebit(cmd & 0x80);
        }

        // SCLK 37
        readbit();

        unsigned char c;
        TCFG *cfg = (PTCFG)&c;
        cfg->pga = pga;
        cfg->ch = ch;
        cfg->ref = ref;
        cfg->speed = speed;
        cfg->res = 0;

        // SCLK 38~45
        for (int i = 0; i < 8; i++)
        {
            writebit(c & 0x80);
            c <<= 1;
        }

        // SCLK 46
        pinMode(dio, INPUT_PULLUP);
        readbit();
        return 0;
    }

    unsigned char readconfig(){
        read();

        //SCLK 28~29
        readbit();
        readbit();

        // SCLK 30~36,write read config command
        unsigned char cmd = 0x56;
        pinMode(dio, OUTPUT);
        for (int i = 0; i < 7; i++)
        {
            cmd <<= 1;
            writebit(cmd & 0x80);
        }

        // SCLK 37
        pinMode(dio, INPUT_PULLUP);
        readbit();

        unsigned char cfg=0;        

        // SCLK 38~45
        for (int i = 0; i < 8; i++)
        {
            cfg<<=1;
            cfg |= readbit();
            
        }

        // SCLK 46
        pinMode(dio, INPUT_PULLUP);
        readbit();
        return cfg;
    }

private:
    int clk;
    int dio;
};

