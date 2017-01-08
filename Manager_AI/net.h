#pragma once
#include "manager.h"
#include <math.h>

#ifndef net_h
#define net_h
class neural_net {
public:
	manager * info;
	int layer_Count;
	std::vector<float> uncrush_nodes;
	std::vector<float> h_nodes;
	//each hidden node has weight for each variable
	std::vector<std::vector<float>> weight_in;
	std::vector<float> weight_out;

	//function to link the neural net to the manager
	void link(manager * t) {
		info = t;
	}
	//must be called after srand is initialized
	void create_weights() {
		//each input needs a weight to each hidden node on the first layer
		//the number of variables in the system is equiv to the number of parts ot a decision
		//so for each input
		std::vector<float> empty;
		for (int i = 0; i < info->variables.size(); i++) {
			//generate an empty array for this input
			
			weight_in.push_back(empty);
			//now fill the array with weights to each hidden node on 1st layer
			for (int j = 0; j < h_nodes.size(); j++) {
				float tmp = rand() % 100000 + 0;
				tmp = tmp / 100000;
				weight_in[i].push_back(tmp);
			}
		}

		//each hidden node gets one weight to the output
		for (int k = 0; k < h_nodes.size(); k++) {
			float tmp = rand() % 100000 + 0;
			tmp = tmp / 100000;
			weight_out.push_back(tmp);
		}
		//for (int j = 0; j < h_nodes.size(); j++) {
		//	std::vector<float> empty;
		//	weight_out.push_back(empty);
		//	weight_in.push_back(empty);
		//	for (int i = 0; i < info->variables.size(); i++) {
		//		float tmp = rand() % 100000 + (000000);
		//		tmp = tmp / 100000;
		//		//input and ouput weights are only necessary for a single layer system
		//		if (layer_Count < 2) {
		//			weight_in[j].push_back(tmp);
		//		}
		//		tmp = rand() % 100000 + (000000);
		//		tmp = tmp / 100000;
		//		weight_out[j].push_back(tmp);
		//	}
		//}
	}
	

	void create_nodes(int count) {
		//creates count number of nodes and sets their values to zero
		for (int i = 0; i < count; i++) {
			h_nodes.push_back(0);
			uncrush_nodes.push_back(0);
		}
	}

	//finds how wrong a decision was
	float calculate_total_error(decision made) {
		//very simple v1 takes the sum of all decision properties
		float alg_result = 0;

		//finds the net change
		//designated alg
		//currently sum of all effects
		//for (int i = 0; i < made.effects.size(); i++) {
		///////////////////////////////////////////////////
		int x = made.effects[0];
		int z = made.effects[1];
		int a = made.effects[2];
			alg_result = (((5*x) * (2 * (z^2))) / (a^4));
			////////////////////////////////////////////////
	//	}


		//save original number
		float crush_alg = alg_result;
		//crush function to between 0 and 1
		crush_alg = (crush_alg / (1 + abs(crush_alg)));

		float net_result = 0;
		if (crush_alg == 0) {
			std::cout << "hah";
		}
		net_result = run_data(made);
		
		float delta_error = 0;
		//note net result has already been crushed by run_data
		//find the delta of the two crushed results
		delta_error = crush_alg - net_result;

		//find the derivative of the algorithm @ alg_result
		//alg_result = 1 / ((abs(alg_result) +1) ^2)
		float tmp = 0;
		tmp = abs(alg_result);
		tmp = tmp + 1;
		tmp = tmp * tmp;
		tmp = 1 / tmp;

		//derivative multiplied by delta_error
		float error_correction = 0;
		error_correction = tmp * delta_error;

		//update weights for output layer

		std::cout << "Algorithm: " << crush_alg << std::endl;
		std::cout << "Network: " << net_result << std::endl;
		std::cout << "Error: " << delta_error << std::endl;
		std::cout << "Error Correction: " << error_correction << std::endl;
		info->data << delta_error << std::endl;
		//return the total margin of error
		return error_correction;
	}

	//function that updates the weights to account for error
	void back_propagate(float total_error, decision current) {
		//update weight out to output layer using error correction
		// error correction / respective hidden layer results
		for (int i = 0; i < h_nodes.size(); i++) {
			//0 because there is only one output to each nodes only has one weight leaving it
			//divide error correction passed to func by current weight and then add to weight
			weight_out[i] += (total_error / weight_out[i]);
		}
		
		//update weight in to 1st hidden node layer
		//this is more complicated since the effects of this error correction on the weight out must be considered
		float tmp_error = 0;
		for (int i = 0; i < h_nodes.size(); i++) {
			//get derivative of uncrushed
			float tmp = uncrush_nodes[i];
			tmp = abs(tmp);
			tmp = tmp + 1;
			tmp = tmp * tmp;
			tmp = 1 / tmp;
			float modifier = total_error * h_nodes[i] * tmp;
			//change in weights == modifier / input data
			for (int j = 0; j < current.effects.size(); j++) {
				weight_in[j][i] += (modifier) / current.effects[j];
			}
			//modifier = modifier / 
			//tmp_error = total_error / weight_out[i][0] * ;
		}
	}

	//float calculate_partial_error(decision made, int place) {
	//	//complicated algorithm
	//	//finds the net change
	//	float total = 0;
	//	//designated alg
	//	//currently sum of all effects
	//	for (int i = 0; i < made.effects.size(); i++) {
	//		total += made.effects[i];
	//	}
	//	float change = -(total - h_nodes[place]) * (h_nodes[place] * (1 - h_nodes[place]));
	//	//subtract calculated value from current weight of node
	//	//multiply change by learning rate to increase training
	//	h_nodes[place] = h_nodes[place] - change;
	//	//total = (1/2)(total - )
	//}

	//passes info through net and arrives at a conclusion
	bool make_decision(decision in) {
		//loops through all variables 
		for (int i = 0; i < info->variables.size(); i++) {
			//create inputs based on preprocessing for each one of variables
			// true input is based on the current status of said variable, how the decision affects it, and any special weight assigned from past information
			info->variables[i].true_input = (in.effects[i] * (1 / info->variables[i].status)) + (in.effects[i] * info->variables[i].weight);
		}

		float tmp = 0;
		for (int j = 0; j < h_nodes.size(); j++) {

		//take the true input and the weight to the first hidden layer, then assign it as the weight to the hidden node
		for (int i = 0; i < info->variables.size(); i++) {
			tmp += info->variables[i].true_input * weight_in[j][i];
		}
		h_nodes[j] = tmp;
	}
		//run activation function on hidden node value
		for (int i = 0; i < h_nodes.size(); i++) {
			//fast sigmoid
			h_nodes[i] = (h_nodes[i] / (1 + abs(h_nodes[i])));
			//if negative invert it
			//so instead of getting a -.95 you get .05
			if (h_nodes[i] < 0) {
				h_nodes[i] = h_nodes[i] + 1;
			}
		}

		// add all hidden node values together
		float sum = 0;
		for (int i = 0; i < h_nodes.size(); i++) {
			
			sum += h_nodes[i];
		}

		//run activation on the sum of hidden nodes
		//sum = (sum / (1 + abs(sum)));

		//compare result against decision threshold

		std::cout << sum << std::endl;
		//returns true if decision should be made
		if (sum > info->threshold) {
			return true; 
		}
		else {
			return false;
		}
	}

	//this function is essentially an update of makedecision for iteration 3
	//but instead of returning true false it returns the calculated number for the network
	//passes info through net and arrives at a conclusion
	float run_data(decision in) {

		float tmp = 0;
		for (int j = 0; j < h_nodes.size(); j++) {

			//take the true input and the weight to the first hidden layer, then assign it as the weight to the hidden node
			for (int i = 0; i < info->variables.size(); i++) {
				tmp += in.effects[i] * weight_in[i][j];
			}
			h_nodes[j] = tmp;
		}
		//run activation function on hidden node value
		for (int i = 0; i < h_nodes.size(); i++) {
			//save uncrushed
			uncrush_nodes[i] = h_nodes[i];
			//fast sigmoid
			h_nodes[i] = (h_nodes[i] / (1 + abs(h_nodes[i])));
			////if negative invert it
			////so instead of getting a -.95 you get .05
			//if (h_nodes[i] < 0) {
			//	h_nodes[i] = abs(1 /h_nodes[i]);
			//}
		}

		// sum of all (hidden node values * output weight) 
		float sum = 0;
		for (int j = 0; j < h_nodes.size(); j++) {
			//for (int i = 0; i < h_nodes.size(); i++) {

				sum += (h_nodes[j] * weight_out[j]);
			//}
		}

		//run activation on the sum of hidden nodes - output layer
		sum = (sum / (1 + abs(sum)));

		

		return sum;
	}
};

#endif 