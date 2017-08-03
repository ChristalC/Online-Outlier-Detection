// Implementation for p2_helper
#include "p2_helper.h"

#include <sstream>
#include <tuple>
#include <iostream>

using namespace std;

/* Function splitString: splits input string with white space
 * Input: string ip
 * Output: vector of strings, get rid of characters after '#'
 */
vector<string> splitString(string ip) {
  // Find the '#' and get rid of anything after, including '#'
  size_t pos;
  pos = ip.find("#");
  if (pos != string::npos) {
    ip = ip.substr(0, pos);
  }

  // Split the string by white spaces
  string buf;
  stringstream ss(ip);

  vector<string> tokens;

  while (ss >> buf) {
    tokens.push_back(buf);
  }
  return tokens;
}

/* Function isWindowSize: check if input string vector is a valid integer
 * Input: vector<string> 
 * Output: true if the vector size is 1 and string can be converted into an integer; false otherwise
 */
bool isWindowSize(vector<string>& ip) {
  return (ip.size() == 1 && isNum(ip[0]));
}

/* Function isNum: check if given string is a number
 * Input: a string
 * Output: true if the string consists of digit only; false if otherwise
 */
bool isNum(string s) {
  return (s.find_first_not_of("0123456789.") == string::npos);
}

// Function intToStr accepts an integer as input and output the number as a string
string intToStr(int num) {
  stringstream ss;
  ss << num;
  return ss.str();
}


/* Function splitCsv: splits Comma Separated Value into a vector of doubles
 * Input: string
 * Output: a vector of doubles. If input is invalid, return empty vector
 * The input doubles have to be non-negative.
 */
vector<double> splitCsv(string ip) {
  vector<double> out;
  // Split the string by comma
  string buf;
  stringstream ss(ip);

  vector<double> empty_vector;
  
  while (getline(ss, buf, ',')) {
    if (isNum(buf)) {
      double d = stod(buf, NULL);
      if (d < 0) {
        cout << "Detect negative point, invalid. " << endl;
        return empty_vector;
      }
      out.push_back(d);
    }
    else {
      return empty_vector;
    }
  }
  return out;
}

/* Function readInput: read from stdin the window size and host:port
 * Input: reference parameter window_size, host and port
 * Output: true if the input read are correct; false otherwise
 */
bool readInput(int& window_size, string& host, string& port) {
  string holder;
  vector<string> read_input;

  // Neglect all comment lines
  do {
    if (getline(cin, holder)) {
      read_input = splitString(holder);
    }
    else {
      return false;
    }
  } while (read_input.size() == 0);

  // Read the first line as window_size. 
  // If a number is read, update the window_size
  // If it is not a number, return false
  if (isWindowSize(read_input)) {
    window_size = stoi(read_input[0], NULL, 10);
  }
  else {
    cout << "Window size is incorrect!\n";
    return false;
  }

  // Neglect comment lines
  do {
    if (getline(cin, holder)) {
      read_input = splitString(holder);
    }
    else {
      return false;
    }
  } while (read_input.size() == 0);

  // Read the second line as host and port, marked by a ':' in between
  // If correctly read, update host and port
  // If not, return false
  if (read_input.size() != 1) {
    cout << "Input host:port is invalid!\n";
    return false;
  }
  holder = read_input[0];
  int pos_colon = holder.find_first_of(":");
  if (pos_colon == string::npos) {
    cout << "Port not specified!" << endl;
    return false;
  }
  host = holder.substr(0, pos_colon);
  port = holder.substr(pos_colon + 1);

  // Check if port is a number, if not, return false
  if (!isNum(port)) {
    cout << "Invalid port entered\n";
    return false;
  }
  
  return true;
}

void displayPos(vector<double> pos) {
  int n = pos.size();
  for (int i = 0; i < n; i++) {
    cout << pos[i] << ", ";
  }
  cout << endl;
}








