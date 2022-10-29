#include <iostream>

struct Point {
    unsigned long long const x, y;

    Point(unsigned long long x, unsigned long long y): x(x), y(y) { }

    Point minx(Point const &rha) const {
        return {rha.x < x ? rha.x : x, y};
    }

    Point miny(Point const &rha) const {
        return {x, rha.y < y ? rha.y : y};
    }

    Point maxx( Point const &rha ) const {
        return {rha.x > x ? rha.x : x, y};
    }

    Point maxy( Point const &rha ) const {
        return {x, rha.y > y ? rha.y : y};
    }

    virtual void print () const {
        std::cout << '(' << x << ',' << y << ')';
    }
};

struct Rectangle: Point {
    Point const right_top;

    explicit Rectangle(Point const &right_top): Point(right_top.x, right_top.y), right_top(right_top) {}
    Rectangle(): Rectangle(Point(0, 0)) {}

    Rectangle operator+(Point const &rha){
        return Rectangle(right_top.maxx(rha).maxy(rha));
    }

    Rectangle operator*(Point const &rha){
        return Rectangle(right_top.minx(rha).miny(rha));
    }

    void print() const override{
        right_top.print();
    }
};

Rectangle processed_string(std::string expression){
    std::string coordinates[6];
    bool bracket = false;
    bool plus = false;
    bool multi = false;
    int shift = 0;
    expression += '+';

    for (char c:expression){
        if (c == ' ')
            continue;

        if (c == '(')
            bracket = true;
        else if (c == ',')
            shift += 1;
        else if (c == ')'){
            bracket = false;
            shift += 1;
        }
        else if (c == '*' || c == '+'){
            if (multi){
                Rectangle r1(Point(std::stoull(coordinates[shift-4]), std::stoull(coordinates[shift-3])));
                Rectangle r2(Point(std::stoull(coordinates[shift-2]), std::stoull(coordinates[shift-1])));
                Rectangle r3 = r1 * r2;
                coordinates[shift-4] = std::to_string(r3.x);
                coordinates[shift-3] = std::to_string(r3.y);
                coordinates[shift-2] = "";
                coordinates[shift-1] = "";
                shift -= 2;
                multi = false;
            }
            if (c == '+' && plus) {
                Rectangle r1(Point(std::stoull(coordinates[shift-4]), std::stoull(coordinates[shift-3])));
                Rectangle r2(Point(std::stoull(coordinates[shift-2]), std::stoull(coordinates[shift-1])));
                Rectangle r3 = r1 + r2;
                coordinates[shift-4] = std::to_string(r3.x);
                coordinates[shift-3] = std::to_string(r3.y);
                coordinates[shift-2] = "";
                coordinates[shift-1] = "";
                shift -= 2;
                plus = false;
            }

            if (c == '*')
                multi = true;
            else
                plus = true;

        }
        else if (bracket){
            coordinates[shift] += c;
        }
    }
    return Rectangle(Point(std::stoull(coordinates[0]), std::stoull(coordinates[1])));
}

int main() {
    std::string expression;
    std::getline(std::cin, expression);

    processed_string(expression).print();
//    Point p1(2, 3);
//    Point p2(3, 1);
//    Rectangle r1(p1);
//    Rectangle r2(p2);
//    (r1 + r2).print();
//    (r1 * r2).print();

    return 0;
}
