#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

class Creature {
protected:
    int hp;
    double vel[2];
    double pos[2];
    bool alive;
public:
    Creature(double x, double y, double vx, double vy) : hp(100), vel({vx, vy}), pos({x, y}), alive(true) {};
    void move() { pos[0] += vel[0]; pos[1] += vel[1]; }
    bool isAlive() { return alive; }
};

class Kwama: public Creature {
    int age;
public:
    Kwama(double x, double y, double vx, double vy, int age): Creature(x, y, vx, vy), age(age) {}
    virtual void act_like_a_kwama() = 0;
    virtual void pouch() = 0;
    virtual unsigned int slaughter() = 0;

};
//// Yout code goes here

class Worker : public Kwama {
    unsigned int cuttle;
    unsigned int eggs;
public:
    Worker(double x, double y, double vx, double vy, int age) : Kwama(x, y, vx, vy, age), cuttle(2), eggs(1) {}
    virtual void act_like_a_kwama() override { cout << "*busy silence*" << endl; }
    virtual void pouch() override
    {
        hp -= 10;
        if (hp <= 0)
        {
            alive = false;
            vel[0] = vel[1] = 0;
        }
    }
    virtual unsigned int slaughter() override{ alive = false; return cuttle * 109 + eggs * 156; }
};

class Warrior : public Kwama {
    unsigned int cuttle;
public:
    Warrior(double vx, double vy, int age) : Kwama(0, 0, vx, vy, age), cuttle(3) {}
    virtual void act_like_a_kwama() override { cout << "RRRRAAARRRR" << endl; };
    virtual void pouch()override
    {
        hp -= 1;
        if (hp <= 0)
        {
            alive = false;
            vel[0] = vel[1] = 0;
        }
    }
    virtual unsigned int slaughter() override{ alive = false; return cuttle * 109; }
};

class Queen : public Kwama {
    unsigned int cuttle;
    unsigned int eggs;
public:
    Queen(int age) : Kwama(0, 0, 0, 0, age), cuttle(1), eggs(10) {}
    virtual void act_like_a_kwama() override { cout << "HHHHFFFFHHH" << endl; };
    virtual void pouch() override{ hp = 0; alive = false; }
    virtual unsigned int slaughter() override{ alive = false; cout << "No more eggs from this mine..." << endl; return cuttle * 109 + eggs * 156;}
};

int main() {
    vector<Kwama*> mine;
    mine.push_back(new Queen(500));
    mine.push_back(new Warrior(10, 10, 100));
    mine.push_back(new Warrior(-10, -10, 100));
    mine.push_back(new Warrior(5, 10, 100));
    mine.push_back(new Warrior(-10, -5, 100));
    mine.push_back(new Worker(10, 1, 1, 1, 20));
    mine.push_back(new Worker(1, 10, -1, 1, 76));
    mine.push_back(new Worker(7, 10, 1, -1, 375));
    mine.push_back(new Worker(4, 1, 1, 1, 20));
    mine.push_back(new Worker(1, 10, -1, 1, 76));
    mine.push_back(new Worker(8, 10, 1, -1, 375));
    mine.push_back(new Worker(1, 4, -1, 1, 76));
    mine.push_back(new Worker(10, 8, 1, -1, 375));
    mine.push_back(new Worker(10, 1, 1, 1, 20));

    unsigned int profit = 0;

    cout << "Month Morning Star" << endl;
    for (auto k : mine)
        k->act_like_a_kwama();

    cout << endl << "Month Sun's Dawn" << endl;
    mine[3]->pouch();
    profit += mine[7]->slaughter();
    for (auto k : mine)
        if (k->isAlive())
            k->act_like_a_kwama();

    cout << endl << "Month First Seed" << endl;
    mine[12]->pouch();
    mine[1]->pouch();
    profit += mine[0]->slaughter();

    for (auto k : mine)
        if (k->isAlive())
            k->act_like_a_kwama();

    cout << endl << "Profit: " << profit << endl;

    for (auto k : mine)
        delete k;
    return 0;
}