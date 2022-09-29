#include "vectors.hpp"

vektor::vektor()
{
    this->x = this->y = this->z = 0;
}

vektor::vektor(const vektor &ref)
{
    this->x = ref.x;
    this->y = ref.y;
    this->z = ref.z;
}

vektor::vektor(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vektor operator*(double val, vektor THIS)
{ 
    vektor temp(val * THIS.x, val * THIS.y, val * THIS.z);
    return temp;
}

vektor vektor::operator*(double val)
{
    vektor temp(val * this->x, val * this->y, val * this->z);
    return temp;
}

double vektor::operator*(vektor val)
{
    return this->x * val.x + this->y + val.y + this->z + val.z;
}

vektor operator+(double val, vektor THIS)
{
    vektor temp(val + THIS.x, val + THIS.y, val + THIS.z);
    return temp;
}

vektor vektor::operator+(double val)
{
    vektor temp(this->x + val, this->y + val, this->z + val);
    return temp;
}

vektor operator-(double val, vektor THIS)
{
    vektor temp(THIS.x - val, THIS.y - val, THIS.z - val);
    return temp;
}

vektor vektor::operator-(double val)
{
    vektor temp(this->x - val, this->y - val, this->z - val);
    return temp;
}

vektor vektor::operator+(vektor val)
{
    vektor temp(this->x + val.x, this->y + val.y, this->z + val.z);
    return temp;
}

vektor vektor::operator-(vektor val)
{
    vektor temp(this->x - val.x, this->y - val.y, this->z - val.z);
    return temp;
}

vektor &vektor::operator=(const vektor &val)
{
    this->x = val.x;
    this->y = val.y;
    this->z = val.z;
    return *this;
}

double vektor::det()
{
    return this->x * this->y * this->z;
}

double vektor::getx()
{
    return this->x;
}

double vektor::gety()
{
    return this->y;
}

double vektor::getz()
{
    return this->z;
}

double vektor::getlength()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

void vektor::setx(double x)
{
    this->x = x;
}

void vektor::sety(double y)
{
    this->y = y;
}

void vektor::setz(double z)
{
    this->z = z;
}

vektor vektor::normalize()
{
    vektor temp = *this;
    double length = this->getlength();
    if(length != 0.0)
    {
        temp.setx(this->x / length);
        temp.sety(this->y / length);
        temp.setz(this->z / length);
    }
    return temp;
}