#pragma once

#include <functional> // std::bind, std::placeholders

// @Todo figure out lambda's and replace std::bind
// https://github.com/TheCherno/Hazel/pull/277
//   variadic args in lambdas were added in C++17
#define NF_BIND_EVENT(f) std::bind(&f, this, std::placeholders::_1)
