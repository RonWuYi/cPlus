//
// Created by Ron on 11/14/2017.
//

#ifndef STONEWT_H
#define STONEWT_H
class Stonewt
{
private:
    enum {Lbs_per_stn = 14};
    int stone;
    double pds_left;
    double pounds;
public:
    Stonewt(double lbs);
    Stonewt(int stn, double lbs);
    Stonewt();
    ~Stonewt();
    void show_lbs()const ;
    void show_stn()const ;
    // conversion functions
    operator int()const ;
    operator double ()const ;
};
#endif //CPLUS_STONEWT_H_H
