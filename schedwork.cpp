#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

void testPrintSchedule(const DailySchedule& sched)
{
    int day = 0;
    for(auto s : sched)
    {
        cout << "Day " << day << ": ";
        for(auto nurse : s)
        {
            cout << nurse << " ";
        }
        cout << endl;
        day++;
    }
}

bool isValid(
    const AvailabilityMatrix& avail,
    const vector<unsigned int>& shifts,
    const size_t maxShifts,
    const DailySchedule& sched,
    const unsigned int currWorker,
    const unsigned int day
);

bool scheduleRec(
    const AvailabilityMatrix& avail,
    vector<unsigned int>& shifts,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    unsigned int r,
    unsigned int c
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    unsigned int numWorkers = avail[0].size();
    unsigned int d = avail.size(); //number of days

    
    vector<unsigned int> empties;
    for (unsigned int i = 0; i < dailyNeed; ++i) {
      empties.push_back(numWorkers);
    }

    for (unsigned int i = 0; i < d; ++i) {
      sched.push_back(empties);
    }
    vector<unsigned int> shifts(numWorkers);

    return scheduleRec(avail,shifts,dailyNeed,maxShifts,sched,0,0);

}

bool scheduleRec(
    const AvailabilityMatrix& avail,
    vector<unsigned int>& shifts,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    unsigned int r,
    unsigned int c
)
{
  if (r == avail.size()) {
    return true;
  }
  for (unsigned int i = 0; i < avail[0].size(); ++i) {
    sched[r][c] = i;
    shifts[i]++;
    
    if (isValid(avail,shifts,maxShifts,sched,i,r)) {
      bool status;
      if (c < sched[0].size()-1) {
        status = scheduleRec(avail, shifts, dailyNeed, maxShifts, sched, r, c+1);
      }
      else {
        status = scheduleRec(avail, shifts, dailyNeed, maxShifts, sched, r+1, 0);
      }
      
      if (status) return true;
    }
    sched[r][c] = avail[0].size();
    shifts[i]--;
  }
  return false;
}

bool isValid(
    const AvailabilityMatrix& avail,
    const vector<unsigned int>& shifts,
    const size_t maxShifts,
    const DailySchedule& sched,
    const unsigned int currWorker,
    const unsigned int day
)
{
  if (shifts[currWorker] > maxShifts || !avail[day][currWorker] || count(sched[day].begin(), sched[day].end(), currWorker) > 1) {
    return false;
  }
  return true;
}

