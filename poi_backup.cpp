/*
 *  Name:        Poi!
 *  Author:      Alan
 *  Description: Computes the best three expeditions to run for the desired
 *               resources.
 */

// THIS FILE SHOULD NOT BE USED
// ...because it looks gross

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::stringstream;
using std::string;
using std::ios_base;
using std::fstream;
using std::vector;

const double kAVERAGE_RESOURCES[5] = {29.2, 30, 21.2, 13.36, 0.448333};

/*
 *  Attempts to convert a string to an integer value. If the operation fails,
 *  it returns false. If it succeeds, function returns true.
 *  @PARAM string value - the string to attempt conversion with
 *  @PARAM int& int_to_store - the integer to store the value in at the end.
 *                             should be unchanged if operation failed.
 *  @RETURN - true if successful, false if it fails
 */
bool StringToInt(string value, int& int_to_store);

/*
 *  Attempts to convert a string to an float value. If the operation fails,
 *  it returns false. If it succeeds, function returns true.
 *  @PARAM string value - the string to attempt conversion with
 *  @PARAM double& double_to_store - the float to store the value in at the end.
 *                                   should be unchanged if operation failed.
 *  @RETURN - true if successful, false if it fails
 */
bool StringToDouble(string value, double& double_to_store);

/*
 *  Runs through the possible combinations of expeditions, checking against
 *  previous max resources every time it generates a combination of 3.
 *  @PARAM int offset - where to start from in the generating array. When
 *                      called, this should be called from 0.
 *  @PARAM int k - How many elements to go out to. When called, this should be
 *                 called from 3.
 *  @PARAM const vector< vector<double> > &expe - main's expeditions vector will
 *                                                be passed in here
 *  @PARAM const vector<int> &num_list - numbered list to pull values from is
 *                                       passed in here
 *  @PARAM vector<int> &buffer - holds the current list of expiditions to
 *                               be checked
 *  @PARAM unsigned int &maximum - holds the current best resource value to be
 *                                 checked against
 *  @PARAM vector<int> &max_expe - holds the current best combination of
 *                                 expeditions
 *  @PARAM const int* factors - resource factors given by the user
 *  @PARAM const int afk - time in minutes user will be afk
 */
void ncr(
  int offset,
  int k,
  const vector< vector<double> > &expe,
  const vector<int> &num_list,
  vector<int> &buffer,
  double &maximum,
  vector<int> &max_expe,
  const int* factors,
  const int afk
    );

int main() {
  // This will store each expidition's hourly gains as vectors<double>
  vector < vector <double> > expeditions;
  int afk_time = 0;
  int afk_minutes = 0;
  int fuel_factor = 1;
  int ammo_factor = 1;
  int iron_factor = 1;
  int baux_factor = 1;
  int bucket_factor = 0;
  // This is used to take in input for the above factors
  string buffer_string;
  
  cout << "Please enter the importance factor for each resource in turn.\n"
       << "Enter 0 if you don't need it, 1 if you want some, 2 or 3 if you "
       << "want a lot." << endl;
  cout << "Fuel:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, fuel_factor)) {
    getline(cin, buffer_string);
  }
  cout << "Ammo:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, ammo_factor)) {
    getline(cin, buffer_string);
  }
  cout << "Iron:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, iron_factor)) {
    getline(cin, buffer_string);
  }
  cout << "Bauxite:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, baux_factor)) {
    getline(cin, buffer_string);
  }
  cout << "Do you need buckets? (\"y\" or \"n\")" << endl;
  getline(cin, buffer_string);
  if (buffer_string == "y") {
    bucket_factor = 1;
  } else if (buffer_string == "n") {
  } else {
    cout << "You entered something wrong. Disregarding buckets." << endl;
  }
  cout << "How many hours and minutes are you going to be afk for?" << endl
       << "Hours:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, afk_time)) {
    getline(cin, buffer_string);
  }
  afk_time *= 60;
  cout << "Minutes:" << endl;
  getline(cin, buffer_string);
  while(!StringToInt(buffer_string, afk_minutes)) {
    getline(cin, buffer_string);
  }
  afk_time += afk_minutes;
  cout << "You entered: " << endl
       << "Time AFK: " << afk_time << " minutes" << endl
       << "Fuel:     " << fuel_factor << endl
       << "Ammo:     " << ammo_factor << endl
       << "Iron:     " << iron_factor << endl
       << "Bauxite:  " << baux_factor << endl
       << "Buckets:  ";
  if (!bucket_factor) {
    cout << "Not needed" << endl;
  } else {
    cout << "Needed" << endl;
  }
  int factors[4] = {fuel_factor, ammo_factor, iron_factor, baux_factor};
  // Flat gain per expiditon is stored in this file, along with minutes.
  // Some lines are 6 long, representing bucket count.
  fstream fin("expidition_tables.txt");
  if (!fin.is_open()) {
    cout << "Error opening file." << endl;
  } else {
    cout << "Tables accessed. Calculating." << endl;
    // Now that the file is open, store each line into a vector<double>.
    for (unsigned int i = 1; i <= 44; ++i) {
      // Buffer vector to be push_back()'d into the main vector.
      vector <double> buff;
      // Buffer string to hold a line from the file.
      string f_string;
      // This determines if the line is 5 or 6 long.
      int resource_count = 1;
      getline(fin, f_string);
      for (int j = 0; j < f_string.length(); ++j) {
        if (isspace(f_string[j])) {
          resource_count ++;
        }
      }
      for (int j = 0; j <= resource_count; ++j) {
        double buffer_double = 0;
        StringToDouble(f_string, buffer_double);
        size_t pos = f_string.find(" ") + 1;
        f_string.erase(0, pos);
        buff.push_back(buffer_double);
      }

      // This code outputs each vector<double> after it got made
      /*for (unsigned int j = 0; j < resource_count; ++j) {
        cout << buff[j] << ' ';
      }
      cout << endl;*/

      // Put the vector<double> into the vector< vector<double> >
      expeditions.push_back(buff);
    }
    // This stores the numbers 0-43, for use by the ncr function to pull from.
    vector<int> numbered_list;
    for (unsigned int i = 0; i < 44; ++i) {
      numbered_list.push_back(i);
    }
    // This stores the combo which gave the most weighted resources so far;
    // each number corresponds to an element of the expeditions vector.
    vector<int> max_expeditions;
    // This is used inside the function to store the current list of expeditions
    // to be checked.
    vector<int> buf_expeditions;
    // This stores the amount of resources the best combo found so far
    // gave, weighted based on given input.
    double max_resources = 0;
    // This function checks all combinations
    ncr(
      0,
      3,
      expeditions,
      numbered_list,
      buf_expeditions,
      max_resources,
      max_expeditions,
      factors,
      afk_time
        );
    if (bucket_factor) {
      int buffer_lowest = 0;
      int buffer_index = 0;
      for (unsigned int i = 1; i < 5; ++i) {
        if ((expeditions.at(max_expeditions.at(1))).at(0) < afk_time) {
          buffer_lowest += ((expeditions.at(max_expeditions.at(1))).at(i))
                            / afk_time * 60
                            * factors[i - 1]
                            / kAVERAGE_RESOURCES[i - 1];
        } else {
          buffer_lowest += ((expeditions.at(max_expeditions.at(1))).at(i))
                            / expeditions.at(max_expeditions.at(1)).at(0) * 60
                            * factors[i - 1]
                            / kAVERAGE_RESOURCES[i - 1];
        }
      }
      int current_lowest = buffer_lowest;
      for (unsigned int i = 1; i < 3; ++i) {
        buffer_lowest = 0;
        for (unsigned int j = 1; j < 5; ++j) {
          if ((expeditions.at(max_expeditions.at(i))).at(0) < afk_time) {
            buffer_lowest += ((expeditions.at(max_expeditions.at(i))).at(j))
                              / afk_time * 60
                              * factors[j - 1]
                              / kAVERAGE_RESOURCES[j - 1];
          } else {
            buffer_lowest += ((expeditions.at(max_expeditions.at(i))).at(j))
                              / expeditions.at(max_expeditions.at(i)).at(0) * 60
                              * factors[j - 1]
                              / kAVERAGE_RESOURCES[j - 1];
          }
        }
        if (buffer_lowest < current_lowest) {
          current_lowest = buffer_lowest;
          buffer_index = i;
        }
      }
      max_expeditions.at(buffer_index) = 1;
    }
    cout << "-------------------------------------------------" << endl
         << "I suggest you run these expeditions: " << endl
         << "[ ";
    for (unsigned int i = 0; i < max_expeditions.size(); ++i) {
      if (max_expeditions.at(i) + 1 < 40) {
        cout << max_expeditions.at(i) + 1;
      } else {
        switch (max_expeditions.at(i)) {
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
        }
      }
      cout << " ";
    }
    cout << "]" << endl
         << "Your hourly gain will be: " << endl;
    unsigned int fuel_fetched = 0;
    unsigned int ammo_fetched = 0;
    unsigned int iron_fetched = 0;
    unsigned int baux_fetched = 0;
    for (unsigned int i = 0; i < 3; ++i) {
      fuel_fetched += expeditions.at(max_expeditions.at(i)).at(1)
                      / expeditions.at(max_expeditions.at(i)).at(0) * 60;
      ammo_fetched += expeditions.at(max_expeditions.at(i)).at(2)
                      / expeditions.at(max_expeditions.at(i)).at(0) * 60;
      iron_fetched += expeditions.at(max_expeditions.at(i)).at(3)
                      / expeditions.at(max_expeditions.at(i)).at(0) * 60;
      baux_fetched += expeditions.at(max_expeditions.at(i)).at(4)
                      / expeditions.at(max_expeditions.at(i)).at(0) * 60;
    }
    cout << "Fuel:    " << fuel_fetched << endl
         << "Ammo:    " << ammo_fetched << endl
         << "Iron:    " << iron_fetched << endl
         << "Bauxite: " << baux_fetched << endl << endl
         << "In a day, you will gain: " << endl
         << fuel_fetched * 24 << " fuel," << endl
         << ammo_fetched * 24 << " ammo," << endl
         << iron_fetched * 24 << " iron," << endl
         << baux_fetched * 24 << " bauxite." << endl << endl;
    if (bucket_factor) {
      cout << "-------------------------------------------------" << endl
           << "Remember, if you have no afk time, always run expedition 2."
           << endl << "It gets 1 bucket per hour if you run it twice in an"
           << " hour." << endl << endl
           << "If you have a lot of afk time, run expeditions 26 or 39 instead,"
           << endl << "but make sure to sparkle your ships, and overdrum if"
           << " needed." << endl << endl
           << "Also remember daily quests! You can get a number of buckets"
           << endl << "every day just by completing dailies." << endl << endl;
    }
  } // fin.is_open()
  return 0;
}

bool StringToInt(string value, int& int_to_store) {
  stringstream converter(value);
  converter.exceptions(ios_base::failbit);
  try {
    converter >> int_to_store;
  } catch (ios_base::failure f) {
    cout << "Couldn't parse integer value." << endl << endl;
    return false;
  }
  return true;
}

bool StringToDouble(string value, double& double_to_store) {
  stringstream converter(value);
  converter.exceptions(ios_base::failbit);
  try {
    converter >> double_to_store;
  } catch (ios_base::failure f) {
    cout << "Couldn't parse floating point value." << endl << endl;
    return false;
  }
  return true;
}

void ncr(
  int offset,
  int k,
  const vector< vector<double> > &expe,
  const vector<int> &num_list,
  vector<int> &buffer,
  double &maximum,
  vector<int> &max_expe,
  const int* factors,
  const int afk
    ) {
  // This code terminates the recursion chain. Check the generated expidition
  // array here.
  if (k == 0) {
    double buffer_resources = 0;
    for (unsigned int i = 0; i < 3; ++i) {
      for (unsigned int j = 1; j < 5; ++j) {
        if ((expe.at(buffer.at(i))).at(0) < afk) {
          buffer_resources += ((expe.at(buffer.at(i))).at(j)
                                / afk * 60
                                * factors[j - 1])
                                / kAVERAGE_RESOURCES[j - 1];
        } else {
          buffer_resources += ((expe.at(buffer.at(i))).at(j)
                                / (expe.at(buffer.at(i))).at(0) * 60
                                * factors[j - 1])
                                / kAVERAGE_RESOURCES[j - 1];
        }
      }
    }
    if (buffer_resources > maximum) {
      maximum = buffer_resources;
      max_expe = buffer;
    }
    return;
  }
  for (int i = offset; i <= num_list.size() - k; ++i) {
    buffer.push_back(num_list.at(i));
    ncr(i + 1, k - 1, expe, num_list, buffer, maximum, max_expe, factors, afk);
    buffer.pop_back();
  }
}
