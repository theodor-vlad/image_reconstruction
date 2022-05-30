#pragma once
#include <string>
#include <chrono>
#include "Chromosome.h"
#include "Genetic.h"
#include "Annealer.h"
#include "HCFI.h"
#include "HCBI.h"
#include <fstream>

class Serializer {

public:
	static enum algorithm_t {
		GENETIC = 0,
		HILLCLIMBER_BEST,
		HILLCLIMBER_FIRST,
		ANNEALER,
		NOT_SET
	};

private:
	algorithm_t algorithm;

public:

	Serializer() {
		algorithm = NOT_SET;
	}

	void set_algortihm(algorithm_t _algorithm) {
		algorithm = _algorithm;
	}

	bool serialize() {
		bool result = true;
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string datetime_s = tokenize_and_return_date(std::string(std::ctime(&now)));
		std::string path;
		std::string method_s;
		Chromosome solution;
		long long time_elapsed;

		switch (algorithm)
		{
		case GENETIC:
			path = "F:\\licenta\\image_processing\\dna\\ga\\" + datetime_s + ".json";
			solution = GA::population[0];
			method_s = "Genetic Algorithm";
			time_elapsed = GA::get_running_duration();
			break;
		case HILLCLIMBER_BEST:
			path = "F:\\licenta\\image_processing\\dna\\hcbi\\" + datetime_s + ".json";
			solution = HCBI::curr;
			method_s = "Hillclimbing - Best improvement";
			time_elapsed = HCBI::get_running_duration();
			break;
		case HILLCLIMBER_FIRST:
			path = "F:\\licenta\\image_processing\\dna\\hcfi\\" + datetime_s + ".json";
			solution = HCFI::curr;
			method_s = "Hillclimbing - First improvement";
			time_elapsed = HCFI::get_running_duration();
			break;
		case ANNEALER:
			path = "F:\\licenta\\image_processing\\dna\\sa\\" + datetime_s + ".json";
			solution = SA::curr;
			method_s = "Simulated Annealing";
			time_elapsed = SA::get_running_duration();
			break;
		default:
			result = false;
			break;
		}

		if (result == true) {
			std::ofstream outfile(path);
			outfile << "{\n" << "\"width\": " << IMG_WIDTH << ",\n";
			outfile << "\"height\": " << IMG_HEIGHT << ",\n";
			outfile << "\"path_to_image\": \"" << path_to_image << "\",\n";
			outfile << "\"approximation_method\": \"" << method_s << "\",\n";
			outfile << "\"time_elapsed\": " << time_elapsed << ",\n";
			outfile << "\"fitness\": \"" << solution.fitness << "\",\n";
			outfile << "\"chromosome\": ";
			outfile << solution;
			outfile << "\n}";
			outfile.close();
		}

		return result;
	}

	~Serializer() = default;

private:
	std::string tokenize_and_return_date(std::string str) {
		const char tok[5] = "\n :";
		char c_str[30];
		char* aux;

		std::string result = "";
		strcpy(c_str, str.c_str());

		aux = strtok(c_str, tok);
		result += aux;
		while (aux) {
			aux = strtok(NULL, tok);
			if (aux) {
				result += "_" + std::string(aux);
			}
		}

		return result;
	}
};
