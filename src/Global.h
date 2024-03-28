#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>

// wymiar długości boku kwadratowego kafelka w pikselach
const unsigned short tile_size = 100;

struct Button_data {
	std::string img_file_path;
	std::wstring desc;
	
	std::string id_name;	///< obecnie nieużywane
};

enum Direction {
	left,
	right,
	up,
	down
};

#endif /* GLOBAL_H_ */