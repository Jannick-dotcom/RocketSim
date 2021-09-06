#include "vectors.hpp"

vektor::vektor()
{
    this->x = this->y = this->z = 0;
}

vektor::vektor(vektor &ref)
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

vektor operator*(double val, vektor &THIS)
{
    THIS.x *= val;
    THIS.y *= val;
    THIS.z *= val;
    return THIS;
}

vektor vektor::operator*(double val)
{
    vektor temp(this->x * val, this->y * val, this->z * val);
    return temp;
}

double vektor::operator*(vektor &val)
{
    return this->x * val.x + this->y + val.y + this->z + val.z;
}

vektor operator+(double val, vektor &THIS)
{
    vektor temp(val - THIS.x, val - THIS.y, val - THIS.z);
    return temp;
}

vektor vektor::operator+(double val)
{
    vektor temp(this->x + val, this->y + val, this->z + val);
    return temp;
}

vektor operator-(double val, vektor &THIS)
{
    vektor temp(THIS.x - val, THIS.y - val, THIS.z - val);
    return temp;
}

vektor vektor::operator-(double val)
{
    vektor temp(this->x - val, this->y - val, this->z - val);
    return temp;
}

vektor vektor::operator+(vektor &val)
{
    vektor temp(this->x + val.x, this->y + val.x, this->z + val.x);
    return temp;
}

vektor vektor::operator-(vektor &val)
{
    vektor temp(this->x - val.x, this->y - val.x, this->z - val.x);
    return temp;
}

