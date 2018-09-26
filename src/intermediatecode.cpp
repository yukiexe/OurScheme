#include "intermediatecode.hpp"

IntermediateCode::IntermediateCode() {
  // ctor
}

IntermediateCode::IntermediateCode( Identifier id ) {
  POS pos ;
  if ( id.idtype == CALLID ) {
    pos.x = 2 ;
    pos.y = 3 ;
    m_code[0] = pos ;
    m_code[1] = TokentoPOS( id.token ) ;
    pos.x = pos.y = 5566 ;
    m_code[2] = pos ;
    m_code[3] = PointertoPOS( 7, id.pointer ) ;
    m_str = "CALL  " + id.token.name + '(' +')' ;
  } // if
  else if ( id.idtype == GOTOID ) {
    pos.x = 2 ;
    pos.y = 11 ;
    m_code[0] = pos ;
    pos.x = pos.y = 5566 ;
    m_code[1] = m_code[2] = pos ;
    m_code[3] = PointertoPOS( 6, id.pointer ) ;
    m_str = "GTO " + id.token.name ;
  } // else if
  else {
    m_code[0] = TokentoPOS( id.token ) ;
    pos.x = pos.y = 5566 ;
    m_code[1] = m_code[2] = m_code[3] = pos ;

    m_str = id.token.name ;
  } // else
} // IntermediateCode( Identifier id )

IntermediateCode::IntermediateCode( Token t1, Token t2, Token t3, Token t4  ) {
    m_code[0] = TokentoPOS( m_token[0] = t1 );
    m_code[1] = TokentoPOS( m_token[1] = t2 );
    m_code[2] = TokentoPOS( m_token[2] = t3 );
    m_code[3] = TokentoPOS( m_token[3] = t4 );
    if ( m_code[0].x == 1 && m_code[0].y == 4 && m_code[2].x == 5566 ) {
      m_str = m_token[3].name + ' ' +
               m_token[0].name + ' ' +
               m_token[1].name + ' ' ;
    } // if
    else if ( m_code[0].x == 1 && m_code[0].y == 4 ) {
      m_str = m_token[3].name + ' ' +
               m_token[0].name + ' ' +
               m_token[1].name + '(' + m_token[2].name + ')' ;
    } // elseif
    else if ( m_code[0].x == 2 && m_code[0].y == 12 ) {
      m_str = m_token[0].name + " " + m_token[1].name +
              " GO TO " + YukiTool::convertInt( m_code[2].y )  ;
    } // else if
    else
      m_str = m_token[3].name + ' ' + '=' + ' ' +
              m_token[1].name + ' ' + m_token[0].name + ' ' + m_token[2].name ;

} // IntermediateCode()

void IntermediateCode::SetCode( unsigned int tx, Token t ) {
  m_code[tx] = TokentoPOS( t ) ;
} // SetCode()

void IntermediateCode::SetCode( unsigned int tx, POS p ) {
  m_code[tx] = p ;
} // SetCode()

void IntermediateCode::SetStr() {
  if ( m_code[0].x == 2 && m_code[0].y == 12 && m_str.size() > 0 )
    m_str += ", ELSE GO TO " + YukiTool::convertInt( m_code[3].y )  ;
} // SetStr()

string IntermediateCode::Getcode() {
  stringstream ss ;
  ss << '(' ;
  for ( unsigned int i = 0 ; i < 4 ; ++i )
    ss << POStoS( m_code[i] ) << ( ( i != 3 ) ? ',' : ')' ) ;
  ss << "  " << m_str << endl ;
  return ss.str() ;
} // Getcode()

bool IntermediateCode::CmpCode( unsigned int tx, POS pos ) {
  return ( m_code[tx].x == pos.x && m_code[tx].y == pos.y ) ;
} // CmpCode()

POS IntermediateCode::TokentoPOS( Token token ) {
  POS pos ;
  pos.x = token.groupN ;
  pos.y = token.index ;
  return pos ;
} // TokentoPOS()

POS IntermediateCode::PointertoPOS( unsigned int tableN, unsigned int p ) {
  POS pos ;
  pos.x = tableN ;
  pos.y = p ;
  return pos ;
} // PointertoPOS()

string IntermediateCode::POStoS( POS pos ) {
  stringstream ss ;
  if ( pos.x != 5566 ) {
    ss << '(' ;
    if ( pos.x < 10 ) ss << ' ' ;
    ss << pos.x << ',' ;
    if ( pos.y < 10 ) ss << ' ' ;
    ss << pos.y << ')' ;
  } // if
  else
    ss << "       " ;
  return ss.str() ;
} // TtoS()
