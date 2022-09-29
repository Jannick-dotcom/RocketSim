#ifndef vectors_h
#define vectors_h

#include <math.h>

class vektor
{
private:
    double x;
    double y;
    double z;

public:
    vektor(); //default
    vektor(const vektor &ref); //KopierKonstr
    vektor(double x, double y, double z); //Initialisier

    friend vektor operator*(double val, vektor THIS);
    vektor operator*(double val);
    double operator*(vektor val);  //Skalarprodukt
    
    friend vektor operator+(double val, vektor THIS);
    vektor operator+(double val);
    vektor operator+(vektor val);

    friend vektor operator-(double val, vektor THIS);
    vektor operator-(double val);
    vektor operator-(vektor val);

    // vektor operator=(vektor val);
    vektor &operator=(const vektor &val);

    double det();
    double getx();
    double gety();
    double getz();
    double getlength();


    void setx(double x);
    void sety(double y);
    void setz(double z);

    vektor normalize();
};

#endif