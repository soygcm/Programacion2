#include <iostream>
using namespace std;

class Pareja {
public:
    double a, b;

    // constructor parametrizado
    Pareja(const double a,const double b)
    {
        //cout << this << endl;
        this->a = a;
        this->b = b;
    }

    Pareja(const Pareja &p){
        //cout << p.a << endl;
        a = p.a;
        b = p.b;
    }

    Pareja& addToA(Pareja &p);
};

// Sobrecarga del operador +
Pareja& operator +(const Pareja &p1,const Pareja &p2)
{

  return *(new Pareja(p1.a + p2.a, p1.b + p2.b) );
}

Pareja& Pareja::addToA(Pareja &p){
        this->a += p.a;
        p.a += this->a;
        return *this;
    }

int main()
{
    Pareja A(50, 75 );
    Pareja B(150, 175 );
    Pareja D(A);

    Pareja C = A + B;

    cout << "A = " << A.a << ',' << A.b << "\n";


    Pareja *E;
    E = &C.addToA(A);

    cout << "A = " << A.a << ',' << A.b << "\n";
    cout << "B = " << B.a << ',' << B.b << "\n";
    cout << "C = " << C.a << ',' << C.b << "\n";
    cout << "C = " << E << "\n";

    return 0;
}
