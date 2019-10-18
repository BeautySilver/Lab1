#include "PRNG.h"
#include <iostream>
#include <iomanip>
#include <cmath>


using namespace std;

double fround(double x, int precision)
// returns rounded number
// rounds to 10**precision (e. g. -2 => 2 digits after comma)
{
    double m = pow(10, -precision);
    return round(m * x) / m;
}

void print_table(AbstractGenerator *generator, const int *arr, unsigned arr_size, unsigned sequence_period)
{
    double step = abs(generator->left() - generator->right()) / arr_size;
    double left = generator->left();
    for (size_t i = 0; i < arr_size - 1; i++)
    {
        cout << "[" << fround(left, -2) << "; " << fround(left + step, -2) << ") \t-\t";
        cout << 100 * arr[i] / sequence_period << "%\n";
        left += step;
    }
    cout << "[" << fround(left, -2) << "; " << fround(left + step, -2) << "]\t-\t";
    cout << 100 * arr[arr_size - 1] / sequence_period << "%\n";
}

void test(AbstractGenerator *generator, unsigned intervals=10)
// prints table for distribution of generator's random numbers
// 'intervals' shows how many parts distribution interval will be divided by
{
    if (intervals <= 0)
        throw logic_error("Number of sub-intervals cannot be negative or 0");
    const unsigned period = 10000;
    int arr[intervals];
    for (size_t i = 0; i < intervals; i++)
        arr[i] = 0;

    double current, current_mod;
    int int_current;
    for (int i = 0; i < period; i++)
    {
         current = generator->next();

        // here is bijection between interval [left; right] and  interval [0, 10] for easier searching of correct index
        // number a from [m, n] after multiplying by (intervals*(a - m)/(n - m)) lands at [0, intervals]
        current_mod = intervals * (current - generator->left()) / (generator->right() - generator->left());
        if (current_mod == intervals)
        {
            arr[intervals - 1]++;
            continue;
        }
        int_current = int(current_mod);
        if (int_current >= 0 && int_current <= intervals - 1)
            arr[int_current]++;
    }
    print_table(generator, arr, intervals, period);

}

int main()
{
    int a;
    while (true)
    {
        cout << "Input a number from 1 to 10 to choose a pseudo-random numbers generator to test" << endl;
        cin >> a;
        if (a < 1 || a > 10 || !cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "WRONG! YOU MUST CHOOSE A NUMBER FROM 1 TO 10!\n" << endl;
        }
        else
            break;
    }

    AbstractGenerator *pgenerator = nullptr;
    switch (a)
    {
        case 1:
            pgenerator = new Generator1;
            break;
        case 2:
            pgenerator = new Generator2;
            break;
        case 3:
            pgenerator = new Generator3;
            break;
        case 4:
            pgenerator = new Generator4;
            break;
        case 5:
            pgenerator = new Generator5;
            break;
        case 6:
            pgenerator = new Generator6;
            break;
        case 7:
            pgenerator = new Generator7;
            break;
        case 8:
            pgenerator = new Generator8;
            break;
        case 9:
            pgenerator = new Generator9;
            break;
        case 10:
            pgenerator = new Generator10;
            break;
        default:
            cout << "Oooops, unpredictable mistake. Wait for new update of our app! \n";
    }

    try
    {
        test(pgenerator);
    }
    catch (const exception &ex)
    {
        cout << ex.what() << endl;
    }

    delete pgenerator;

    return 0;
}