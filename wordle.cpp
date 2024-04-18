#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void genRequirements(
    std::set<std::string>& ret,
    const std::string& options,
    std::string input,
    std::set<int> visited,
    unsigned int index
);

void genAllStrs(
    const std::string& floating,
    std::set<std::string>& ret,
    std::string input,
    unsigned int index
);

void getInitStr(
    std::string& options,
    const std::string& in,
    unsigned int remaining,
    unsigned int index
);



// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
  std::string options = floating;

  unsigned int emptyCount = 0;
  for (unsigned int i = 0; i < in.size(); ++i) {
    if (!isalpha(in[i])) {
      emptyCount++;
    }
  }
  unsigned int remaining = emptyCount - floating.size();
  
  getInitStr(options, in, remaining, 0);
  
  std::set<std::string> init;
  std::set<int> visited; //to generate all combos with no reps
  genRequirements(init, options, in, visited, 0);
  
  std::set<std::string>::iterator it = init.begin();
  std::set<std::string> ret;
  while (it != init.end()) {
    genAllStrs(floating, ret, *it, 0);
    it++;
  }

  std::set<std::string>::iterator it2 = ret.begin();
  while (it2 != ret.end()) {
    if (dict.find(*it2) == dict.end()) {
      it2 = ret.erase(it2);
    }
    else {
      it2++;
    }
  }

  return ret;
}

// Define any helper functions here
void genRequirements(
    std::set<std::string>& ret,
    const std::string& options,
    std::string input,
    std::set<int> visited,
    unsigned int index
)   
{
  if (index == input.size() && ret.find(input) == ret.end()) {
    ret.insert(input);
  }
  else if (isalpha(input[index])) { //if we have a locked-in letter
    genRequirements(ret, options, input, visited, index+1);
  }
  else {
    for (unsigned int i = 0; i < options.size(); ++i) {
      if (visited.find(i) != visited.end()) { //if we've seen this floating number before
        continue;
      }
      visited.insert(i);
      input[index] = options[i];
      genRequirements(ret, options, input, visited, index+1);
      visited.erase(i);
    }
  }
}

void genAllStrs(
    const std::string& floating,
    std::set<std::string>& ret,
    std::string input,
    unsigned int index
) 
{
  if (index == input.size()) {
    ret.insert(input);
  }
  else if (isalpha(input[index])) {
    genAllStrs(floating, ret, input, index+1);
  }
  else {
    for (int i = 0; i < 26; ++i) {
      input[index] = char('a'+i);
      genAllStrs(floating, ret, input, index+1);
    }
  }
}

void getInitStr(
    std::string& options,
    const std::string& in,
    unsigned int remaining,
    unsigned int index
) 
{
  if (index == remaining) {
    return;
  }
  options+='_';
  getInitStr(options, in, remaining, index+1);
}

