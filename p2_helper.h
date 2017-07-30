#ifndef P2_HELPER_H
#define P2_HELPER_H

#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

/* Function splitString: splits input string with white space
 * Input: string ip
 * Output: vector of strings, get rid of characters after '#'
 */
vector<string> splitString(string ip);

/* Function isWindowSize: check if input string vector is a valid integer
 * Input: vector<string> with size 1
 * Output: true if the string can be converted into an integer; false otherwise
 */
bool isWindowSize(vector<string>& ip);


/* Function isEdge: check if given string vector provides a valid edge data
 * Input: vector<string> with size of 3
 * Output: true if the 3 strings are integers
 */
bool isEdge(vector<string>& ip);

/* Function isNum: check if given string is a number
 * Input: a string
 * Output: true if the string consists of digit only; false if otherwise
 */
bool isNum(string);

string intToStr(int num);


#endif











