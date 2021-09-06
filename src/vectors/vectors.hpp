#ifndef vectors_h
#define vectors_h

class vektor
{
private:
    double x;
    double y;
    double z;

public:
    vektor(); //default
    vektor(vektor &ref); //KopierKonstr
    vektor(double x, double y, double z); //Initialisier

    friend vektor operator*(double val, vektor &THIS);
    vektor operator*(double val);
    double operator*(vektor &val);  //Skalarprodukt
    
    friend vektor operator+(double val, vektor &THIS);
    vektor operator+(double val);

    friend vektor operator-(double val, vektor &THIS);
    vektor operator-(double val);

    vektor operator+(vektor &val);
    vektor operator-(vektor &val);

    double det();
};

#endif