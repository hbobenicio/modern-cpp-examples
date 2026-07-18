#include <iostream>
#include <chrono>
#include <thread>
#include <coroutine>

using std::cout;

struct Task
{
    struct promise_type
    {
        Task get_return_object() {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend()   noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    Task(std::coroutine_handle<promise_type> handle)
        : handle(handle)
    {}

    std::coroutine_handle<promise_type> handle;
};

struct AnswerAwaitable {
private:
    int answer = 0;

public:
    /**
     * Callback called on the start of an co_await expression, intended to indicate whether we should
     * suspend execution (return false -> we're not ready yet) 
     */
    constexpr bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) {
        // really naive... would be better implemented inside a real scheduler
        std::cerr << "info: calculating the answer to life, the universe and everything...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3'000));
        this->answer = 42;
        std::cerr << "info: done calculating... will resume the coroutine with the answer now.\n";

        //TODO consider creating the answer state in a promise and copy it here with
        // handle.promise().set_answer(42);
        //Problems specializing our Task for that?

        handle.resume();
    }

    int await_resume() const noexcept {
        return this->answer;
    }

    // AnswerTask(std::coroutine_handle<promise_type> handle)       : handle(handle) {}
    // //AnswerTask(std::coroutine_handle<Task::promise_type> handle) : handle() {}
    // std::coroutine_handle<promise_type> handle;
};

AnswerAwaitable get_answer() {
    return AnswerAwaitable{};
}

Task run() {
    int x = co_await get_answer();
    cout << "Answer to Life, The Universe and Everything: " << x << '\n';
    co_return;
}

int main() {
    auto task = run();
    //TODO scheduler: block_on to check if tasks have finished
}

