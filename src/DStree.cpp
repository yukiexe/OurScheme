#include "..\include\DStree.h"

Command::Command() {
  SetType( NOOOOO ) ;
  m_name = "" ;
} // Command::Command()

Command::Command( TokenP token ) {
  m_name = token->GetName() ;
  if ( CheckCmd( "cons" ) || CheckCmd( "list" ) ) SetType( CONSTRUCTOR ) ;
  else if ( CheckCmd( "quote" ) || CheckCmd( "'" ) ) SetType( QUOTE ) ;
  else if ( CheckCmd( "define" ) ) SetType( DEFINE ) ;
  else if ( CheckCmd( "car" ) || CheckCmd( "cdr" ) ) SetType( PART_ACCESSOR ) ;
  else if ( CheckCmd( "pair?" ) || CheckCmd( "null?" ) ||
            CheckCmd( "integer?" ) || CheckCmd( "real?" ) ||
            CheckCmd( "number?" ) || CheckCmd( "string?" ) ||
            CheckCmd( "boolean?" ) || CheckCmd( "symbol?" ) )
    SetType( PRIMITIVE_PREDICATE ) ;
  else if ( CheckCmd( "+" ) || CheckCmd( "-" ) ||
            CheckCmd( "*" ) || CheckCmd( "/" ) ||
            CheckCmd( "not" ) || CheckCmd( "and" ) ||
            CheckCmd( "or" ) || CheckCmd( ">" ) ||
            CheckCmd( ">=" ) || CheckCmd( "<" ) ||
            CheckCmd( "<=" ) || CheckCmd( "=" ) ||
            CheckCmd( "string-append" ) || CheckCmd( "string>?" ) )
    SetType( BASIC ) ;
  else if ( CheckCmd( "eqv?" ) || CheckCmd( "equal?" ) ) SetType( EQIVALENCE ) ;
  else if ( CheckCmd( "begin" ) ) SetType( BEGIN ) ;
  else if ( CheckCmd( "if" ) || CheckCmd( "cond" ) ) SetType( CONDITIONAL ) ;
  else if ( CheckCmd( "clean-environment" ) || CheckCmd( "exit" ) ) SetType( ETC ) ;
  else SetType( NOOOOO ) ;
} // Command::Command()

DStree::DStree() {
  m_id = -1 ;
  m_ln = 0 ;
  m_command = NULL ;
  m_isHaveDOT = false ;
  m_tokens[ 0 ] = NULL ;
  m_tokens[ 1 ] = NULL ;
  m_dotdst = NULL ;
} // DStree::DStree()

DStree::DStree( LevelN ln ) {
  m_id = -1 ;
  m_ln = ln ;
  m_command = NULL ;
  m_isHaveDOT = false ;
  m_tokens[ 0 ] = NULL ;
  m_tokens[ 1 ] = NULL ;
  m_dotdst = NULL ;
} // DStree::DStree()

DStree::DStree( LevelN ln, TokenP token1, TokenP token2 ) {
  m_id = -1 ;
  m_ln = ln ;
  m_command = NULL ;
  m_isHaveDOT = false ;
  m_tokens[ 0 ] = token1 ;
  m_tokens[ 1 ] = token2 ;
  m_dotdst = NULL ;
} // DStree::DStree()


void DStree::Copy( DStree * dst ) {
  m_id = dst->GetID() ;
  m_ln = dst->GetLevelN() ;
  m_command = dst->GetCommand() ;
  m_isHaveDOT = dst->GetDOT() ;
  m_tokens[ 0 ] = dst->GetToken( 0 ) ;
  m_tokens[ 1 ] = dst->GetToken( 1 ) ;
  SetDST( dst->GetDSTs() ) ;
  m_dotdst = dst->GetDOTDST() ;
} // DStree::Copy()


string DStree::Str( LevelN ln, bool isDOT ) {
  stringstream ss ( stringstream::in | stringstream::out ) ;
  unsigned int spacesN = ( ln + 1 ) * 2 ;

  if ( GetCommand() != NULL ) return Str( ln ) ;

  if ( GetToken( 0 ) != NULL ) {
    if ( !isDOT || ! GetToken( 0 )->CheckType( LEFTPAREN ) )
      ss << GetToken( 0 )->Str() ;
  } // if

  for ( unsigned int i = 0 ; i < m_dsts.size() ; ++i ) {
    if ( i == 0 && GetToken( 0 )->CheckType( LEFTPAREN ) && !isDOT ) ss << " " ;
    else ss << string( spacesN, ' ' ) ;

    ss << GetDST( i )->Str( ln + 1, false ) << endl ;
  } // for

  if ( GetDOT() ) {
    if ( ! m_dotdst->GetToken( 0 )->CheckType( NIL ) ) {
      if ( m_dotdst->GetToken( 0 )->CheckType( LEFTPAREN ) )
        ss << m_dotdst->Str( ln, true ) ;
      else {
        // for ( unsigned int j = 0 ; j < spacesN ; ++j ) ss << " " ;
        ss << string( spacesN, ' ' ) << "." << endl ;
        ss << string( spacesN, ' ' ) << m_dotdst->Str( ln + 1, false  ) << endl ;
      } // else
    } // if
  } // if

  if ( GetToken( 1 ) != NULL &&
       ( !isDOT || !GetToken( 0 )->CheckType( LEFTPAREN ) ) ) {
    spacesN = ln * 2 ;
    ss << string( spacesN, ' ' ) << GetToken( 1 )->Str() ;
  } // if

  return ss.str() ;
} // DStree::Str()

string DStree::Str( LevelN ln ) {
  stringstream ss ( stringstream::in | stringstream::out ) ;
  if ( m_command->CheckType( DEFINE ) ) ss << m_tokens[ 0 ]->Str() << " defined" ;
  else if ( m_command->CheckCmd( "clean-environment" ) ) ss << "environment cleaned" ;
  else if ( m_command->CheckType( QUOTE ) ) {
    DStreeP nextDST = new DStree( ln ) ;
    DStreeP nextDST2 = new DStree( ln + 1 ) ;
    nextDST->SetToken( 0, new Token( LEFTPAREN ) ) ;
    nextDST->SetToken( 1, new Token( RIGHTPAREN ) ) ;
    nextDST2->SetToken( 0, new Token( SYMBOL, "quote" ) ) ;
    nextDST->SetDST( nextDST2 ) ;
    nextDST->SetDST( GetDSTs()[ 0 ] ) ;
    ss << nextDST->Str( ln, false ) ;
  } // else if
  else if ( m_command->CheckCmd( "lambda" ) ) ss << "#function" ;


  return ss.str() ;
} // DStree::Str()
