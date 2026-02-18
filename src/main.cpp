#include <iostream>
#include <variant>

using ReturnType = std::variant<int, bool>;
struct IOperation {
    virtual ReturnType ResultOf(int* beg, int* end) = 0;
    virtual ~IOperation()                           = default;
};

class Operation : public IOperation {
  public:
    virtual ReturnType ResultOf(int* beg, int* end) {

    };
};

class BoolOperation : public IOperation {
  public:
    virtual ReturnType ResultOf(int* beg, int* end) override {}
};

void Operate(IOperation* op) {
    using namespace std;
    int  arr[5]{};
    auto result = op->ResultOf(arr, arr + 5);
}

int main() {
    std::cout << "Program running " << std::endl;
    return 0;
}
