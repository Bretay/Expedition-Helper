/*
 * Name:        expedition_helper.h
 * Author:      Alan Bretney
 * Description: Declares a class to help determine which expeditions to run
 */

#ifndef EXPEDITION_HELPER_H
#define EXPEDITION_HELPER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class ExpeditionHelper {
 public:
  /*
   * Default constructor. Loads in the tables, calculates averages, sets
   * the array of factors_, and populates num_list_
   */
  ExpeditionHelper(int *factors);
  /*
   * Destructor. Deletes the dynamic array factors_
   */
  ~ExpeditionHelper() {delete factors_;}
  /*
   * Calculates the three expeditions that will give the desired resources,
   * based on the factors_ given. Final output stored int maximum_expeditions_.
   * @param int offset - How far into the recursion the current call is
   *                     When called, this should always be 0
   * @param int k      - How far to go into the recursion
   *                     When called, this should always be 3
   */
  void nCr(int offset = 0, int k = 3);
  /*
   * Displays the resource gain for the current maximum expeditions.
   */
  void ResourceGain() const;
  /*
   * Accessor for maximum_expeditions_
   */
  std::vector<int> GetMaximumExpeditions() const {return maximum_expeditions_;}
  /*
   * Mutator for afk_time_
   */
  void SetAfkTime(int afk_time) {afk_time_ = afk_time;}

 private:
  std::vector< std::vector<double> > expeditions_;
  std::vector<int> num_list_;
  std::vector<int> buffer_;
  double maximum_;
  std::vector<int> maximum_expeditions_;
  int *factors_;
  bool bucket_factor_;
  int afk_time_;
  int averages_[5];
};

#endif /* EXPEDITION_HELPER_H */
