#include <iostream>
#include <array>
#include <tuple>
#include <random>
#include <vector>
#include <functional>
#include <numeric>
#include <cassert>

namespace training {

    template <typename T>
    struct Item {
        std::vector<T> inputs;
        bool expected_output;
    };
}

namespace logicalports::andport {
    struct TrainingItem: public training::Item<int> {
        TrainingItem(int x, int y, bool expected_output) {
            this->inputs.push_back(x);
            this->inputs.push_back(y);
            this->expected_output = expected_output;
        }
    };

    static std::vector<TrainingItem> create_training_set() {
        std::vector<TrainingItem> training_set;
        training_set.emplace_back(0, 0, false);
        training_set.emplace_back(0, 1, false);
        training_set.emplace_back(1, 0, false);
        training_set.emplace_back(1, 1, true);

        return training_set;
    }
}

static std::pair<double, bool> feed_foward(const std::vector<int>& xs, const std::vector<double>& ws);

int main() {
    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::uniform_real_distribution dis{ 0.0, 1.0 };

    std::vector<double> weights = { dis(gen), dis(gen) };

    std::cout << "weights: " << weights[0] << ", " << weights[1] << "\n\n";

    const auto training_set = logicalports::andport::create_training_set();

    for (const auto& training_item: training_set) {
        std::cout << "input: ";
        for (const auto& x: training_item.inputs) {
            std::cout << x << ',';
        }
        std::cout << '\n';
        std::cout << "expected: " << training_item.expected_output << '\n';
        
        const auto [sum, actual_output] = feed_foward(training_item.inputs, weights);
        std::cout << "output: " << actual_output << '\n';

        // TODO calculate error and back propagate to adjust the weights
        std::cout << '\n';
    }
}

static std::pair<double, bool> feed_foward(const std::vector<int>& xs, const std::vector<double>& ws) {
    assert(xs.size() == ws.size());

    double total = 0.0;
    for (int i = 0; i < xs.size(); i++) {
        total += xs[i] * ws[i];
    }

    bool active = (total / xs.size()) >= 0.5;

    return { total, active };
}
