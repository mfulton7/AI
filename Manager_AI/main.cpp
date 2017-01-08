#include "manager.h"
#include "net.h"




int main() {
	manager brain;
	generator jenny;
	neural_net neurons;

	brain.initialize();
	jenny.initialize(time(NULL));
	neurons.link(&brain);
	neurons.create_nodes(10);
	neurons.create_weights();


	//global flags
	//flag to tell if the program should be accepting input
	bool can_io = true;
	//flag to tell if the current process should be stopped
	bool kill = false;


	//input loop
	std::string input;
	while (input != "exit") {
		std::cout << brain.name << ": ";
		std::getline(std::cin, input);

		//displays list of commands and what they can do
		if (input == "help") {
			std::cout << "full status report -> " << "print full status of system variables" << std::endl;
			std::cout << "exit -> close program" << std::endl;
			std::cout << "help -> lists system commands" << std::endl;
			std::cout << "create decision -> creates a sample decision" << std::endl;
			std::cout << "print decision -> prints currently selected decision to console" << std::endl;
			std::cout << "print all -> prints all decisions in system" << std::endl;
			std::cout << "create set -> creates a set of 10 decisions" << std::endl;
			std::cout << "print variable -> prints the current number variables governed" << std::endl;
			std::cout << "print numbers -> prints random numbers" << std::endl;
			std::cout << "execute decision -> causes the currently selected decision to effect variables" << std::endl;
			std::cout << "print number -> prints the current decision identifier " << std::endl;
			std::cout << "execute all -> causes all decisions in the set to be executed" << std::endl;
			std::cout << "increment number -> adds one to the current decision identifier" << std::endl;
			std::cout << "decrement number -> subtracts one from the current decision identifier" << std::endl;
			std::cout << "make decision -> returns whether or not the manager thinks the decision should be made" << std::endl;
			std::cout << "process set -> makes and executes decisions for all decisions in set" << std::endl;
			std::cout << "toggle warnings -> turns warnings for system integrity on/off" << std::endl;
		}
		//////////////////////////////////////////////
		else if (input == "full status report") {
			brain.const_status();
		}
		//////////////////////////////////////////////
		else if (input == "create decision") {
			brain.dset.push_back(jenny.createdecision(-100, 100, brain.variables.size()));
			brain.ds = brain.dset.size() - 1;
		}
		//////////////////////////////////////////////
		else if (input == "create set") {
			std::cout << "Please input the size of the set... " << std::endl;
			int size;
			std::cin >> size;
			brain.dset = jenny.createset(size, brain.variables.size());
			brain.ds = 0;
		}
		////////////////////////////////////////////
		else if (input == "print decision") {
			for (int i = 0; i < brain.dset[brain.ds].effects.size(); i++) {
				std::cout << brain.dset[brain.ds].effects[i];
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		///////////////////////////////////////////////
		else if (input == "print all") {
			for (int k = 0; k < brain.dset.size(); k++) {
				for (int i = 0; i < brain.dset[brain.ds].effects.size(); i++) {
					std::cout << brain.dset[k].effects[i];
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
		////////////////////////////////////////////
		else if (input == "print variable") {
			std::cout << "There are " << brain.variables.size() << " currently governed by the system" << std::endl;
		}
		/////////////////////////////////////////
		else if (input == "print numbers") {
			int i = 0;
			while (i < 10) {
				std::cout << (rand() % 20 + (-10)) << std::endl;
				i++;
			}
		}
		///////////////////////////////////////
		else if (input == "execute decision") {
			/*for (int i = 0; i < brain.variables.size(); i++) {
		  brain.variables[i].status += brain.dset[brain.ds].effects[i];
			}*/
			brain.execute_yes(brain.dset[brain.ds]);
			if (!brain.systems_check()) {
				std::cout << "Warning Critical System Failure Detected!" << std::endl;
			}
		}
		////////////////////////////////////////
		else if (input == "print number") {
			std::cout << "Current decision identifier " << brain.ds << " out of " << brain.dset.size();
		}
		//////////////////////////////////////////
		else if (input == "execute all") {
			for (int j = 0; j < brain.dset.size(); j++) {
				/*for (int i = 0; i < brain.variables.size(); i++) {
				  brain.variables[i].status += brain.dset[brain.ds].effects[i];
				}*/
				brain.execute_yes(brain.dset[brain.ds]);
				if (!brain.systems_check()) {
					std::cout << "Warning Critical System Failure Detected!" << std::endl;
				}
				//log data
				brain.log_data();
			}
		}
		///////////////////////////////////////////
		else if (input == "increment number") {
			brain.ds++;
		}
		////////////////////////////////////////////
		else if (input == "decrement number") {
			brain.ds--;
		}
		////////////////////////////////////////////
		else if (input == "enable tracking") {
			brain.track = true;
			std::cout << "Tracking is now enabled, each change to variables will be recorded to an external file." << std::endl;
		}
		/////////////////////////////////////////////
		else if (input == "disable tracking") {
			brain.track = false;
			std::cout << "Tracking is now disabled." << std::endl;
		}
		//////////////////////////////////////////////
		else if (input == "tracking status") {
			std::cout << brain.track << std::endl;
		}
		//////////////////////////////////////////////
		else if (input == "make decision") {
			bool result = neurons.make_decision(brain.dset[brain.ds]);
			if (result) {
				std::cout << "The answer is yes." << std::endl;
			}
			else {
				std::cout << "The answer is no." << std::endl;
			}
		}
		/////////////////////////////////////////////////
		else if (input == "process set") {
			//for each decision
			for (int i = 0; i < brain.dset.size(); i++) {
				//if the decision is being made  
				if (neurons.make_decision(brain.dset[brain.ds])) {

					std::cout << "Yes" << std::endl;
					/*  for (int i = 0; i < brain.variables.size(); i++) {
						brain.variables[i].status += brain.dset[brain.ds].effects[i];
					  }*/
					brain.execute_yes(brain.dset[brain.ds]);
					for (int i = 0; i < neurons.h_nodes.size(); i++) {
						//neurons.calculate_partial_error(brain.dset[brain.ds], i);
					}

					if (brain.track) {
						brain.log_data();
					}

				}
				//if the decision is not being made
				else {
					std::cout << "No" << std::endl;
					brain.execute_no(brain.dset[brain.ds]);
				}
				if (!brain.systems_check()) {
					std::cout << "Failure on line " << i << std::endl;
					break;
				}
				brain.ds++;
			}

		}
		////////////////////////////////////////////////////////////
		else if (input == "toggle warnings") {
			brain.warn = !brain.warn;
		}
		/////////////////////////////////////////////////////////////
		else if (input == "print weights") {
			//for (int i = 0; i< neurons.we)
		}
		/////////////////////////////////////////////////////////////
		else if (input == "test") {
				neurons.back_propagate(neurons.calculate_total_error(brain.dset[brain.ds]), brain.dset[brain.ds]);
		}

		else if (input == "test set") {
			for (int i = 0; i < brain.dset.size(); i++) {
				neurons.back_propagate(neurons.calculate_total_error(brain.dset[brain.ds]), brain.dset[brain.ds]);
				brain.ds++;
			}
		}

		/////////////////////////////////////////////////
		else {
			std::cout << "Error unable to parse input" << std::endl;
			continue;
		}


	}
}
