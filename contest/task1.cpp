#include <iostream>

class Cell final {
    public:
        char v; 					                         // Вертикаль, от 'A' до 'H'
        unsigned short int h;		                         // Горизонталь, от '1' до '8'

        Cell(): Cell('A', 1) {}			                     // Конструктор клетки по умолчанию
        Cell(char v, unsigned short int h): v(v), h(h) {}	 // Конструктор заданной клетки поля с параметрами
};


class Piece {
    public:
        Cell cell;

        Piece(Cell c): cell(c) {};
        Piece(): Piece(Cell('A', 1)) {};

        virtual bool available(Cell c) const = 0;
};

class King: public Piece {
    public:
        King(Cell c): Piece(c) {};
        King(): King(Cell('A', 1)) {};

        bool available(Cell c) const override {
            auto dh = abs((short int)c.h - (short int)cell.h);
            auto dv = abs(c.v - cell.v);
            return ((dh < 2) && (dv < 2) && (dv + dh));
        }
};

class Bishop: public Piece {
    public:
        Bishop(Cell c): Piece(c) {};
        Bishop(): Bishop(Cell('A', 1)) {};

        bool available(Cell c) const override {
            auto dh = abs((short int)c.h - (short int)cell.h);
            auto dv = abs(c.v - cell.v);
            return ((dh == dv) && (dv + dh));
        }
};

class Rook: public Piece {
    public:
        Rook(Cell c): Piece(c) {};
        Rook(): Rook(Cell('A', 1)) {};

        bool available(Cell c) const override {
            auto dh = abs((short int)c.h - (short int)cell.h);
            auto dv = abs(c.v - cell.v);
            return ((dh * dv == 0) && (dv + dh));
        }
};

class Queen: public Piece {
    public:
        Queen(Cell c): Piece(c) {};
        Queen(): Queen(Cell('A', 1)) {};

        bool available(Cell c) const override {
            auto dh = abs((short int)c.h - (short int)cell.h);
            auto dv = abs(c.v - cell.v);
            return (((dh * dv == 0) || (dh == dv)) && (dv + dh));
        }
};

void are_available(Cell c, const Piece **pieces, bool *availability, std::size_t pieces_count) {
    for (std::size_t piece_idx = 0; piece_idx != pieces_count; ++piece_idx)
        availability[piece_idx] = pieces[piece_idx]->available(c);
}

int main() {
        const std::size_t size = 4;
        const Piece * pieces[size];
        bool availability[size];
    
        pieces[0] = new King(Cell('A',1));
        pieces[1] = new Queen(Cell('B',2));
        pieces[2] = new Rook(Cell('C',3));
        pieces[3] = new Bishop(Cell('D',4));
    
        are_available(Cell('A', 1), pieces, availability, size);
        for(auto ans : availability)
            std::cout << ans << ' ';
        std::cout << std::endl;
    
        for (auto p : pieces)
            delete p;
    
        return 0;
}