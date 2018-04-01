#include "..\include\yukitool.h"

YukiTool::YukiTool() {
    //ctor
}

void YukiTool::toUpper( string &s ) {
  transform( s.begin(), s.end(), s.begin(), ::toupper);
} // toUpper()

string YukiTool::convertInt( int number ) {
   stringstream ss ; //create a stringstream
   ss << number ; //add number to the stream
   return ss.str() ; //return a string with the contents of the stream
} // convertInt()

int YukiTool::convertStr( string str ) {
   unsigned int numb ;
   istringstream ( str ) >> numb;
   return numb ;
} // convertStr()
