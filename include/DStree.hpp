#ifndef DSTREE_H
#define DSTREE_H

#include <vector>

#include "Token.hpp"

typedef unsigned int LevelN ;
typedef int ID ;

enum CommandType {
  CONSTRUCTOR,
  QUOTE,
  DEFINE,
  PART_ACCESSOR,
  PRIMITIVE_PREDICATE,
  BASIC,
  EQIVALENCE,
  BEGIN,
  CONDITIONAL,
  ETC,
  NOOOOO
};

class Command {
public:
  Command() ;
  Command( TokenP token ) ;

  void SetType( CommandType type ) { m_type = type ; } // SetType()

  bool CheckType( CommandType type ) { return ( m_type == type ) ; } // CheckType()

  bool CheckCmd( string str ) { return ( m_name == str ) ; } // CheckCmd()

  string GetName() { return m_name ; }

protected:
private:
  CommandType m_type ;
  string m_name ;
};

typedef Command *CommandP ;

class DStree {
public:
  DStree() ;
  DStree( LevelN ln ) ;
  DStree( LevelN ln, TokenP token1, TokenP token2 ) ;

  void SetID( ID id ) { m_id = id ; } // SetID()

  void SetLevelN( LevelN ln ) { m_ln = ln ; } // SetLevelN()

  void SetCommand( CommandP cmd ) { m_command = cmd ; } // SetCommand()

  void SetDOT( bool isHaveDOT ) { m_isHaveDOT = isHaveDOT ; } // SetDOT()

  void SetToken( unsigned int i, TokenP tp ) { m_tokens[i] = tp ; } // SetToken()

  void SetDST( DStree * dst ) { m_dsts.push_back( dst ) ; } // SetDST()

  void SetDST( vector<DStree *> vdst ) { m_dsts = vdst ; } // SetDST()

  void SetDOTDST( DStree * dotdst ) { m_dotdst = dotdst ; } // SetDOTDST()

  // get

  ID GetID() { return m_id ; } // GetID()

  LevelN GetLevelN() { return m_ln ; } // GetLevelN()

  CommandP GetCommand() { return m_command ; } // GetCommand()

  bool GetDOT() { return m_isHaveDOT ; } // GetDOT()

  TokenP GetToken( unsigned int i ) { return m_tokens[i] ; } // GetToken()

  vector<DStree *> GetDSTs() { return m_dsts ; } // GetDSTs()

  DStree * GetDST( unsigned int i ) { return m_dsts[ i ] ; } // GetDSTs()

  unsigned int GetDSTsSize() { return m_dsts.size() ; } // GetDSTsSize()

  DStree * GetDOTDST() { return m_dotdst ; } // GetDOTDST()


  void EraseDST( unsigned int i ) { m_dsts.erase( m_dsts.begin() + i ) ; } // SetDST()

  void Copy( DStree * dst ) ;

  string Str() { return Str( false, 0 ) ; } // Str()

protected:
private:
  ID m_id ;
  LevelN m_ln ;
  CommandP m_command ;
  bool m_isHaveDOT ;
  TokenP m_tokens[ 2 ] ;
  vector<DStree *> m_dsts ;
  DStree * m_dotdst ;

  string Str( LevelN ln, bool isDOT ) ;
  string Str( LevelN ln )  ;
};

typedef DStree *DStreeP ;
typedef vector<DStree> DSTs ;
typedef vector<DStreeP> DSTPs ;

#endif // DSTREE_H
