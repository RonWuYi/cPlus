// Singleton base class hpp file

class sBase
{
public:
    static sBase* instance();
    static bool exists();
    inline int getDataX(){ return mDataX; };
    inline int setDataX(int _in){ mDataX = _in; };
    virtual int getDataY() = 0;
    virtual int setDataY(int _in) = 0;
protected:
    sBase(int);
    virtual ~sBase(){};
    static sBase* mpoSssInstance;
private:
    int mDataX;
};
