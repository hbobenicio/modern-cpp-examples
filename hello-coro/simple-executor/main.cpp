#include <iostream>
#include <string_view>
#include <chrono>
#include <thread>
#include <coroutine>
#include <print>

#define eprintln(...) std::println(stderr, __VA_ARGS__)
#define trace()       eprintln(__func__)

struct TaskPromise;

struct Task: public std::coroutine_handle<TaskPromise> {
    using promise_type = ::TaskPromise;

    Task(std::coroutine_handle<TaskPromise> coro) : std::coroutine_handle<TaskPromise>(coro) {}

    virtual ~Task() {
        this->destroy();
    }
};

struct TaskPromise
{
    Task get_return_object() {
        return Task::from_promise(*this);
    }

    auto initial_suspend() noexcept {
        return std::suspend_always{};
    }

    auto final_suspend() noexcept {
        return std::suspend_always{};
        // destroy();
        // return std::suspend_never{};
    }

    void return_void() {
        //noop - Tasks do not return anything
    }

    void unhandled_exception() {
        // just rethrow the exception
        throw;
    }
};

struct AnswerAwaitable {
private:
    int answer = 0;
    bool is_done = false;

public:
    /**
     * Callback called on the start of an co_await expression, intended to indicate whether we should
     * suspend execution (return false -> we're not ready yet) 
     */
    constexpr bool await_ready() const noexcept {
        return is_done;
    }

    void await_suspend(std::coroutine_handle<> handle)
    {
        // really naive... would be better implemented inside a real scheduler
        std::cerr << "info: calculating the answer to life, the universe and everything...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3'000));
        this->answer = 42;
        std::cerr << "info: done calculating... will resume the coroutine with the answer now.\n";

        //TODO consider creating the answer state in a promise and copy it here with
        // handle.promise().set_answer(42);
        //Problems specializing our Task for that?

        this->is_done = true;
        handle.resume();
    }

    int await_resume() const noexcept {
        return this->answer;
    }

    // AnswerTask(std::coroutine_handle<promise_type> handle)       : handle(handle) {}
    // //AnswerTask(std::coroutine_handle<Task::promise_type> handle) : handle() {}
    // std::coroutine_handle<promise_type> handle;
};

//NOTE this is not a coroutine... it just creates an Awaitable object
AnswerAwaitable get_answer() {
    return AnswerAwaitable{};
}

Task run() {
    int x = co_await get_answer();
    std::println("Answer to Life, The Universe and Everything: {}", x);
    co_return;
}

int main() {
    Task task = run();

    // dummy naive scheduler
    while (!task.done()) {
        task.resume();
    }
}
