#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "decisions.h"
#include <fstream>
#include <thread>

//#include <pthread.h>
#ifndef manager_h
#define manager_h


// class for defining and storing information about system variables
class constituent {

 public:

  //current status of variable
  float status;
  //current weight of how system treats variable in calculations
  float weight;
  float modifier;

  //final input after preproccessing
  float true_input;

  //name of variable
  std::string name;
  //brief description of what the variable is and why it matters to system
  std::string info;

  //method to update the modifier of a variable based on current stat
  void update_modifier(){
    modifier  = 1/status;
}

  void update_weight(){}

  //constructor
  constituent() {
    status = 100;
    weight = 1;
    //    update_weight();
    update_modifier();
    name = " test";
    info = " ???? ";
  }
  
};

// class for governing whole system
// contains all goals and stats for system
class manager {
 public:
  std::string name;

  //flag for wheather to export results to text file
  bool track;

  //threshold for decision making
  float threshold;

  //flag for warning messages
  bool warn = true;

  //current set
  std::vector<decision> dset;
  //currently selected decision
  int ds;

  //penalty for no
  int penalty = -10;

  //container to hold all variables that the system must govern
  std::vector<constituent> variables;


  //output file
  std::ofstream data;

  //container to hold all goals that the system aims to accomplish
  //std::vector<> goals;

  //used for startup to get parameters for system
  void initialize() {
    track = false;
    std::cout << "Please set a name for the system..." << std::endl;
    std::cin >> name;
    std::cout << "Please input the number of variables for " << name << " to monitor..."  << std::endl;
    int tmp;
    std::cin >> tmp;
    for (int i = 0; i < tmp; i++) {
      constituent j;
      variables.push_back(j);
    }
    ds = 0;
    std::cout<<"Please enter the decision threshold for the system... " << std::endl;
    float t;
    std::cin >> t;
    threshold = t;
    
    //open text output file
    data.open("log.txt");
    if(data.is_open()){
    data << "Data export file: \n";
      }
      else{
	std::cout << "Error Unable to establish connection to log file" << std::endl;      
}
    
  }

  //status
  std::string full_status(constituent v) {
    std::string tmp;
    tmp = "Name:\t";
    tmp += v.name;
    tmp += "\t";
    tmp += "Info:\t";
    tmp += v.info;
    tmp += "\t";
    tmp += "Status:\t";
    tmp += std::to_string(v.status);
    tmp += "\t";
    tmp += "Weight:\t";
    tmp += std::to_string(v.weight);
    tmp += "\n";
    return tmp;

  }

  //alter variables according to decision
  void execute_yes(decision d) {
	  for (int i = 0; i < variables.size(); i++) {
		  variables[i].status += d.effects[i];		  
	  }
	  
  }

  void execute_no(decision d) {
	  for (int i = 0; i < variables.size(); i++) {
		  variables[i].status += penalty;
	  }
  }

  //print current status of system const
  void const_status() {
    std::cout << "There are currently " << variables.size() << " in " << name << " ...\n";
    for (int i = 0; i < variables.size(); i++) {
      std::cout << full_status(variables[i]);
    }
  }

  //saves current status of system to file
  void log_data() {
    for (int i = 0; i < variables.size(); i++) {
      data << full_status(variables[i]);
    }
  }


  //check for system integrity
  bool systems_check(){
    for(int i = 0; i< variables.size(); i++){
      if(variables[i].status < 0){
	data << "Critical systems failure detected.... " << std::endl;
	std::cout << "Critical systems failure detected.... " << std::endl;
	//full_status();
	return false;
       }
      else{
	continue;
      }
   } 
    return true;
}


};

#endif