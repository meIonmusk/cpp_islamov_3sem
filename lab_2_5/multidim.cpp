#include <iostream>
#include <cassert>
#include <stdarg.h>

template <typename T, unsigned dim> 
class Grid final { 
public :
    using value_type = T;
    using size_type = unsigned; 

private :
    value_type* data;
    size_type* size;

    size_type get_vol(size_type n) const{
        size_type vol = 1;
        for (size_type i = 0; i < n; i++)
            vol *= size[i];
        return vol;
    }

    void init(value_type val){
        for (size_type i = 0; i < get_vol(dim); i++)
            data[i] = val;
    }

    void construct(size_type i, value_type val){
        init(val);
    }

    template <typename First, typename ... Args>
    void construct(size_type i, First sizex, Args ... args){
        size[i] = sizex;
        construct(++i, args...);
    }

public:
    template <typename ... Args>
    Grid(Args ... args) {
        size = new size_type[dim];
        data = new value_type[get_vol(dim)];
        construct(0, args...);
    }

    Grid(size_type* sizes, value_type* vals) {
        size = sizes;
        data = vals;
    }

    Grid(Grid<value_type, dim> const &) = delete; 
    Grid(Grid<value_type, dim>&&)= delete;
    // Grid<value_type, dim>& operator=(Grid<value_type, dim>&) = delete ;
    // Grid<value_type, dim>& operator=(Grid<value_type, dim>&&) = delete ;
    
    Grid<value_type, dim> operator=(Grid<value_type, dim>&& grid) {
        delete [] data;
        for (size_type idx = 0; idx < dim; idx++)
            size[idx] = grid.size[idx];
        
        size_type vol = get_vol(dim);
        data = new value_type [vol];
        for (size_type idx = 0; idx < vol; idx++)
            data[idx] = grid.data[idx];
    }

    Grid<value_type, dim> operator=(Grid<value_type, dim>& grid){
        delete [] data;
        for (size_type idx = 0; idx < dim; idx++)
            size[idx] = grid.size[idx];
        
        size_type vol = get_vol(dim);
        data = new value_type [vol];
        for (size_type idx = 0; idx < vol; idx++)
            data[idx] = grid.data[idx];
    }

    size_type get_idx(size_type i, size_type sizex) const{
        return sizex;
    }

    template <typename ... Args>
    size_type get_idx(size_type i, size_type sizex, Args ... args) const{
        return sizex * get_vol(dim - i) + get_idx(++i, args...);
    }

    template <typename ... Args>
    value_type operator ()(Args ... args) const{
        return data[get_idx(1, args...)] ;
    }

    // Grid<value_type, dim - 1> operator [](size_type idx) const{
    auto operator [](size_type idx) const {
        auto sizes = new size_type [dim - 1];
        for (size_type i = 0; i < dim - 1; i++)
            sizes[i] = size[i+1];
        size_type vol = get_vol(dim - 1);

        auto vals = new value_type [vol];
        for (size_type i = 0; i < vol; i++)
            vals[i] = data[idx * vol + i];

        return Grid<value_type, dim - 1>(sizes, vals);
    }

    ~Grid(){
        delete [] data;
        delete [] size;
    } 
};


int main() {
    Grid<float, 3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));
    Grid<float, 2> g2(2, 5, 2.0f); 
    assert(2.0f == g2(1, 1));
    Grid<float, 2> g21(2, 5, 3.0f);
    g2 = g3[1];
    assert(1.0f == g2(1, 1));
    return 0;
}