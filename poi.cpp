/*
 *  Name:        Poi!
 *  Author:      Alan
 *  Description: Computes the best three expeditions to run for the desired
 *               resources.
 */

#include "expedition_helper.h"
#include "string_to.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::vector;

int main() {
  int factors[5];
  int afk_time;
  int afk_minutes;
  string buff_the_string;
  char buff_the_buckets;
  cout << "Please enter the importance factor for each resource in turn."
       << endl << "Enter 0 if you don't need it, 1 if you want some, 2 or 3 if "
       << "you want a lot." << endl;
  // Get from the user all the factors
  cout << "Fuel:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, factors[0])) {
    getline(cin, buff_the_string);
  }
  cout << "Ammo:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, factors[1])) {
    getline(cin, buff_the_string);
  }
  cout << "Iron:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, factors[2])) {
    getline(cin, buff_the_string);
  }
  cout << "Bauxite:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, factors[3])) {
    getline(cin, buff_the_string);
  }
  cout << "Do you need buckets? (\"y\" or \"n\")" << endl;
  getline(cin, buff_the_string);
  while (!StringToChar(buff_the_string, buff_the_buckets)) {
    getline(cin, buff_the_string);
  }
  if (tolower(buff_the_string.at(0)) == 'y') {
    factors[4] = 1;
    cout << "Factoring in buckets." << endl;
  } else if (tolower(buff_the_string.at(0)) == 'n') {
    factors[4] = 0;
    cout << "Disregarding buckets." << endl;
  } else {
    factors[4] = 0;
    cout << "You entered something wrong. Disregarding buckets." << endl;
  }
  cout << "How many hours and minutes are you going to be afk for?" << endl
       << "Hours:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, afk_time)) {
    getline(cin, buff_the_string);
  }
  cout << "Minutes:" << endl;
  getline(cin, buff_the_string);
  while (!StringToInt(buff_the_string, afk_minutes)) {
    getline(cin, buff_the_string);
  }
  afk_minutes += afk_time * 60;
  cout << "You entered:" << endl
       << "Time AFK: " << afk_minutes << " minutes" << endl
       << "Fuel:     " << factors[0] << endl
       << "Ammo:     " << factors[1] << endl
       << "Iron:     " << factors[2] << endl
       << "Bauxite:  " << factors[3] << endl
       << "Buckets:  ";
  if (!factors[4]) {
    cout << "Not needed" << endl;
  } else {
    cout << "Needed" << endl;
  }
  // Now with all the factors, contruct the object
  ExpeditionHelper NewExpeditions(factors);
  NewExpeditions.SetAfkTime(afk_minutes);
  cout << "Performing calculations." << endl << endl;
  NewExpeditions.nCr(0, 3);
  cout << "I suggest you run these expeditions: " << endl << "[ ";
  for (unsigned int i = 0;
       i < NewExpeditions.GetMaximumExpeditions().size();
       ++i) {
    switch (NewExpeditions.GetMaximumExpeditions().at(i)) {
    case 40:
      cout << "A1";
      break;
    case 41:
      cout << "A2";
      break;
    case 42:
      cout << "A3";
      break;
    case 43:
      cout << "B1";
      break;
    default:
      cout << NewExpeditions.GetMaximumExpeditions().at(i) + 1;
      break;
    }
    cout << " ";
  }
  cout << "]" << endl << endl;
  NewExpeditions.ResourceGain();
  return 0;
}
