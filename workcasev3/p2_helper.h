#ifndef P2_HELPER_H
#define P2_HELPER_H

#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

typedef struct Socket_arg {
  const char* h;
  const char* p;
} Socket_arg;

/* Function splitString: splits input string with white space
 * Input: string ip
 * Output: vector of strings, get rid of characters after '#'
 */
vector<string> splitString(string ip);

/* Function splitCsv: splits Comma Separated Value into a vector of doubles
 * Input: string
 * Output: a vector of doubles. If input is invalid, return empty vector
 */
vector<double> splitCsv(string ip);

/* Function isWindowSize: check if input string vector is a valid integer
 * Input: vector<string> with size 1
 * Output: true if the string can be converted into an integer; false otherwise
 */
bool isWindowSize(vector<string>& ip);

/* Function isNum: check if given string is a number
 * Input: a string
 * Output: true if the string consists of digit only; false if otherwise
 */
bool isNum(string);

string intToStr(int num);

bool readInput(int& window_size, string& host, string& port);

void displayPos(vector<double> pos);

#endif











