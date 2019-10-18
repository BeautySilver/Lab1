//
// Created by Vovka Pultik on 05.10.2019.
//

#include "PRNG.h"
#include <stdexcept>
#include <cmath>
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

using namespace std;


/// AbstractGenerator implementation

double AbstractGenerator::left() const
{
    if (left_ > right_)
        throw logic_error("Left edge is greater than right edge of distribution generator");
    return left_;
}

double AbstractGenerator::right() const
{
    if (left_ > right_)
        throw logic_error("Left edge is greater than right edge of distribution generator");
    return right_;
}


/// AbstractGeneratorInt implementation
double AbstractGeneratorInt::next() noexcept
{
    return next_int() / double(m);
}


/// Generator1 implementation
unsigned int Generator1::next_int() noexcept
{
    last_int = (a*last_int + c) % m;
    return last_int;
}



/// Generator2 implementation
unsigned int Generator2::next_int() noexcept
{
    last_int = (d*last_int*last_int + a*last_int + c) % m;
    return last_int;
}



/// Generator3 implementation
Generator3::Generator3()
{
    Generator1 g1;
    last_int_next = g1.next_int();
}

unsigned int Generator3::next_int() noexcept
{
    unsigned long sum = last_int_next + last_int;
    last_int = last_int_next;
    last_int_next = sum % m;
    return last_int_next;
}


/// Generator4 implementation
unsigned int Generator4::next_int() noexcept
{
    last_int = (a*reverse(last_int) + c) % m;
    return last_int;
}

unsigned long Generator4::reverse(unsigned long x) noexcept
{
    int x1 = 0, y = 0;
    gcdext(x, m, x1, y);
    return (x1 < 0) ? (-x1)%m : x1%m;
}

void Generator4::gcdext(unsigned a, unsigned b, int &x, int &y) noexcept
{
    if (a == min(a, b))
        swap(a,b);
    if (!b)
    {
        x = a;
        y = 0;
        return;
    }

    gcdext(b, a%b, x, y);
    int t;
    t = y;
    y = x - int(a/b)*y;
    x = t;
}


/// Generator5 implementation
Generator5::Generator5()
{
    last_int = g1.next_int();
    last_int2 = g2.next_int();
}

unsigned Generator5::next_int() noexcept
{
    unsigned long long res = last_int - last_int2 + m;
    last_int = g1.next_int();
    last_int2 = g2.next_int();
    return unsigned(res % m);
}


/// Generator6 implementation

Generator6::Generator6()
{
    left_ = -3;
    right_ = 3;
}

double Generator6::next() noexcept
{
    double sum = 0;
    for (int i = 0; i < 12; i++)
        sum += g1.next();
    return m + (sum - 6)*sigma;
}


/// Generator7 implementation


Generator7::Generator7()
{
    left_ = -3;
    right_ = 3;
}

double Generator7::next() noexcept
{
    if (queue)
    {
        queue = false;
        return next_number;
    }

    double temp;
    double v1, v2;
    do
    {
        v1 = 2*(g1.next()) - 1;
        v2 = 2*(g2.next()) - 1;
        temp = v1*v1 + v2*v2;
    }
    while (temp >= 1 || temp == 0);
    temp = sqrt((-2) * log(temp) / temp);
    next_number = v2 * temp;
    return v1 * temp;
}


Generator8::Generator8()
{
    left_ = -3;
    right_ = 3;
}

double Generator8::next() noexcept
{
    double x, u;
    do {
        do {
            double v = g1.next();
            do
                u = g2.next();
            while (!u);
            x = sqrt(8 / M_E) * ((v - 0.5) / u);

            if (x*x <= upper_bound(u))
                return x;
        } while (x*x >= lower_bound(u));
    }
    while (x*x > final_check(u));
    return x;
}

double Generator8::upper_bound(double u) const noexcept
{
    return 5 - 4*pow(M_E, 0.25)*u;
}

double Generator8::lower_bound(double u) const noexcept
{
    return 4*pow(M_E, -1.35)/u + 1.4;
}

double Generator8::final_check(double u) const noexcept
{
    return (-4) * log(u);
}

Generator9::Generator9()
{
    left_ = 0;
    right_ = 100;
}

double Generator9::next() noexcept
{
    double u = g1.next();
    return log(1-u) * mu;
}


/// Generator10 implementation
Generator10::Generator10()
{
    left_ = 0;
    right_ = 100;
}

double Generator10::next() noexcept
{
    double u, y, x, v;
    do
    {
        do
        {
            do
                u = g1.next();
            while (u == 0.5);

            y = tan(u * M_PI);
            x = sqrt(2 * a - 1) * y + a - 1;
        } while (x <= 0);
        v = g2.next();
    }
    while (v > check(x, y));
    return x;
}

double Generator10::check(double x, double y) const noexcept
{
    return (1 + y*y)  *  exp((a-1) * log(x/(a-1)) - sqrt(2*a - 1)*y);
}