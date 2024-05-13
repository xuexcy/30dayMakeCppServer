#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(int size=std::thread::hardware_concurrency());
    ~ThreadPool();

    template <class F, class... Args>
    auto add(F&& f, Args&&... args) ->
            std::future<typename std::invoke_result_t<F&&, Args&&...>>;

    auto size() const { return threads_.size(); }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_{false};
};

template <class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) ->
        std::future<typename std::invoke_result_t<F&&, Args&&...>> {
    using ReturnType = std::invoke_result_t<F&&, Args&&...>;
    auto shared_promise = std::make_shared<std::promise<ReturnType>>();
    auto task = [func = std::move(f), ...largs=std::move(args), p = shared_promise]() {
        if constexpr (std::is_same_v<ReturnType, void>) {
            func(largs...);
            p->set_value();
        } else {
            p->set_value(func(largs...));
        }
    };
    auto res = shared_promise->get_future();
    {
        std::unique_lock<std::mutex> lk(mtx_);
        if (stop_) {
            throw std::runtime_error("ThreadPool already stop, can't add task any more");
        }
        tasks_.emplace(std::move(task));
    }
    cv_.notify_one();
    return res;
}
