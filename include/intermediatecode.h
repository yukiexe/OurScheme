#ifndef INTERMEDIATECODE_H
#define INTERMEDIATECODE_H

#include <sstream>
#include <vector>
#include "Table.h"
#include "yukitool.h"

struct POS {
  unsigned int x ;
  unsigned int y ;
} ;

class IntermediateCode {
    public:
        IntermediateCode();
        IntermediateCode( Identifier id ) ;
        IntermediateCode( Token t1, Token t2, Token t3, Token t4  ) ;

        void SetCode( unsigned int tx, Token t ) ;
        void SetCode( unsigned int tx, POS p ) ;
        void SetStr() ;
        string Getcode() ;
        POS Getcode( unsigned int i ) { return m_code[i]; }
        string GetStr() { return m_str ; }

        bool CmpCode( unsigned int tx, POS pos ) ;
        POS TokentoPOS( Token token ) ;
        POS PointertoPOS( unsigned int tableN, unsigned int p ) ;
        string POStoS( POS pos ) ;
    protected:
    private:
        POS m_code[4];
        Token m_token[4];
        string m_str ;
};

typedef vector<IntermediateCode> IntermediateCodeTable ;

#endif // INTERMEDIATECODE_H
