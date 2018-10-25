#include "Token.hpp"


char LineCounter::Counter( const char &ch ) { // 與 peek 之 char 同位置
  if ( ch == '\n' ) {
    m_oldline = m_line ;
    m_oldcolumn = m_column ;
    if ( !m_newLine ) ++m_line ;
    else m_newLine = false ;
    m_column = 1 ;
  } // if
  else ++m_column ;

  return ch ;
} // LineCounter::Counter()

// Token
Token::Token( TokenType type ) {
  SetType( type ) ;
  if ( CheckType( LEFTPAREN )  ) SetName( "(" ) ;
  else if ( CheckType( RIGHTPAREN )  ) SetName( ")" ) ;
  else if ( CheckType( DOT )  ) SetName( "." ) ;
  else if ( CheckType( NIL )  ) SetName( "nil" ) ;
  else if ( CheckType( T )  ) SetName( "#t" ) ;
} // Token::Token()

Token::Token( TokenType type, string str ) {
  SetType( type ) ;
  SetName( str ) ;
} // Token::Token()

char Token::CharPeek( LineCounter &lc ) { // find first non-space char
  if ( isspace( cin.peek() ) ) { // tab ('\t'), space (' '), (white-space control codes: '\f','\v','\n','\r')
    lc.Counter( cin.get() ) ;
    return CharPeek( lc ) ;
  } // if
  else if ( cin.peek() == ';' ) { // comment
    while ( cin.peek() != '\n' && cin.peek() != -1 )
      lc.Counter( cin.get() ) ;

    return CharPeek( lc ) ;
  } // else if
  else if ( cin.peek() == -1 ) throw -1 ;
  else
    return cin.peek() ;
} // Token::CharPeek()

void Token::GetToken( LineCounter &lc ) {
  Initialization() ;

  char ch = CharPeek( lc ) ;
  string str = "" ;
  if ( ch == '\"' ) {
    do { // Set Token Name
      str.push_back( lc.Counter( cin.get() ) ) ;
    } while ( cin.peek() != '\"' && cin.peek() != '\n' && cin.peek() != -1 ) ; // do while

    if ( cin.peek() == -1 ) throw -1 ;
    else if ( cin.peek() == '\n' ) throw 3 ;
    else str.push_back( lc.Counter( cin.get() ) ) ;

    SetType( STRING ) ;
  } // if
  else if ( ch == '(' || ch == ')' || ch == '\'' )
    str.push_back( lc.Counter( cin.get() ) ) ;
  else { // ???
    do { // Set Token Name
      str.push_back( lc.Counter( cin.get() ) ) ;
    } while ( isgraph( cin.peek() )
              && cin.peek() != -1
              && cin.peek() != '(' && cin.peek() != ')'
              && cin.peek() != '\'' && cin.peek() != '\"' ) ; // do while

  } // else

  SetName( str ) ;

  if ( CheckType( NOTHING )  ) {
    if ( CheckCommandType() ) SetType( COMMAND ) ;
    else if ( CheckName( "(" ) ) SetType( LEFTPAREN ) ;
    else if ( CheckName( ")" )  ) SetType( RIGHTPAREN ) ;
    else if ( CheckName( "." )  ) SetType( DOT ) ;
    else if ( CheckName( "nil" )  || CheckName( "#f" ) || CheckName( "()" )  ) // WARNING!!!
      SetType( NIL ) ;
    else if ( CheckName( "t" ) || CheckName( "#t" ) ) SetType( T ) ;
    else if ( ! AnalysisNumToken( str )  ) // INT, FLOAT
      SetType( SYMBOL ) ; // SYMBOL
  } // if
} // Token::GetToken()

bool Token::AnalysisNumToken( string & str ) {
  if ( str.empty() ||
       ( !isdigit( str[ 0 ] ) && str[ 0 ] != '+' && str[ 0 ] != '-' && str[ 0 ] != '.' ) ) return false ;

  if ( str == "+" || str == "-" ) return false ;

  bool isFloat = ( str[ 0 ] == '.' ) ;
  for ( int i = 1 ; i < str.length() ; ++i ) {
    if ( !isdigit( str[ i ] ) ) {
      if ( isFloat ) return false ; // second dot
      else if ( str[ i ] == '.' ) isFloat = true ;
      else return false ; // etc
    } // if
  } // for

  if ( isFloat ) SetType( FLOAT ) ;
  else SetType( INT ) ;

  return true ;
} // Token::AnalysisNumToken()

bool Token::CheckCommandType() {
  vector <string> commandStrs ;
  commandStrs.push_back( "cons" ) ;
  commandStrs.push_back( "list" ) ;
  commandStrs.push_back( "quote" ) ;
  commandStrs.push_back( "'" ) ;
  commandStrs.push_back( "define" ) ;
  commandStrs.push_back( "car" ) ;
  commandStrs.push_back( "cdr" ) ;
  commandStrs.push_back( "pair?" ) ;
  commandStrs.push_back( "null?" ) ;
  commandStrs.push_back( "integer?" ) ;
  commandStrs.push_back( "real?" ) ;
  commandStrs.push_back( "number?" ) ;
  commandStrs.push_back( "string?" ) ;
  commandStrs.push_back( "boolean?" ) ;
  commandStrs.push_back( "symbol?" ) ;
  commandStrs.push_back( "+" ) ;
  commandStrs.push_back( "-" ) ;
  commandStrs.push_back( "*" ) ;
  commandStrs.push_back( "/" ) ;
  commandStrs.push_back( "not" ) ;
  commandStrs.push_back( "and" ) ;
  commandStrs.push_back( "or" ) ;
  commandStrs.push_back( ">" ) ;
  commandStrs.push_back( ">=" ) ;
  commandStrs.push_back( "<" ) ;
  commandStrs.push_back( "<=" ) ;
  commandStrs.push_back( "=" ) ;
  commandStrs.push_back( "string-append" ) ;
  commandStrs.push_back( "string>?" ) ;
  commandStrs.push_back( "eqv?" ) ;
  commandStrs.push_back( "equal?" ) ;
  commandStrs.push_back( "begin" ) ;
  commandStrs.push_back( "if" ) ;
  commandStrs.push_back( "cond" ) ;
  commandStrs.push_back( "clean-environment" ) ;
  commandStrs.push_back( "exit" ) ;
  commandStrs.push_back( "let" ) ;
  commandStrs.push_back( "lambda" ) ;


  for ( unsigned int i = 0 ; i < commandStrs.size() ; ++i )
    if ( CheckName( commandStrs[ i ] ) ) return true ;

  return false ;
} // Token::CheckCommandType()

string Token::Str() {
  stringstream ss ( stringstream::in | stringstream::out ) ;
  if ( CheckType( NIL ) ) return "nil" ;
  else if ( CheckType( T ) ) return "#t" ;
  else if ( CheckType( INT ) ) {
    ss << m_name ;
    int x ;
    ss >> x ;
    ss.clear() ;
    ss.str( "" ) ;
    ss << x ;
    return ss.str() ;
  } // else if
  else if ( CheckType( FLOAT ) ) {
    ss.precision( 3 ) ;
    ss << m_name ;
    float val ;
    ss >> val ;
    ss.clear() ;
    ss.str( "" ) ;
    ss << fixed << val ;
    return ss.str() ;
  } // else if
  else if ( CheckType( COMMAND ) ) return "#function" ;


  // LEFTPAREN, RIGHTPAREN, INT
  return m_name ;
} // Token::Str()

