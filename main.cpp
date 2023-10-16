#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

//**Класс лампочка
class LightBulb
{
public:
    LightBulb();
    void turnOn();
    void turnOff();
    bool getInformationAboutLight();
private:
    bool _isLight;

};
//методы
LightBulb::LightBulb()
{
    _isLight=false; // сначала лампочка выключена
}
void LightBulb::turnOff() // выключаем лампочку
{
    _isLight=false;
}
void LightBulb::turnOn() // включаем лампочку
{
    _isLight=true;
}
bool LightBulb::getInformationAboutLight() // смотрим горит лампочка или нет
{
    return _isLight;
}

//**Класс стратегии
class IStrategy
{
public:
    virtual bool doBehavior (LightBulb* lightBulb)=0;
};

class IDefaultBehavior: public IStrategy
{
    bool doBehavior (LightBulb* lightBulb)
    {
        if(lightBulb->getInformationAboutLight() == false && _isFirst == true)
        {
            lightBulb->turnOn();
            _isFirst = false;
        }
        return false;
    }

private:
    bool _isFirst = true;
};

class ICounterBehavior: public IStrategy
{
    bool doBehavior (LightBulb* lightBulb)
    {
        if(lightBulb->getInformationAboutLight() == true)
        {
            lightBulb->turnOff();
            _count++;
            if(_count == 4)
                return true;
            else
                return false;
        }
        else return false;
    }
private:
    int _count=0;
};

//**Класс заключенного
class Prisoner
{
public:
    Prisoner() {}
    virtual bool enterRoom(LightBulb* lightBulb);
    void setStrategyInRoom (IStrategy* IS);

private:
    IStrategy* _pStrategy;

};
//методы
bool Prisoner::enterRoom(LightBulb* lightBulb)
{
    return _pStrategy->doBehavior(lightBulb);
}
void Prisoner::setStrategyInRoom (IStrategy* IS)
{
    _pStrategy = IS;
}


//**Класс надзирателя
class Warden
{
public:
    Warden(){_pLightBulb = new LightBulb;}
    void startExperiment ();
    void makeContract(); // заключенные договариваются, кто будет счетчиком

private:
    LightBulb* _pLightBulb;
    Prisoner _masPrisoner [5];
};
void Warden::startExperiment()
{
     bool weFree = false;
     srand(time(0));
     while(weFree==false)
     {
         int start = 0;
         int end = 4;
         int i = rand() % (end - start + 1) + start;
         qDebug() << i;
         weFree = _masPrisoner[i].enterRoom(_pLightBulb);
     }
     qDebug()<< "We are free";

}
void Warden::makeContract()
{
    _masPrisoner[0].setStrategyInRoom(new IDefaultBehavior);
    _masPrisoner[1].setStrategyInRoom(new IDefaultBehavior);
    _masPrisoner[2].setStrategyInRoom(new IDefaultBehavior);
    _masPrisoner[3].setStrategyInRoom(new ICounterBehavior);
    _masPrisoner[4].setStrategyInRoom(new IDefaultBehavior);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Warden warden;
    warden.makeContract();
    warden.startExperiment();
    w.show();
    return a.exec();
}
