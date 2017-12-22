//
// Created by hdc on 12/22/17.
//
#ifndef CPLUS_TV_H
#define CPLUS_TV_H

class Tv
{
public:
    friend class Remote;
    enum {Off, On};
    enum {MinVal, MaxVal = 20};
    enum {Antenna, Cable};
    enum {TV, VCR};

    Tv(int s = Off, int mc = 100):state(s),volume(5),

private:
    int state;
    int volume;
    int maxchannel;
    int channel;
    int mode;
    int input;
};
#endif //CPLUS_TV_H
