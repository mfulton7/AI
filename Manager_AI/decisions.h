#include <time.h>
#ifndef decision_h
#define decision_h

//class for decison
struct decision {
  //list of effects for each variable
  std::vector<float> effects;

  //risk slot that can be assigned by system

};

//decision generator
class generator {
 public:



  //set up randomness and generator
  void initialize(int seed) {
    srand(seed);
    
  }

  //func to create a random set of decisions
  //takes an int as input and creates that many
  std::vector<decision> createset(int size , int varnum) {
    std::vector<float> set;
    //create size of set times number of variables per decision number of random numbers
    for (int j = 0; j < (size * varnum); j++) {
		int tmp = rand() % 100 + (1);
		//added because zero crashes system for some reason
		if (tmp == 0) {
			tmp = 1;
		}
      set.push_back(tmp);
    }
    std::vector<decision> result;
    for (int i = 0; i < size; i++) {
      decision tmp;
      int k;
      k = i * varnum;
      for (int j = (k); j < (k + varnum); j++) {
	//grabs the appropriate amount of variables
	tmp.effects.push_back(set[j]);
      }
      result.push_back(tmp);
    }

    return result;
  }

  //create a random decision based on the range passed to it
  //needs the number of variables in a system to create a full decision
  decision createdecision(int min, int max, int variables) {
    
    decision t;
    int range = max - min;
    
    //loop through however many variables
    for (int i = 0; i < variables; i++) {
		int tmp = rand() % 200 + (-100);
		//added because zero crashes system for some reason
		if (tmp == 0) {
			tmp = 1;
		}
      t.effects.push_back(tmp);
      
    }
    return t;
  }
};


#endif