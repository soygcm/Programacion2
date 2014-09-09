/* p2-ta-3.cpp  (c) 2013 adolfo@di-mare.com */

#include <iostream>     // cin+cout
#include <iomanip>      // setw()
#include <climits>      // INT_MAX
#include "rational.h"   // clase de n�meros racionales

int main(void) {
/*  resultado
    <><><><><><><><> Qu� hace el programa... */
/*  programador <><><><><><><><>
    Ponga aqu� su nombre y carnet
    <><><><><><><><> */

    rational<long> r,    // <><><><><><><><>
                   acum; // <><><><><><><><>
    char op;             // <><><><><><><><>

    // <><><><><><><><>
    acum = 0;

    // <><><><><><><><>
    std::cout << acum << " > ";
    std::cin  >> op;

    // <><><><><><><><>
    while (op != '.') {
        // <><><><><><><><>
        std::cin >> r;        // <><><><><><><><>

        // <><><><><><><><>
        switch (op) {
            case '+': acum.operator+=(r);         break;  /* acum += r; */
            case '-': acum  = operator-(acum,r);  break;  /* acum - r;  */
            case '*': acum *= r;          break;
            case '/': acum  = acum / r;   break;
        default:
            // <><><><><><><><>
            std::cout << "\n(" << op << ")==> Operaci�n inv�lida\n";
        }

        // <><><><><><><><>
        std::cout << acum << " > ";
        std::cin.ignore(INT_MAX, '\n');
        std::cin >> op;
    }

    // <><><><><><><><>
    return 0;
}  // main()

/* EOF: p2-ta-3.cpp */
