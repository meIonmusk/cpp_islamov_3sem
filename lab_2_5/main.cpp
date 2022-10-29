#include <iostream>
#include <cassert>

template <typename T> 
class Grid final { 
public :
    using value_type = T;
    using size_type = unsigned; 

private :
    T ** data ;
    size_type const y_size , x_size ;
    
public:
    Grid(size_type y_size , size_type x_size, T const &val): 
        y_size(y_size), x_size(x_size) {
        data = new T*[y_size];
        for (size_type i = 0; i < y_size; i++){
            data[i] = new T[x_size];
            for (size_type j = 0; j < x_size; j++)
                data[i][j] = val;
        }
    }

    Grid(T const &t){
        Grid(1, 1, t);
    }

    Grid(size_type y_size , size_type x_size){
        Grid(y_size, x_size, T());
    }

    Grid(Grid<T> const &) = delete; 
    Grid(Grid<T>&&)= delete;
    Grid<T>& operator=(Grid<T>&) = delete ;
    Grid<T>& operator=(Grid<T>&&) = delete ;

    T operator ()( size_type y_idx , size_type x_idx) const { 
        return data[y_idx][x_idx] ;
    }

    T& operator ()( size_type y_idx , size_type x_idx) { 
        return data[y_idx][x_idx] ;
    }

    Grid<T>& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it)
            *it=t; 
        return *this ;
    }

    Grid<T>& operator=(T &&t) {
        // for (auto it = data, end = data + x_size * y_size; it != end; ++it)
        //     *it=t; 
        // return *this ;
        return Grid(std::move(t));
    }

    T* operator [](size_type idx){
        return data[idx];
    }

    size_type get_y_size () const { return y_size ; }
    size_type get_x_size () const { return x_size ; }

    ~Grid(){
        for (size_type idx = 0; idx < y_size; idx++)
            delete data[idx];
        delete [] data;
    } 
};

int main() {
    Grid<float> g(3, 2, 0.0f); 

    // g[0][1] = 10;

    // std::cout << g[0][1] << std::endl;
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<int>::size_type;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert (1.0f == g(y_idx , x_idx )); 
    return 0;
}