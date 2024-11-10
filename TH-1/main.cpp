#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <vector>
#include <chrono>
#include <thread>
#include <numeric>

void sumArrayPart(const std::vector<int>& array, size_t start, size_t end, long long& partial_sum) {
    partial_sum = std::accumulate(array.begin() + start, array.begin() + end, 0LL);
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "You input wrong quantity numbers!";
		exit(errno);

	}

	size_t N = std::atoi(argv[1]);
	size_t M = std::atoi(argv[2]);
	
	std::vector<int> array(N);
	for (int i = 0; i < N; i++) {
		array[i] = random() % 100;
	}

	auto start_time = std::chrono::high_resolution_clock::now();
    	long long sum = std::accumulate(array.begin(), array.end(), 0LL);
    	auto end_time = std::chrono::high_resolution_clock::now();
    	std::chrono::duration<double> duration = end_time - start_time;
    	std::cout << "Time spent without threads: " << duration.count() << " seconds\n";
	
	start_time = std::chrono::high_resolution_clock::now();
    	std::vector<std::thread> threads;
    	std::vector<long long> partial_sums(M, 0);
    	size_t chunk_size = N / M;
	
	for (size_t i = 0; i < M; ++i) {
        	size_t start = i * chunk_size;
        	size_t end = (i == M - 1) ? N : start + chunk_size;
        	threads.emplace_back(sumArrayPart, std::cref(array), start, end, std::ref(partial_sums[i]));
    	}

	for (auto& thread: threads) {
		thread.join();
    	}

	long long total_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
    	end_time = std::chrono::high_resolution_clock::now();
    	duration = end_time - start_time;
    	std::cout << "Time spent with " << M << " threads: " << duration.count() << " seconds\n";
    
	return 0;
}
