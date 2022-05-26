#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "json.hpp"
#include "Chromosome.h"

using json = nlohmann::json;

class Deserializer {
	Deserializer() = delete;
	
public:
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
			std::cout << "Invalid json 0\n";
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
			std::cout << "Invalid json 1\n";
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
				std::cout << "Invalid json 2\n";
				return Chromosome();
			}

			Polygon to_be_inserted;
			for (auto vertex_it = (*poly_it)["vertices"].begin(); vertex_it != (*poly_it)["vertices"].end(); vertex_it++) {

				if (false == (*vertex_it).contains("x") ||
					false == (*vertex_it)["x"].is_number_float() ||
					false == (*vertex_it).contains("y") ||
					false == (*vertex_it)["y"].is_number_float()) {
					std::cout << "Invalid json 3\n";
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
				std::cout << "Invalid json 4\n";
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