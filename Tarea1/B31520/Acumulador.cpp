// Acumulador.cpp (c) 2005 adolfo@di-mare.com

/** \file  Acumulador.cpp
    \brief Implementaciones para la clase \c "Acumulador".

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2008
*/

#include "Acumulador.h" // Jala las declaraciones del archivo de encabezado

/** Verifica la invariante de la clase \c Acumulador.
    - <code>(m_cantidad >= 0 ) && (m_menor <= m_mayor)</code>

    \par <em>Rep</em> Modelo de la clase
    \code
    +------------+
    | m_total    | <== Total acumulado hasta el momento.
    +------------+
    | m_cantidad | <== Número de valores agregados al acumulador.
    +------------+
    | m_menor    | <== Valor menor agregado hasta el momento.
    +------------+
    | m_mayor    | <== Valor mayor agregado hasta el momento.
    +------------+
    \endcode
*/
bool check_ok( const Acumulador& A ) {
    if ( ! (&A != 0) ) {
        return false; /// - Invariante: El objeto no puede estar almacenado en una posición nula.
    }
    if ( ! (A.m_menor <= A.m_mayor) ) {
        return false; /// - Invariante: El valor menor debe ser inferior al valor mayor.
    }
    if ( ! (A.m_cantidad >= 0) ) {
        return false; /// - Invariante: La cantidad de valores acumulados debe ser \c 0 o mayor.
    }
    if ( (A.m_cantidad == 0) && ( (A.m_total !=0) || (A.m_menor != 0) || (A.m_mayor != 0) ) ) {
        return false; /// - Invariante: Cantidad nula de valores acumulados con otros campos no nulos.
    }
    if ( (A.m_cantidad == 1) && ( (A.m_total != A.m_mayor) || (A.m_menor != A.m_mayor) ) ) {
        return false; /// - Invariante: Con un solo valor en el acumlador todos los campos deben tener el mismo valor.
    }
    return true;
} // check_ok()

// EOF: Acumulador.cpp
