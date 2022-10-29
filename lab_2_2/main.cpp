#include <iostream>
#include <set>
#include <random>
#include <utility>
#include <fstream>

class Predicate {
public:
    virtual bool operator()(int s) const = 0;
};

class DiscreteState: public Predicate {
private:
    int const state;

public:
    explicit DiscreteState(int state): state(state) { }

    bool operator()(int s) const override {
        return s == state;
    }
};

class SegmentState: public Predicate {
private:
    int const beg, end;

public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }

    bool operator()(int s) const override {
        return s >= beg && s <= end;
    }
};

class SetState: public Predicate {
private:
    std::set<int> const states;

public:
    SetState(): states() { }
    explicit SetState(std::set<int> src): states(std::move(src)) { }

    bool operator()(int s) const override {
        return states.count(s) > 0;
    }
};

class SegmentHoleState: public Predicate {
private:
    int const beg, end;
    std::set<int> const holes;

public:
    explicit SegmentHoleState(int beg, int end, std::set<int> src): beg(beg), end(end), holes(std::move(src)) { }
    explicit SegmentHoleState(): SegmentHoleState(0, -1, {}) {}
    explicit SegmentHoleState(int beg, int end): SegmentHoleState(beg, end, {}) {}

    bool operator()(int s) const override {
        return (s >= beg && s <= end) && (!holes.count(s));
    }
};

class SegmentAddState: public Predicate {
private:
    int const beg, end;
    std::set<int> const add;

public:
    explicit SegmentAddState(int beg, int end, std::set<int> src): beg(beg), end(end), add(std::move(src)) {}
    explicit SegmentAddState(): SegmentAddState(0, -1, {}) {}
    explicit SegmentAddState(int beg, int end): SegmentAddState(beg, end, {}) {}
    explicit SegmentAddState(std::set<int> src): SegmentAddState(0, -1, std::move(src)) {}

    bool operator()(int s) const override {
        return (s >= beg && s <= end) || (add.count(s));
    }
};

class SegmentHoleAddState: public Predicate {
private:
    int const beg, end;
    std::set<int> const holes;
    std::set<int> const add;

public:
    explicit SegmentHoleAddState(int beg, int end, std::set<int> holes, std::set<int> add): beg(beg), end(end), holes(std::move(holes)), add(std::move(add)) { }
    explicit SegmentHoleAddState(): SegmentHoleAddState(0, -1, {}, {}) {}
    explicit SegmentHoleAddState(int beg, int end): SegmentHoleAddState(beg, end, {}, {}) {}
    explicit SegmentHoleAddState(std::set<int> holes, std::set<int> add): SegmentHoleAddState(0, -1, holes, add) { }
    explicit SegmentHoleAddState(std::set<int> add): SegmentHoleAddState(0, -1, {}, add) { }


    bool operator()(int s) const override {
        return (s >= beg && s <= end) && (!holes.count(s)) || add.count(s);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min),
                                                                                      test_max(test_max),
                                                                                      test_count(test_count) {}

    float operator()(Predicate const &predicate) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (predicate(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

void task1(int test_count) {
    std::ofstream task1_s;
    std::ofstream task1_ss;
    task1_s.open("task1_s.txt");
    task1_ss.open("task1_ss.txt");
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    for (int idx = 1; idx < test_count; idx+=200) {
        ProbabilityTest pt(10, 0, 100, idx);
        task1_s << idx << ' ' << pt(s) << std::endl;
        task1_ss << idx << ' ' << pt(ss) << std::endl;
    }
    task1_s.close();
    task1_ss.close();
}

void task2(int number=10000) {
    std::ofstream task2_s;

    task2_s.open("task2_s.txt");

    for (int idx = 1000; idx < number; idx++) {
        SegmentState s(0, idx);
        ProbabilityTest pt(10, 0, 10000, 10000);
        auto p = pt(s);
        task2_s << idx << ' ' << p << ' ' << round(10000 * p) << std::endl;
    }
    task2_s.close();
}

void task3(bool discrete=true, int test_count=100000, bool set=true, int number=10000, int seed=10) {
    if (discrete) {
        std::ofstream task3_d;
        DiscreteState d(1);
        task3_d.open("task3_d.txt");
        for (int idx = 1; idx < test_count; idx += test_count / 1000) {
            ProbabilityTest pt(10, 0, 100, idx);
            task3_d << idx << ' ' << pt(d) << std::endl;
        }
        task3_d.close();
    }
    if (set) {
        std::ofstream task3_ss;
        task3_ss.open("task3_ss.txt");
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(0, 1000);
        for (int idx = 1000; idx < number; idx ++) {
            std::set<int> state;
            for (int i = 0; i < idx; i++)
                state.insert(dstr(rng));

            SetState ss(state);
            ProbabilityTest pt(10, 0, 10000, 10000);
            auto p = pt(ss);
            task3_ss << idx << ' ' << p << ' ' << round(10000 * p) << std::endl;
        }
        task3_ss.close();
    }
}

int main() {
    task3(false);
    return 0;
}
