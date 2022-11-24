#include <iostream>
#include <vector>

/*struct DoubleAbstractStack {
            // some code goes here
        public:
            bool push(double next_element) {
                // some code goes here
                before_push();
                // push code
                after_push(next_element);
                return true;
            }

            // other public methods
        protected:
            virtual void before_push() = 0;
            virtual void after_push(double pushed_element) = 0;
            virtual void before_pop() = 0;
            virtual void after_pop(double popped_element) = 0;
};*/

struct DoubleStackWithMin: public DoubleAbstractStack {
    std::vector<double> stack_min;

    DoubleStackWithMin(): DoubleAbstractStack() {}

    void before_pop() override {
        return;
    }

    void before_push() override {
        return;
    }

    void after_push(double pushed) override {
        stack_min.push_back((stack_min.empty() || pushed < stack_min.back())? pushed : stack_min.back());
    }
    
    void after_pop(double popped) override {
        stack_min.pop_back();
    }

    double get_min() const {
        return stack_min.back();
    }
};

// void operate_stack(DoubleAbstractStack &stack);
// void check_min(DoubleStackWithMin const &stack);

// int main() {
//     DoubleStackWithMin min_stack;
//     operate_stack(min_stack);
//     check_min(min_stack);
// }