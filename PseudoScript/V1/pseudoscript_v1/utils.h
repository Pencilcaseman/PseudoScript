#pragma once

// STL includes
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <functional>
#include <map>
#include <regex>
#include <chrono>

#define TIME ((double) std::chrono::high_resolution_clock().now().time_since_epoch().count() / 1000000)
