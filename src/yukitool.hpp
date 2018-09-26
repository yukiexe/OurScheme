#ifndef YUKITOOL_H
#define YUKITOOL_H

#include <string>
#include <sstream>
#include <algorithm>

using namespace std ;

class YukiTool {
  public:
    YukiTool();
    static void toUpper( string &s ) ;
    static string convertInt( int number ) ;
    static int convertStr( string str ) ;
  protected:
  private:
};

#endif // YUKITOOL_H
