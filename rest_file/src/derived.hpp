// Singleton derived class hpp file

#include "base.hpp"

class sDerived : public sBase
{
public:
    static void create(int,int);
    virtual inline int getDataY(){ return mDataY; };
    virtual inline int setDataY(int _in){ mDataY = _in; };
protected:
    sDerived(int,int);      // Can't be called by non-member functions
    virtual ~sDerived() {}; // Can't be called by non-member functions
private:
    int mDataY;
};
