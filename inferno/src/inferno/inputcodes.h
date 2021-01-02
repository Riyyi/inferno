#ifndef INPUTCODES_H
#define INPUTCODES_H

#define MAP_KEY(key) #key, key

#include <unordered_map> // std::unordered_map

namespace Inferno {

	extern std::unordered_map<const char*, int> KeyCode;

}

#endif // INPUTCODES_H
