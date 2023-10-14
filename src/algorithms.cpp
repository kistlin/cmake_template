#include "algorithms.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

void print_container(const std::string& label, const std::vector<int>& v);

void algorithms() {
	std::vector<int> c = {1, 2, 3, 4, 5, 6, 7};
	int x = 5;
	// remove-erase idiom
	c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());

	print_container("c: ", c);

	// copy_if
	std::vector<int> to_vector;
	std::copy_if(
	    c.begin(), c.end(),
	    std::back_inserter(to_vector),  // used to append at the end, else vector is too small and exception happens
	    [](int x) { return x > 5; });
	print_container("to_vector: ", to_vector);

	std::vector<int> v1 = {1, 2, 3, 4, 5};
	auto it =
	    std::remove_if(v1.begin(), v1.end(), std::bind2nd(std::greater<int>(), 3));  // bind2nd deprecated in C++11
	v1.erase(it, v1.end());
	print_container("v1: ", v1);

	// find_first_of
	std::vector<int> v2{0, 2, 3, 25, 5};
	const auto t2 = {3, 19, 10, 2};
	auto result2 = std::find_first_of(v2.begin(), v2.end(), t2.begin(), t2.end());
	if (result2 == v2.end()) {
		std::cout << "no elements of v were equal to 3, 19, 10 or 2\n";
	}
	else {
		std::cout << "found a match at " << std::distance(v2.begin(), result2) << "\n";
	}

	std::unordered_map<int, std::string> u_map1 = {{1, "1"}, {2, "2"}};
	std::unordered_map<int, std::string> t_u_map1 = {{2, "2"}};
	auto result3 = std::find_first_of(u_map1.begin(), u_map1.end(), t_u_map1.begin(), t_u_map1.end());
	if (result3 != u_map1.end()) {
		std::cout << "found pair {" << result3->first << ": " << result3->second << "}\n";
	}
}

void print_container(const std::string& label, const std::vector<int>& v) {
	std::cout << label;
	for (auto const element: v) {
		std::cout << element << " ";
	}
	// std::for_each(c.begin(), c.end(), [](int i){ std::cout << i << ' '; });
	std::cout << '\n';
}