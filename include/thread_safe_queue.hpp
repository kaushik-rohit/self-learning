#include <vector>
#include <mutex>
class ThreadSafeQueue {
public:
    int front();
    void pop();
    void push(int val);
    int size();
    bool empty();

private:
    std::vector<int> data;
    mutable std::mutex mtx_;        // mutable for const methods
};
