// StyleCheckType string, vector, stringstream
// OurScheme Project3
# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include "Token.hpp"
# include "DStree.hpp"
# include "intermediatecode.hpp"

using namespace std ;

// Tokne.h
// Tokne.cpp

// DStree.h
// DStree.cpp

class Defined {
public:
  Defined() { m_isFunc = false ; }

  Defined( bool isFunc, TokenP symbol, DStreeP dst ) {
    m_isFunc = isFunc ;
    SetSymbol( symbol ) ;
    SetDST( dst ) ;
  } // Defined()

  void SetSymbol( TokenP symbol ) { m_symbols.push_back( symbol ) ; } // SetSymbol()

  void SetSymbol( unsigned int i, TokenP symbol ) { m_symbols[ i ] = symbol ; } // SetSymbol()

  void SetName( TokenP symbol ) { m_symbols[ 0 ] = symbol ; } // SetName()

  void SetDST( DStreeP dst ) { m_dst.push_back( dst ) ; } // SetDST()

  void SetID( ID id ) { m_dst[ 0 ]->SetID( id ) ; } // SetID()

  // bool CheckType( CommandType type ) { return ( m_type == type ) ; } // CheckType()

  bool IsFunc() { return m_isFunc ; } // IsFunc()

  bool CheckSymbol( TokenP symbol ) { return m_symbols[ 0 ]->CheckToken( symbol ) ; } // CheckSymbol()

  string GetName() { return m_symbols[ 0 ]->GetName() ; }

  unsigned int GetSymbolsSize() { return m_symbols.size() ; }

  TokenP GetSymbol( unsigned int i ) { return m_symbols[ i ] ; }

  TokenPs GetSymbols() { return m_symbols ; }

  unsigned int GetDSTsSize() { return m_dst.size() ; }

  DStreeP GetDST() { return m_dst[ 0 ] ; }

  DStreeP GetDST( unsigned int i ) { return m_dst[ i ] ; }

  // vector<DStreeP> GetDSTs() { return m_dst ; }


  void Copy( Defined * def ) {
    m_isFunc = def->IsFunc() ;
    m_symbols = def->GetSymbols() ;
    SetDST( def->GetDST() ) ; // Pointer
  } // Copy()

protected:
private:
  bool m_isFunc ;
  TokenPs m_symbols ;
  DSTPs m_dst ;
};

typedef Defined *DefinedP ;
typedef vector<DefinedP> Defineds ;

class DefinedTable {
public:
  DefinedTable() { } // DefinedTable()

  void SetDefined( DefinedP defined ) {
    TokenP symbol = defined->GetSymbol( 0 ) ;
    for ( unsigned int i = 0 ; i < m_defineds.size() ; ++i ) {
      if ( m_defineds[ i ]->CheckSymbol( symbol ) ) {
        defined->SetID( m_defineds[ i ]->GetDST()->GetID() ) ;
        m_defineds[ i ] = defined ;
        return ;
      } // if
    } // for

    defined->SetID( m_defineds.size() ) ;
    m_defineds.push_back( defined ) ;
  } // SetDefined()

  Defineds GetDefineds() { return m_defineds ; } // GetDefineds()

  DefinedP GetDefined( unsigned int i ) { return m_defineds[ i ] ; } // GetDefineds()

  bool CheckSymbol( TokenP symbol, DefinedP &defined ) {
    for ( unsigned int i = 0 ; i < m_defineds.size() ; ++i ) {
      if ( m_defineds[ i ]->CheckSymbol( symbol ) ) {
        defined = m_defineds[ i ] ;
        return true ;
      } // if
    } // for

    defined = NULL ;
    return false ;
  } // CheckSymbol()

  bool SetAliasFunc( TokenP aliasSymbol, DStreeP dst ) {
    TokenP symbol = dst->GetToken( 0 ) ;
    if ( symbol == NULL ) return false ;
    else {
      DefinedP defined = NULL ;
      if ( CheckSymbol( symbol, defined ) ) {
        DefinedP tempDef = new Defined() ;
        tempDef->Copy( defined ) ;
        tempDef->SetName( aliasSymbol ) ;
        SetDefined( tempDef ) ;
        return true ;
      } // if
      else if ( symbol->CheckCommandType() ) {
        SetDefined( new Defined( true, aliasSymbol, dst ) ) ;
        return true ;
      } // else if
      else return false ;
    } // else
  } // SetAliasFunc()

  void Insert( DefinedTable * defTable ) {
    Defineds defs( defTable->GetDefineds() ) ;
    m_defineds.insert( m_defineds.end(), defs.begin(), defs.end() ) ;
  } // Insert()

  void Clear() { m_defineds.clear() ; } // Clear()

protected:
private:
  Defineds m_defineds ;
};

typedef DefinedTable * DefinedTableP ;

unsigned int gtestN ;
bool gIsSkip ;
bool gIsQ ;
LineCounter gLC ;
CommandP gCmd ;
TokenP gNextT ;
DefinedTable gDefinedTable ;

DefinedP gNextDefined ;

bool SExp( DStreeP &dst ) ;
bool ATOM( TokenP &token, bool &gotLEFTPAREN ) ;
bool CommandExp( DStreeP &dst ) ;
bool SExpDOT( DStreeP &dst ) ;

// bool EvalSExp( DStreeP indst, DStreeP &outdst ) ;
void PredicateCheck( bool isIt, DStreeP &resultDST ) ;

void Initialization() {
  cin >> gtestN ;
  cout << "Welcome to OurScheme!\n" << endl ;
  // cin.get() ;
} // Initialization()

void Reset( DStreeP &dst, DStreeP &resultdst ) {
  dst = new DStree() ;
  resultdst = NULL ;
  gLC.Reset( gIsSkip ) ;
  gIsSkip = false ;
  gIsQ = false ;
  gNextT = NULL ;
  gNextDefined = NULL ;
  cout << "> " ;
} // Reset()

void Skip() {
  gIsSkip = true ;
  while ( cin.peek() != '\n' && cin.peek() != -1 )
    cin.get() ;
  if ( cin.peek() == '\n' ) cin.get() ;
} // Skip()

void CheckgNextT( TokenP &token ) {
  if ( gNextT == NULL ) token = new Token( gLC ) ;
  else {
    token = gNextT ;
    gNextT = NULL ;
  } // else
} // CheckgNextT()

bool CheckNextT( TokenP &token, TokenType type ) {
  CheckgNextT( token ) ;
  if ( token->CheckType( type ) ) return true ;
  else gNextT = token ;

  return false ;
} // CheckNextT()

void Output( string str ) { cout << str << endl << endl ; } // Output()

void Output( DStreeP dst ) { Output( dst->Str() ) ; } // Output()

bool SExp( DStreeP &dst ) { // <S-exp>
  TokenP token ;

  if ( CheckNextT( token, COMMAND ) ) {
    CommandP cmd = new Command( token ) ;

    if ( cmd->CheckCmd( "'" ) ) {
      dst->SetCommand( cmd ) ;

      bool isgIsQOn = gIsQ ;
      gIsQ = true ;

      DStreeP nextDST = new DStree( dst->GetLevelN() + 1 ) ;
      if ( ! SExp( nextDST ) ) throw 1 ;
      dst->SetDST( nextDST ) ;

      if ( !isgIsQOn ) gIsQ = false ;

      return true ;
    } // if
    else gNextT = token ;

  } // if

  bool gotLEFTPAREN = false ;
  if ( ATOM( token, gotLEFTPAREN ) ) {
    dst->SetToken( 0, token ) ;
    return true ;
  } // if
  else if ( gotLEFTPAREN ) {
    dst->SetToken( 0, token ) ;

    if ( gIsQ || !CommandExp( dst ) ) {
      LevelN newLN = dst->GetLevelN() + 1 ;
      DStreeP nextDST = new DStree( newLN ) ;
      if ( ! SExp( nextDST ) ) throw 1 ;
      do {
        dst->SetDST( nextDST ) ;
        nextDST = new DStree( newLN ) ;
      } while ( SExp( nextDST ) ) ;

      dst->SetDOT( SExpDOT( dst ) ) ; // [ DOT <S-exp> ]
    } // if

    if ( ! CheckNextT( token, RIGHTPAREN ) ) throw 2 ;
    else dst->SetToken( 1, token ) ;

    return true ;
  } // else if

  return false ;
} // SExp()

bool ATOM( TokenP &token, bool &gotLEFTPAREN ) {
  CheckgNextT( token ) ;
  if ( token->CheckType( SYMBOL ) ) return true ;
  else if ( token->CheckType( INT ) || token->CheckType( FLOAT ) ) return true ;
  else if ( token->CheckType( STRING ) ) return true ;
  else if ( token->CheckType( NIL ) ) return true ;
  else if ( token->CheckType( T ) ) return true ;
  else if ( gIsQ && token->CheckType( COMMAND ) ) {
    token->SetType( SYMBOL ) ;
    return true ;
  } // else if
  else if ( token->CheckType( COMMAND ) ) {
    return true ;
  } // else if
  else if ( token->CheckType( LEFTPAREN ) ) {
    gotLEFTPAREN = true ;
    TokenP nextT ;
    if ( CheckNextT( nextT, RIGHTPAREN ) ) {
      token->SetType( NIL ) ;
      return true ;
    } // if
    else return false ;

  } // else if
  else {
    gNextT = token ;
    return false ;
  } // else

} // ATOM()

bool CommandExp( DStreeP &dst ) {
  TokenP token ;
  if ( ! CheckNextT( token, COMMAND ) ) return false ;
  CommandP cmd = new Command( token ) ;

  if ( cmd->CheckCmd( "'" ) ) {
    gNextT = token ;
    return false ;
  } // if
  else dst->SetCommand( cmd ) ;

  bool isgIsQOn = gIsQ ;
  if ( cmd->CheckCmd( "quote" ) ) gIsQ = true ;

  LevelN newLN = dst->GetLevelN() + 1 ;
  DStreeP nextDST = new DStree( newLN ) ;
  while ( SExp( nextDST ) ) {
    dst->SetDST( nextDST ) ;
    nextDST = new DStree( newLN ) ;
  } // while

  dst->SetDOT( SExpDOT( dst ) ) ; // [ DOT <S-exp> ]

  if ( cmd->CheckCmd( "quote" ) )
    if ( !isgIsQOn ) gIsQ = false ;


  return true ;
} // CommandExp()

bool SExpDOT( DStreeP &dst ) {
  TokenP token ;
  if ( CheckNextT( token, DOT ) ) {
    DStreeP nextDST = new DStree( dst->GetLevelN() + 1 ) ;
    if ( ! SExp( nextDST ) ) throw 1 ;
    else dst->SetDOTDST( nextDST ) ;

    return true ;
  } // if
  else return false ;
} // SExpDOT()


void LocalDefinedTableProcess( DefinedTableP defTable, DefinedTableP nextDefTable ) {
  nextDefTable->Insert( defTable ) ;

} // LocalDefinedTableProcess()

bool EvalSExp( DStreeP indst, DStreeP &outdst, DefinedTableP defTable ) {
  LevelN ln = indst->GetLevelN() ;
  DStreeP nextDST ;
  DStreeP tempDST ;
  DStreeP tempDST2 ;

  if ( indst->GetCommand() == NULL ) {
    if ( indst->GetToken( 0 ) != NULL ) {
      if ( indst->GetToken( 0 )->CheckType( LEFTPAREN ) ) {
        outdst = new DStree( ln, new Token( LEFTPAREN ), new Token( RIGHTPAREN ) ) ;

        // ckeck Func
        EvalSExp( indst->GetDST( 0 ), tempDST, defTable ) ; // tempDST ���

        if ( gNextDefined == NULL ) throw tempDST->GetToken( 0 ) ;
        else if ( ! gNextDefined->IsFunc() ) throw gNextDefined->GetDST()->GetToken( 0 ) ;
          // EvalSExp( gNextDefined->GetDST(), tempDST, defTable ) ;

        // bool isLambda = false ;
        if ( gNextDefined->GetDST()->GetCommand() != NULL &&
             gNextDefined->GetDST()->GetCommand()->CheckCmd( "lambda" ) ) {
          // isLambda = true ;
          EvalSExp( gNextDefined->GetDST(), tempDST, defTable ) ;
        } // if

        DefinedP def = gNextDefined ;
        gNextDefined = NULL ;


        if ( def->GetDST()->GetToken( 0 )->CheckType( COMMAND ) ) {
          nextDST = new DStree( ln, new Token( LEFTPAREN ), new Token( RIGHTPAREN ) ) ;
          // nextDST->Copy( tempDST ) ;
          nextDST->SetCommand( new Command( def->GetDST()->GetToken( 0 ) ) ) ;
          vector<DStreeP> dst = indst->GetDSTs() ;
          vector<DStreeP> dst2( dst.begin() + 1, dst.end() ) ;
          nextDST->SetDST( dst2 ) ;
          nextDST->SetDOT( indst->GetDOT() ) ;
          nextDST->SetDOTDST( indst->GetDOTDST() ) ;
          EvalSExp( nextDST, outdst, defTable ) ;
          return true ;
        } // if

        // error (incorrect number of arguments)
        // if ( isLambda )

        if ( def->GetSymbolsSize() && indst->GetDSTsSize() != def->GetSymbolsSize() )
          throw new Command( def->GetSymbol( 0 ) ) ;

        DefinedTableP nextDefTable = new DefinedTable() ;
        for ( unsigned int i = 1 ; i < indst->GetDSTsSize() ; ++i ) {
          EvalSExp( indst->GetDST( i ), tempDST, defTable ) ;
          nextDefTable->SetDefined( new Defined( false, def->GetSymbol( i ), tempDST ) ) ;
        } // for

        // LocalDefinedTableProcess( defTable, nextDefTable ) ;

        // �ǤJ�w�q��A����Func
        for ( unsigned int i = 0 ; i < def->GetDSTsSize() ; ++i )
          EvalSExp( def->GetDST( i ), outdst, nextDefTable ) ;

        return true ;
        /*
        if ( indst->GetDOT() ) {
          outdst->SetDOT( true ) ;
          EvalSExp( indst->GetDOTDST(), tempDST, defTable ) ;
          outdst->SetDOTDST( tempDST ) ;
        } // if
        */

      } // if
      else if ( indst->GetToken( 0 )->CheckType( SYMBOL ) ) {
        if ( defTable->CheckSymbol( indst->GetToken( 0 ), gNextDefined ) ||
             gDefinedTable.CheckSymbol( indst->GetToken( 0 ), gNextDefined ) ) {
          if ( indst->GetLevelN() == 0 && gNextDefined->GetSymbolsSize() > 1 ) {
            gNextDefined = NULL ;
            outdst = new DStree( indst->GetLevelN(),
                                 new Token( COMMAND, indst->GetToken( 0 )->GetName() ),
                                 indst->GetToken( 1 ) ) ;
          } // if
          else outdst = gNextDefined->GetDST() ;
        } // if
        else throw indst->GetToken( 0 ) ;

      } // else if
      else outdst = indst ;
    } // if
    else outdst = indst ;

    return true ;
  } // if
  else {
    CommandP cmd = indst->GetCommand() ;
    if ( cmd->CheckType( CONSTRUCTOR ) ) {
      outdst = new DStree( ln, new Token( LEFTPAREN ), new Token( RIGHTPAREN ) ) ;

      if ( cmd->CheckCmd( "cons" ) ) {
        if ( indst->GetDSTsSize() != 2 ) throw cmd ;

        outdst->SetDOT( true ) ;

        EvalSExp( indst->GetDST( 0 ), tempDST, defTable ) ;
        outdst->SetDST( tempDST ) ;

        EvalSExp( indst->GetDST( 1 ), tempDST, defTable ) ;
        outdst->SetDOTDST( tempDST ) ;
      } // if
      else if ( cmd->CheckCmd( "list" ) ) { // non?
        for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
          EvalSExp( indst->GetDST( i ), tempDST, defTable ) ;
          outdst->SetDST( tempDST ) ;
        } // for

      } // else if

      if ( outdst->GetDSTsSize() == 0 ) {
        outdst = new DStree( ln ) ;
        outdst->SetToken( 0, new Token( NIL ) ) ;
      } // if

      return true ;
    } // if
    else if ( cmd->CheckType( QUOTE ) ) {
      if ( indst->GetDSTsSize() != 1 ) throw cmd ;
      if ( indst->GetDST( 0 )->GetToken( 0 )->CheckType( SYMBOL ) )
        indst->GetDST( 0 )->GetToken( 0 )->SetType( NOTHING ) ;
      outdst = indst->GetDST( 0 ) ;
      return true ;
    } // else if
    else if ( cmd->CheckType( DEFINE ) ) { // return Eval???
      if ( indst->GetLevelN() != 0 ) throw string( "define format" ) ;
      else if ( indst->GetDSTsSize() != 2 ) throw string( "define format" ) ;

      nextDST = indst->GetDST( 0 ) ;

      // �p�G�w�w�q������B�z
      /*
      if ( nextDST->GetCommand() != NULL ) EvalSExp( nextDST, tempDST, defTable ) ;
      else tempDST = nextDST ;

      nextDST = tempDST ;
      */

      tempDST = ( nextDST->GetDSTsSize() > 0 ) ? nextDST->GetDST( 0 ) : nextDST ;

      if ( ! tempDST->GetToken( 0 )->CheckType( SYMBOL ) ||
           tempDST->GetToken( 0 )->CheckCommandType() ) throw string( "define format" ) ;

      for ( unsigned int i = 0 ; i < nextDST->GetDSTsSize() ; ++i ) {
        if ( ! nextDST->GetDST( i )->GetToken( 0 )->CheckType( SYMBOL ) ||
             nextDST->GetDST( i )->GetToken( 0 )->CheckCommandType() )
          throw string( "define format" ) ;
      } // for


      TokenP name = tempDST->GetToken( 0 ) ;
      if ( nextDST->GetDSTsSize() > 0 ) {
        DefinedP defined = new Defined( true, name, indst->GetDST( 1 ) ) ;
        for ( unsigned int i = 1 ; i < nextDST->GetDSTsSize() ; ++i )
          defined->SetSymbol( nextDST->GetDST( i )->GetToken( 0 ) ) ;

        gDefinedTable.SetDefined( defined ) ;
      } // if
      else if ( ! gDefinedTable.SetAliasFunc( name, indst->GetDST( 1 ) ) ) {
        if ( indst->GetDST( 1 )->GetCommand() != NULL && indst->GetDST( 1 )->GetCommand()->CheckCmd( "lambda" ) )
          tempDST2 = indst->GetDST( 1 ) ;
        else EvalSExp( indst->GetDST( 1 ), tempDST2, defTable ) ;
        gDefinedTable.SetDefined( new Defined( false, name, tempDST2 ) ) ;
      } // else if


      // ��X��
      outdst = indst ;
      outdst->SetToken( 0, tempDST->GetToken( 0 ) ) ;
      return true ;
    } // else if
    else if ( cmd->CheckType( PART_ACCESSOR ) ) {
      if ( indst->GetDSTsSize() != 1 ) throw cmd ;
      else if ( EvalSExp( indst->GetDST( 0 ), nextDST, defTable ) ) {

        if ( gNextDefined != NULL && gNextDefined->GetSymbolsSize() > 1 ) {
          gCmd = cmd ;
          nextDST = new DStree() ;
          nextDST->SetCommand( new Command( new Token( SYMBOL, "lambda" ) ) ) ;
          throw nextDST ;
        } // if

        if ( nextDST->GetDSTsSize() < 1 ) {
          gCmd = cmd ;
          throw nextDST ;
        } // if

        if ( cmd->CheckCmd( "car" ) ) {
          if ( nextDST->GetDSTsSize() > 0 )
            outdst = nextDST->GetDST( 0 ) ;
          else
            outdst = nextDST ;
        } // if
        else if ( cmd->CheckCmd( "cdr" ) ) {
          if ( nextDST->GetDSTsSize() > 1 ) {
            vector<DStreeP> dsts = nextDST->GetDSTs() ;
            vector<DStreeP> dsts2( dsts.begin() + 1, dsts.end() ) ;
            outdst = new DStree( ln, new Token( LEFTPAREN ), new Token( RIGHTPAREN ) ) ;

            outdst->SetDST( dsts2 ) ;
            outdst->SetDOT( nextDST->GetDOT() ) ;
            outdst->SetDOTDST( nextDST->GetDOTDST() ) ;
          } // if
          else {
            if ( nextDST->GetDSTsSize() == 0 ) outdst = nextDST ;
            else if ( nextDST->GetDSTsSize() == 1 ) {
              if ( nextDST->GetDOT() ) outdst = nextDST->GetDOTDST() ;
              else outdst = new DStree( ln, new Token( NIL ), NULL ) ;
            } // else if
            else throw "error" ;

          } // else
        } // else if
      } // else if

      return true ;
    } // else if
    else if ( cmd->CheckType( PRIMITIVE_PREDICATE ) ) {
      if ( indst->GetDSTsSize() != 1 ) throw cmd ; // ?

      if ( EvalSExp( indst->GetDST( 0 ), tempDST, defTable ) ) {
        TokenP token = tempDST->GetToken( 0 ) ;
        outdst = new DStree( ln ) ;

        if ( cmd->CheckCmd( "pair?" ) ) // tempDST->GetDSTsSize() >= 2 || tempDST->GetDOT()
          PredicateCheck( tempDST->GetDSTsSize() != 0, outdst ) ;
        else if ( cmd->CheckCmd( "null?" ) ) // || token->CheckType( T )
          PredicateCheck( ( token->CheckType( NIL ) ), outdst ) ;
        else if ( cmd->CheckCmd( "integer?" ) )
          PredicateCheck( token->CheckType( INT ), outdst ) ;
        else if ( cmd->CheckCmd( "real?" ) )
          PredicateCheck( token->CheckType( FLOAT ), outdst ) ;
        else if ( cmd->CheckCmd( "number?" ) )
          PredicateCheck( token->CheckType( INT ) || token->CheckType( FLOAT ), outdst ) ;
        else if ( cmd->CheckCmd( "string?" ) )
          PredicateCheck( token->CheckType( STRING ), outdst ) ;
        else if ( cmd->CheckCmd( "boolean?" ) )
          PredicateCheck( ( token->CheckType( NIL ) || token->CheckType( T ) ), outdst ) ;
        else if ( cmd->CheckCmd( "symbol?" ) )
          PredicateCheck( token->CheckType( SYMBOL ), outdst ) ;

        return true ;
      } // if
    } // else if
    else if ( cmd->CheckType( BASIC ) ) {
      if ( cmd->CheckCmd( "not" ) ) {
        if ( indst->GetDSTsSize() != 1 ) throw cmd ;
      } // if
      else if ( indst->GetDSTsSize() < 2 ) throw cmd ;

      vector<DStreeP> dsts ;
      for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
        EvalSExp( indst->GetDST( i ), tempDST, defTable ) ;
        dsts.push_back( tempDST ) ;
      } // for

      stringstream ss ( stringstream::in | stringstream::out ) ;
      outdst = new DStree( ln ) ;
      if ( cmd->CheckCmd( "+" ) || cmd->CheckCmd( "-" ) ||
           cmd->CheckCmd( "*" ) || cmd->CheckCmd( "/" ) ) {
        float total = 0 ;
        float x = 0 ;

        bool isFloat = false ;
        for ( unsigned int i = 0 ; i < dsts.size() ; ++i ) {
          if ( ! dsts[ i ]->GetToken( 0 )->CheckType( FLOAT ) &&
               ! dsts[ i ]->GetToken( 0 )->CheckType( INT ) ) {
            gCmd = cmd ;
            throw dsts[ i ] ;
          } // if

          if ( dsts[ i ]->GetToken( 0 )->CheckType( FLOAT ) ) isFloat = true ;
          ss << dsts[ i ]->GetToken( 0 )->GetName() ;
          ss >> x ;
          ss.clear() ;
          ss.str( "" ) ;
          if ( i == 0 ) total = x ;
          else {
            if ( cmd->CheckCmd( "+" ) ) total += x ;
            else if ( cmd->CheckCmd( "-" ) ) total -= x ;
            else if ( cmd->CheckCmd( "*" ) ) total *= x ;
            else if ( cmd->CheckCmd( "/" ) ) total /= x ;
          } // else
        } // for

        ss.clear() ;
        ss.str( "" ) ;
        ss << total ;


        if ( isFloat )
          outdst->SetToken( 0, new Token( FLOAT, ss.str() ) ) ;
        else
          outdst->SetToken( 0, new Token( INT, ss.str() ) ) ;
      } // if
      else if ( cmd->CheckCmd( "not" ) || cmd->CheckCmd( "and" ) || cmd->CheckCmd( "or" ) ) {
        if ( cmd->CheckCmd( "not" ) )
          PredicateCheck( ! dsts[ 0 ]->GetToken( 0 )->GetBool(), outdst ) ;
        else { // cmd->CheckCmd( "and" ) || cmd->CheckCmd( "or" )
          bool x[ 2 ] = { dsts[ 0 ]->GetToken( 0 )->GetBool(), dsts[ 1 ]->GetToken( 0 )->GetBool() } ;
          if ( cmd->CheckCmd( "and" ) )
            PredicateCheck( x[ 0 ] && x[ 1 ], outdst ) ;
          else PredicateCheck( x[ 0 ] || x[ 1 ], outdst ) ;
        } // else
      } // else if
      else if ( cmd->CheckCmd( ">" ) || cmd->CheckCmd( ">=" ) ||
                cmd->CheckCmd( "<" ) || cmd->CheckCmd( "<=" ) ||
                cmd->CheckCmd( "=" ) ) {
        vector<float> x ;
        float temp ;
        for ( unsigned int i = 0 ; i < dsts.size() ; ++i ) {
          ss << dsts[ i ]->GetToken( 0 )->GetName() ;
          ss >> temp ;
          x.push_back( temp ) ;
          ss.clear() ;
          ss.str( "" ) ;
        } // for

        bool result = true ;
        if ( cmd->CheckCmd( ">" ) ) {
          for ( unsigned int i = 0 ; i < x.size() - 1 ; ++i )
            result = result && ( x[ i ] > x[ i + 1 ] ) ;
        } // if
        else if ( cmd->CheckCmd( ">=" ) )
          for ( unsigned int i = 0 ; i < x.size() - 1 ; ++i )
            result = result && ( x[ i ] >= x[ i + 1 ] ) ;

        else if ( cmd->CheckCmd( "<" ) )
          for ( unsigned int i = 0 ; i < x.size() - 1 ; ++i )
            result = result && ( x[ i ] < x[ i + 1 ] ) ;

        else if ( cmd->CheckCmd( "<=" ) )
          for ( unsigned int i = 0 ; i < x.size() - 1 ; ++i )
            result = result && ( x[ i ] <= x[ i + 1 ] ) ;

        else if ( cmd->CheckCmd( "=" ) )
          for ( unsigned int i = 0 ; i < x.size() - 1 ; ++i )
            result = result && ( x[ i ] == x[ i + 1 ] ) ;

        PredicateCheck( result, outdst ) ;
      } // else if
      else if ( cmd->CheckCmd( "string-append" ) ) {
        string str ;
        ss << '\"' ;
        for ( unsigned int i = 0 ; i < dsts.size() ; ++i ) {
          if ( ! dsts[ i ]->GetToken( 0 )->CheckType( STRING ) ) {
            gCmd = cmd ;
            throw dsts[ i ] ;
          } // if

          str = dsts[ i ]->GetToken( 0 )->GetName() ;
          ss << str.substr( 1, str.size() - 2 ) ;
        } // for

        ss << '\"' ;

        outdst->SetToken( 0, new Token( STRING, ss.str() ) ) ;
      } // else if
      else if ( cmd->CheckCmd( "string>?" ) ) {
        vector<string> strs ;
        string str ;
        for ( unsigned int i = 0 ; i < dsts.size() ; ++i ) {
          if ( ! dsts[ i ]->GetToken( 0 )->CheckType( STRING ) ) {
            gCmd = cmd ;
            throw dsts[ i ] ;
          } // if

          str = dsts[ i ]->GetToken( 0 )->GetName() ;
          strs.push_back( str.substr( 1, str.size() - 2 ) ) ;
        } // for

        for ( unsigned int i = 0 ; i < strs.size() - 1 ; ++i ) {
          if ( ( strs[ i ] < strs[ i + 1 ] ) ) {
            PredicateCheck( false, outdst ) ;
            return true ;
          } // if
        } // for

        PredicateCheck( true, outdst ) ;
      } // else if

      return true ;
    } // else if
    else if ( cmd->CheckType( EQIVALENCE ) ) {
      if ( indst->GetDSTsSize() != 2 ) throw cmd ;

      vector<DStreeP> dsts ;
      for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
        EvalSExp( indst->GetDST( i ), tempDST, defTable ) ;
        dsts.push_back( tempDST ) ;
      } // for

      outdst = new DStree( ln ) ;
      if ( cmd->CheckCmd( "eqv?" )  ) {
        if ( dsts[ 0 ]->GetID() == -1 && dsts[ 1 ]->GetID() == -1 ) {
          if ( dsts[ 0 ]->GetToken( 0 )->CheckType( STRING ) ||
               dsts[ 1 ]->GetToken( 0 )->CheckType( STRING ) ||
               dsts[ 0 ]->GetToken( 0 )->CheckType( LEFTPAREN ) ||
               dsts[ 1 ]->GetToken( 0 )->CheckType( LEFTPAREN ) )
            PredicateCheck( false, outdst ) ;
          else
            PredicateCheck( dsts[ 0 ]->Str() == dsts[ 1 ]->Str(), outdst ) ;
        } // if
        else
          PredicateCheck( dsts[ 0 ]->GetID() == dsts[ 1 ]->GetID() &&
                          dsts[ 0 ]->GetID() != -1 && dsts[ 1 ]->GetID() != -1, outdst ) ;
      } // if
      else
        PredicateCheck( dsts[ 0 ]->Str() == dsts[ 1 ]->Str(), outdst ) ;
    } // else if
    else if ( cmd->CheckType( BEGIN ) ) {
      if ( indst->GetDSTsSize() < 1 ) throw cmd ; // ?

      vector<DStreeP> dsts ;
      for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
        EvalSExp( indst->GetDST( i ), tempDST, defTable ) ;
        dsts.push_back( tempDST ) ;
      } // for

      outdst = dsts.back() ;

      return true ;
    } // else if
    else if ( cmd->CheckType( CONDITIONAL ) ) {
      if ( cmd->CheckCmd( "if" ) ) {
        if ( indst->GetDSTsSize() != 3 ) throw cmd ;
      } // if
      else {
        if ( indst->GetDSTsSize() == 0 ) throw cmd ;

        for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
          if ( indst->GetDST( i )->GetDSTsSize() < 2 ) {
            gCmd = cmd ;
            throw indst->GetDST( i ) ;
          } // if
        } // for

      } // else


      if ( cmd->CheckCmd( "if" ) ) {
        EvalSExp( indst->GetDST( 0 ), tempDST, defTable ) ;
        if ( tempDST->GetToken( 0 )->GetBool() )
          EvalSExp( indst->GetDST( 1 ), tempDST, defTable ) ;
        else
          EvalSExp( indst->GetDST( 2 ), tempDST, defTable ) ;

        outdst = tempDST ;
      } // if
      else {
        for ( unsigned int i = 0 ; i < indst->GetDSTsSize() ; ++i ) {
          EvalSExp( indst->GetDST( i )->GetDST( 0 ), tempDST, defTable ) ;
          if ( tempDST->GetToken( 0 )->GetBool() ) {
            for ( unsigned int j = 1 ; j < indst->GetDST( i )->GetDSTsSize() ; ++j ) {
              EvalSExp( indst->GetDST( i )->GetDST( j ), tempDST, defTable ) ;
            } // for

            outdst = tempDST ;
            // indst->GetDST( i )->GetDST( tempDST->GetDSTsSize() - 1 ) ;
            return true ;
          } // if
        } // for

        throw string( "no return value" ) ;
      } // else

      return true ;
    } // else if
    else if ( cmd->CheckCmd( "let" ) ) {
      string errorMag( "LET format" ) ;
      if ( indst->GetDSTsSize() < 2 ) throw errorMag ;

      nextDST = indst->GetDST( 0 ) ;
      if ( nextDST->GetCommand() != NULL ) throw errorMag ;

      for ( unsigned int i = 0 ; i < nextDST->GetDSTsSize() ; ++i )
        if ( nextDST->GetDST( i )->GetDSTsSize() != 2 ) throw errorMag ;

      DefinedTableP nextDefTable = new DefinedTable() ;
      for ( unsigned int i = 0 ; i < nextDST->GetDSTsSize() ; ++i ) {
        tempDST = nextDST->GetDST( i ) ;
        if ( ! tempDST->GetDST( 0 )->GetToken( 0 )->CheckType( SYMBOL ) ) throw errorMag ;

          EvalSExp( tempDST->GetDST( 1 ), tempDST2, defTable ) ;
          nextDefTable->SetDefined( new Defined( false, tempDST->GetDST( 0 )->GetToken( 0 ),
                                                 tempDST2 ) ) ;
      } // for

      LocalDefinedTableProcess( defTable, nextDefTable ) ;

      // Run
      vector<DStreeP> dsts ;
      for ( unsigned int i = 1 ; i < indst->GetDSTsSize() ; ++i ) {
        EvalSExp( indst->GetDST( i ), tempDST, nextDefTable ) ;
        dsts.push_back( tempDST ) ;
      } // for

      outdst = dsts.back() ;
      return true ;
    } // else if
    else if ( cmd->CheckCmd( "lambda" ) ) {
      // if ( indst->GetDSTsSize() < 1 ) throw cmd ; // ?
      nextDST = indst->GetDST( 0 ) ;

      gNextDefined = new Defined() ;
      gNextDefined->SetSymbol( new Token( SYMBOL, "lambda" ) ) ;
      for ( unsigned int i = 0 ; i < nextDST->GetDSTsSize() ; ++i )
        gNextDefined->SetSymbol( nextDST->GetDST( i )->GetToken( 0 ) ) ;

      for ( unsigned int i = 1 ; i < indst->GetDSTsSize() ; ++i )
        gNextDefined->SetDST( indst->GetDST( i ) ) ;


      /*
      for ( unsigned int i = 0 ; i < defined->GetDSTsSize() ; ++i )
        EvalSExp( defined->GetDST( i ), tempDST2, defTable ) ;
      */

      outdst = indst ;
      return true ;
    } // else if
    else if ( cmd->CheckCmd( "clean-environment" ) ) {
      if ( indst->GetLevelN() != 0 ) throw string( "clean-environment format" ) ;

      gDefinedTable.Clear() ;
      outdst = indst ;
      return true ;
    } // else if
    else if ( cmd->CheckCmd( "exit" ) ) {
      if ( indst->GetLevelN() != 0 ) throw string( "level of exit" ) ;
      else throw 0 ;

      return true ;
    } // else if

  } // else

} // EvalSExp()

void PredicateCheck( bool isIt, DStreeP &resultDST ) {
  TokenP resultToken = ( isIt ) ? new Token( T ) : new Token( NIL ) ;
  resultDST->SetToken( 0, resultToken ) ;
} // PredicateCheck()

int main() {
  Initialization() ;

  bool isExit = false ;
  DStreeP dst ;
  DStreeP resultdst ;
  // int i = 0 ;
  while ( !isExit ) {
    Reset( dst, resultdst ) ;
    try {
      if ( ! SExp( dst ) ) throw 1 ;
      EvalSExp( dst, resultdst, new DefinedTable() ) ;
      Output( resultdst ) ;

      // if ( ++i == 6 ) isExit = true ; // test
    } // try
    catch ( int n ) { // ERROR
      stringstream ss ( stringstream::in | stringstream::out ) ;
      ss << "ERROR " ;
      if ( n == 1 || n == 2 || n == 3 ) {
        ss << "(unexpected character) : line " << gLC.GetLineNum() << " column " ;

        if ( n == 1 && gNextT->CheckType( DOT ) ) {
          ss << gLC.GetColumnNum() ;
          if ( cin.peek() == '\n' )
            ss << " LINE-ENTER encountered" ;
          else
            ss << " character \'" << ( char ) cin.peek() << "\'" ;
        } // if
        else if ( n == 2 || ( n == 1 && !gNextT->CheckType( DOT ) ) ) {
          ss << gLC.GetColumnNum() - gNextT->GetName().size() ;
          if ( gNextT->GetName() == "\n" )
            ss << " LINE-ENTER encountered" ;
          else
            ss << " character \'" << gNextT->GetName()[ 0 ] << "\'" ;
        } // else if
        else {
          ss << gLC.GetColumnNum() ;
          ss << " LINE-ENTER encountered" ;
        } // else

        Skip() ;
      } // if
      else if ( n == -1 )
        ss << ": END-OF-FILE encountered when there should be more input" ;

      if ( n != 0 ) Output( ss.str() ) ;
      if ( n == 0 || n == -1 ) isExit = true ;
      if ( n == 0 ) cout << "\n" ;
    } // catch
    catch ( TokenP token ) {
      stringstream ss ( stringstream::in | stringstream::out ) ;
      ss << "ERROR " ;
      if ( token->CheckType( SYMBOL ) )
        ss << "(unbound symbol) : " << token->Str() ;
      else
        ss << "(attempt to apply non-function) : " << token->Str() ;
      Output( ss.str() ) ;
    } // catch
    catch ( string errorMag ) {
      stringstream ss ( stringstream::in | stringstream::out ) ;
      if ( errorMag == "no return value" )
        ss << "ERROR " << '(' << errorMag << ')' << " : cond" ;
      else
        ss << "ERROR " << '(' << errorMag << ')' ;

      Output( ss.str() ) ;
    } // catch
    catch ( CommandP errorCmd ) {
      stringstream ss ( stringstream::in | stringstream::out ) ;
      ss << "ERROR (incorrect number of arguments) : " << errorCmd->GetName() ;

      Output( ss.str() ) ;
    } // catch
    catch ( DStreeP errorDST ) {
      stringstream ss ( stringstream::in | stringstream::out ) ;
      if ( gCmd->CheckCmd( "cond" ) )
        ss << "ERROR (cond parameter format) : " << errorDST->Str() ;
      else
        ss << "ERROR (" << gCmd->GetName() << " with incorrect argument type) : " << errorDST->Str() ;

      Output( ss.str() ) ;
    } // catch

  } // while

  Output( "Thanks for using OurScheme!" ) ;

  return 0;
} // main()
