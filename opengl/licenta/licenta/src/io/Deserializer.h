#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "json.hpp"
#include "../building_blocks/Chromosome.h"

using json = nlohmann::json;

class Deserializer {
	Deserializer() = delete;
	
public:
	static double get_fitness(const std::string& path) {
		std::ifstream infile(path);
		std::stringstream buffer;
		buffer << infile.rdbuf();
		json j = json::parse(buffer.str());

		if (false == j.is_object() ||
			false == j.contains("fitness") ||
			false == j["fitness"].is_number_float()) {
			return -1.0;
		}
		return j["fitness"];
	}

	static int get_running_time(const std::string& path) {
		std::ifstream infile(path);
		std::stringstream buffer;
		buffer << infile.rdbuf();
		json j = json::parse(buffer.str());

		if (false == j.is_object() ||
			false == j.contains("time_elapsed") ||
			false == j["time_elapsed"].is_number_unsigned()) {
			return -1;
		}
		return j["time_elapsed"];
	}

	static std::string get_approximation_method(const std::string& path) {
		std::ifstream infile(path);
		std::stringstream buffer;
		buffer << infile.rdbuf();
		json j = json::parse(buffer.str());

		if (false == j.is_object() ||
			false == j.contains("approximation_method") ||
			false == j["approximation_method"].is_string()) {
			return "Unknown approximation method";
		}
		return j["approximation_method"];
	}

	static bool get_and_set_screen_dimensions(const std::string& path) {
		std::ifstream infile(path);
		std::stringstream buffer;
		buffer << infile.rdbuf();
		json j = json::parse(buffer.str());

		if (false == j.is_object() ||
			false == j.contains("width") ||
			false == j.contains("height") ||
			false == j["width"].is_number_unsigned() ||
			false == j["height"].is_number_unsigned()) {
			return false;
		}
		IMG_WIDTH = j["width"];
		IMG_HEIGHT = j["height"];
		return true;
	}

	static Chromosome reconstruct_solution_from_file(const std::string& path) {
		std::ifstream infile(path);
		std::stringstream buffer;
		buffer << infile.rdbuf();
		json j = json::parse(buffer.str());

		if (false == j.contains("chromosome") || false == j["chromosome"].is_array()) {
			return Chromosome();
		}
		j = j["chromosome"];

		Chromosome solution;
		for (auto poly_it = j.begin(); poly_it != j.end(); poly_it++) {

			if (false == (*poly_it).is_object() ||
				false == (*poly_it).contains("vertices") ||
				false == (*poly_it)["vertices"].is_array() ||
				false == (*poly_it).contains("color") ||
				false == (*poly_it)["color"].is_object()) {
				return Chromosome();
			}

			Poly to_be_inserted;
			for (auto vertex_it = (*poly_it)["vertices"].begin(); vertex_it != (*poly_it)["vertices"].end(); vertex_it++) {

				if (false == (*vertex_it).contains("x") || false == (*vertex_it).contains("y")) {
					return Chromosome();
				}

				double x = (*vertex_it)["x"];
				double y = (*vertex_it)["y"];
				to_be_inserted.vertices.push_back(Point(x, y));
			}

			if (false == (*poly_it)["color"].contains("r") ||
				false == (*poly_it)["color"]["r"].is_number_float() ||
				false == (*poly_it)["color"].contains("g") ||
				false == (*poly_it)["color"]["g"].is_number_float() ||
				false == (*poly_it)["color"].contains("b") ||
				false == (*poly_it)["color"]["b"].is_number_float() ||
				false == (*poly_it)["color"].contains("a") ||
				false == (*poly_it)["color"]["a"].is_number_float()) {
				return Chromosome();
			}

			double r = (*poly_it)["color"]["r"];
			double g = (*poly_it)["color"]["g"];
			double b = (*poly_it)["color"]["b"];
			double a = (*poly_it)["color"]["a"];
			to_be_inserted.color = Color(r, g, b, a);

			solution.polygons.push_back(to_be_inserted);
		}

		return solution;
	}
};