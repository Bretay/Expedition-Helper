/*
 * Name:        expedition_helper.cpp
 * Author:      Alan Bretney
 * Description: Implements a class to help determine which expeditions to run
 */

#include "expedition_helper.h"
#include "string_to.h"

#include <stdlib.h>

using std::vector;
using std::fstream;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;

ExpeditionHelper::ExpeditionHelper(int *factors) {
  // Expedition data is stored in this file. This includes Time, Flat Resources,
  // and Bucket Chance if applicable
  fstream fin("expedition_tables.txt");
  if (!fin.is_open()) {  // This really should never happen
    cout << "Error accessing tables." << endl;
    // The code should not be run, the class should not be used
    exit(EXIT_FAILURE);
  } else {
    cout << "Tables accessed. Processing." << endl;
    // Now that the file is open, store each line into a vector<double>
    for (unsigned int i = 1; i <= 44; ++i) {
      // Buffer vecotr to be push_back()'d into the main vector
      vector<double> buff;
      // Buffer string to hold a single line from the file
      string f_string;
      // This determines if the line is 5 or 6 long
      int resource_count = 1;
      getline(fin, f_string);
      for (unsigned int j = 0; j < f_string.length(); ++j) {
        if (isspace(f_string[j])) {
          resource_count++;
        }
      }
      // This puts the line into vector<double> buff
      for (int j = 0; j <= 5; ++j) {
        double buffer_double = 0;
        StringToDouble(f_string, buffer_double);
        size_t pos = f_string.find(" ") + 1;
        f_string.erase(0, pos);
        buff.push_back(buffer_double);
      }
      // Now that vector<double> buff is complete, put it in the master vector
      expeditions_.push_back(buff);
      // This outputs each vector<double> after it is made, for testing purposes
      /*for (unsigned int j = 0; j < 6; ++j) {
        cout << (expeditions_.at(i - 1)).at(j) << ' ';
      }
      cout << endl;*/
    }
    if (!factors) {
      cout << "Error reading resource factors." << endl;
    } else {
      cout << "Resource factors accessed. Processing." << endl;
      factors_ = new int[5];
      for (unsigned int i = 0; i < 5; ++i) {
        factors_[i] = factors[i];
      }
    }
    // This loop populates num_list_, for use by the nCr function
    cout << "Populating numbered list." << endl;
    for (unsigned int i = 0; i < 44; ++i) {
      num_list_.push_back(i);
    }
    // Calculate the average of each resource, based on the tables
    cout << "Calculating averages." << endl;
    for (unsigned int i = 0; i < 5; ++i) {
      double temp = 0;
      for (unsigned int j = 0; j < 44; ++j) {
        // Add the next expedition's resource value in for this given resource
        temp += (expeditions_.at(j)).at(i + 1);
      }
      averages_[i] = temp;
    }
    for (unsigned int i = 0; i < 5; ++i) {
      averages_[i] = averages_[i] / 44;
    }
    // Set all basic variables
    maximum_ = 0;
    bucket_factor_ = false;
    for (unsigned int i = 0; i < 3; i++) {
      maximum_expeditions_.push_back(i);
    }
  }  // fin.is_open()
}

void ExpeditionHelper::nCr(int offset, int k) {
  if (k == 0) {  // If at the end of the combination
    double buffer_resources = 0;
    // Calculate the buffer resources for this combination, based on given afk
    // time and given factors
    for (unsigned int i = 0; i < 3; ++i) {
      for (unsigned int j = 1; j < 5; ++j) {
        if ((expeditions_.at(buffer_.at(i))).at(0) < afk_time_) {
          if (factors_[4]) {
            buffer_resources += ((expeditions_.at(buffer_.at(i))).at(j)
                                / afk_time_
                                * 60
                                * factors_[j - 1])
                                / averages_[j - 1];
            buffer_resources += (expeditions_.at(buffer_.at(i))).at(5)
                                / afk_time_
                                * 60;
          } else {
            buffer_resources += ((expeditions_.at(buffer_.at(i))).at(j)
                                / afk_time_
                                * 60
                                * factors_[j - 1])
                                / averages_[j - 1];
          }
        } else {
          if (factors_[4]) {
            buffer_resources += ((expeditions_.at(buffer_.at(i))).at(j)
                                / (expeditions_.at(buffer_.at(i))).at(0)
                                * 60
                                * factors_[j - 1])
                                / averages_[j - 1];
            buffer_resources += (expeditions_.at(buffer_.at(i))).at(5)
                                / expeditions_.at(buffer_.at(i)).at(0)
                                * 60;
          } else {
            buffer_resources += ((expeditions_.at(buffer_.at(i))).at(j)
                                / (expeditions_.at(buffer_.at(i))).at(0)
                                * 60
                                * factors_[j - 1])
                                / averages_[j - 1];
          }
        }
      }
    }
    /*for (unsigned int i = 0; i < buffer_.size(); ++i) {
      cout << buffer_.at(i) << " ";
    }
    cout << endl << "Resources: " << buffer_resources << endl;*/
    if (buffer_resources > maximum_) {
      maximum_ = buffer_resources;
      maximum_expeditions_ = buffer_;
    }
    return;
  }
  // k != 0; not at the end of the combination yet
  // loop for all remaining numbers
  for (unsigned int i = offset; i <= num_list_.size() - k; ++i) {
    buffer_.push_back(num_list_.at(i));
    nCr(i + 1, k - 1);
    buffer_.pop_back();
  }
}

void ExpeditionHelper::ResourceGain() const {
  unsigned int fuel_fetched = 0;
  unsigned int ammo_fetched = 0;
  unsigned int iron_fetched = 0;
  unsigned int baux_fetched = 0;
  double buckets_fetched = 0;
  cout << "Your average hourly gain will be: " << endl;
  for (unsigned int i = 0;
       i < maximum_expeditions_.size();
       ++i) {
    if (expeditions_.at(maximum_expeditions_.at(i)).at(0) > afk_time_) {
      fuel_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(1)
                      / expeditions_.at(maximum_expeditions_.at(i)).at(0)
                      * 60;
      ammo_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(2)
                      / expeditions_.at(maximum_expeditions_.at(i)).at(0)
                      * 60;
      iron_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(3)
                      / expeditions_.at(maximum_expeditions_.at(i)).at(0)
                      * 60;
      baux_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(4)
                      / expeditions_.at(maximum_expeditions_.at(i)).at(0)
                      * 60;
      buckets_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(5)
                         / expeditions_.at(maximum_expeditions_.at(i)).at(0)
                         * 60;
    } else {
      fuel_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(1)
                      / afk_time_
                      * 60;
      ammo_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(2)
                      / afk_time_
                      * 60;
      iron_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(3)
                      / afk_time_
                      * 60;
      baux_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(4)
                      / afk_time_
                      * 60;
      buckets_fetched += expeditions_.at(maximum_expeditions_.at(i)).at(5)
                         / afk_time_
                         * 60;
    }
  }
  cout << "Fuel:    " << fuel_fetched << endl
       << "Ammo:    " << ammo_fetched << endl
       << "Iron:    " << iron_fetched << endl
       << "Bauxite: " << baux_fetched << endl
       << "Buckets: " << buckets_fetched << endl << endl
       << "Your average daily gain will be: "<< endl
       << "Fuel:    " << fuel_fetched * 24 << endl
       << "Ammo:    " << ammo_fetched * 24 << endl
       << "Iron:    " << iron_fetched * 24 << endl
       << "Bauxite: " << baux_fetched * 24 << endl
       << "Buckets: " << buckets_fetched * 24 << endl << endl;
  if (factors_[4]) {
    cout << "Remember to do daily quests. You can easily gain around 10" << endl
         << "buckets per day from them." << endl << endl;
  }
}
