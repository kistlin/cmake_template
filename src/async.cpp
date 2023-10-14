#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

int async_example_1() {
	// future from a packaged_task
	std::packaged_task<int()> task([] { return 7; });  // wrap the function
	std::future<int> f1 = task.get_future();           // get a future
	std::thread t(std::move(task));                    // launch on a thread

	// future from an async()
	std::future<int> f2 = std::async(std::launch::async, [] { return 8; });

	// future from a promise
	std::promise<int> p;
	std::future<int> f3 = p.get_future();
	std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

	std::cout << "Waiting..." << std::flush;
	f1.wait();
	f2.wait();
	f3.wait();
	std::cout << "Done!\nResults are: " << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
	t.join();
	return 0;
}

std::mutex m;
struct X {
	void foo(int i, const std::string& str) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << ' ' << i << '\n';
	}
	void bar(const std::string& str) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << '\n';
	}
	int operator()(int i) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << i << '\n';
		return i + 10;
	}
};

template<typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end) {
	auto len = end - beg;
	if (len < 1000)
		return std::accumulate(beg, end, 0);

	RandomIt mid = beg + len / 2;
	auto handle = std::async(std::launch::async, parallel_sum<RandomIt>, mid, end);
	int sum = parallel_sum(beg, mid);
	return sum + handle.get();
}

int async_example_2() {
	std::vector<int> v(10000, 1);
	std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';

	X x;
	// Calls (&x)->foo(42, "Hello") with default policy:
	// may print "Hello 42" concurrently or defer execution
	auto a1 = std::async(&X::foo, &x, 42, "Hello");
	// Calls x.bar("world!") with deferred policy
	// prints "world!" when a2.get() or a2.wait() is called
	auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");
	// Calls X()(43); with async policy
	// prints "43" concurrently
	auto a3 = std::async(std::launch::async, X(), 43);
	a2.wait();                      // prints "world!"
	std::cout << a3.get() << '\n';  // prints "53"
	return 0;
}  // if a1 is not done at this point, destructor of a1 prints "Hello 42" here
