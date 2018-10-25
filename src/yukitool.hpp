#ifndef YUKITOOL_HPP_
#define YUKITOOL_HPP_

#include <string>
#include <sstream>
#include <algorithm>

using namespace std ;

class YukiTool {
  public:
    YukiTool() {}
    static void toUpper( string &s ) {
      transform( s.begin(), s.end(), s.begin(), ::toupper);
    } // toUpper()

    static string convertInt( int number ) {
      stringstream ss ; //create a stringstream
      ss << number ; //add number to the stream
      return ss.str() ; //return a string with the contents of the stream
    } // convertInt()

    static int convertStr( string str ) {
       unsigned int numb ;
       istringstream ( str ) >> numb;
       return numb ;
    } // convertStr()

};

#endif // YUKITOOL_HPP_
