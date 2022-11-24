#include <iostream>

class SubstepStarter final {
    int n = 0;
    int k = 0;
public:
    bool start() {
        return n == k;
    }

    void end() { k++;}

    bool next_run() {
        n++;
        if (k > n) {
            
            k = 0;
            return true;
        }
        return false;
    }
};


void run_substep(SubstepStarter &s) {
    if (s.start()) {
    std::cout << '1';
  }
  s.end();

  if (s.start()) {
    std::cout << '2';
  }
  s.end();
  std::cout << '#';
}

void run_step(SubstepStarter &s) {
    do {
    run_substep(s);
    } while(s.next_run());
}

int main() {
    SubstepStarter s;
    run_step(s);
    return 0;
}