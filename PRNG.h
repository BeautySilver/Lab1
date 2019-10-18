//
// Created by Vovka Pultik on 05.10.2019.
//
// Pseudo-random numbers generators

#ifndef LAB1_PRNG_H
#define LAB1_PRNG_H


#include <ctime>


class AbstractGenerator
{
public:
    // returns next random number
    virtual double next() noexcept = 0;
    [[nodiscard]] virtual double left() const;
    [[nodiscard]] virtual double right() const;

    virtual ~AbstractGenerator() = default;
protected:
    // edges of distribution interval, 'left_' <= 'right_'
    double left_ = 0;
    double right_ = 1;
};

class AbstractGeneratorInt: public AbstractGenerator
/// Abstract superclass for classes that operates with integer pseudo-random numbers
{
public:
    virtual unsigned int next_int() noexcept = 0;
    double next() noexcept override;
protected:
    static constexpr unsigned long m = 8388607; // 2**23 - 1;
    // before first number is generated, this variable is considered as seed
    unsigned long last_int = (unsigned long)time(nullptr) % m;
};


class Generator1: public AbstractGeneratorInt
/// Linear congruential generator
{
public:
    unsigned int next_int() noexcept final;
private:
    static constexpr int a = 2896; // int(sqrt(m))
    static constexpr int c = 2;
};


class Generator2: public AbstractGeneratorInt
/// Quadratic congruential generator
{
public:
    unsigned int next_int() noexcept final;
private:
    static constexpr int d = 2896; // int(sqrt(m))
    static constexpr int a = 1449; // average of 'd' and 'c'
    static constexpr int c = 2;
};


class Generator3: public AbstractGeneratorInt
/// Fibonacci numbers generator
{
public:
    Generator3();
    unsigned int next_int() noexcept final;
private:
    // next number after the 'last_int' in pseudo-random sequence
    unsigned long last_int_next;
};


class Generator4: public AbstractGeneratorInt
/// Inversive congruential generator
{
public:
    unsigned int next_int() noexcept final;
private:
    unsigned long reverse(unsigned long x) noexcept;
    void gcdext(unsigned a, unsigned b, int &x, int &y) noexcept;
    // finds x and y: ax + by = 1
    // a, b > 0

    static constexpr unsigned a = 131073; // 2**17 + 1
    static constexpr unsigned c = 2;
};


class Generator5: public AbstractGeneratorInt
/// Union-method generator
{
public:
    Generator5();
    unsigned next_int() noexcept final;
private:
    Generator1 g1;
    Generator2 g2;

    unsigned long last_int2;
};

class Generator6: public AbstractGenerator
{
public:
    Generator6();
    double next() noexcept final;
private:
    Generator1 g1;

    static constexpr int m = 0;
    static constexpr int sigma = 1;
};


class Generator7: public AbstractGenerator
/// Polar coordinates method.
{
public:
    Generator7();
    double next() noexcept final;
private:
    Generator1 g1;
    Generator2 g2;

    bool queue = false;
    double next_number = 0;
};


class Generator8: public AbstractGenerator
/// Relation-method generator
{
public:
    Generator8();
    double next() noexcept final;
private:
    [[nodiscard]] double upper_bound(double u) const noexcept;
    [[nodiscard]] double lower_bound(double u) const noexcept;
    [[nodiscard]] double final_check(double u) const noexcept;

    Generator1 g1;
    Generator2 g2;
};


class Generator9: public AbstractGenerator
/// Logarithm-method for exponential distribution
{
public:
    Generator9();
    double next() noexcept final;
private:
    Generator1 g1;
    static constexpr double mu = -20;
};


class Generator10: public AbstractGenerator
/// Gamma-distribution method
{
public:
    Generator10();
    double next() noexcept final;
private:
    [[nodiscard]] double check(double x, double y) const noexcept;

    Generator1 g1;
    Generator2 g2;

    static constexpr double a = 35;
};
#endif //LAB1_PRNG_H
