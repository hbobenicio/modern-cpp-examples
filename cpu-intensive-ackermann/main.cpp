#include <iostream>
#include <functional>
#include <cstdint>
#include <chrono>
#include <thread>
#include <array>

static uintmax_t ackermann(uintmax_t m, uintmax_t n) {
    if (m == 0) {
        return n + 1;
    }
    if (n == 0) {
        return ackermann(m - 1, 1);
    }
    return ackermann(m - 1, ackermann(m, n - 1));
}

static void run_ackermann(int id, int count, uintmax_t m, uintmax_t n) {
    for (int i = 0; i < count; i++) {
        uintmax_t result = ackermann(m, n);
        std::cout << id << ':' << i << ": " << result << '\n';
    }
}

template<typename Callable>
static auto measure_time(Callable f) {
    auto start = std::chrono::steady_clock::now();

    std::invoke(f);

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return elapsed;
}

static void run(int count, int num_threads, int m, int n) {
    std::vector<std::thread> pool;
    pool.reserve(num_threads);

    for (int tid = 0; tid < num_threads; tid++) {
        pool.emplace_back(run_ackermann, tid, count, m, n);
    }

    for (auto& t: pool) {
        t.join();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "você não passou 4 argumentos obrigatórios (count, num_threads, m e n)\n";
        return EXIT_FAILURE;
    }

    // Código inseguro. Não use em produção!
    int count = std::atoi(argv[1]);
    int num_threads = std::atoi(argv[2]);
    int m = std::atoi(argv[3]);
    int n = std::atoi(argv[4]);

    auto elapsed = measure_time([=](){
        run(count, num_threads, m, n);
    });

    std::cout << "total elapsed time: " << elapsed << "ms\n";
}
