#include <iostream>

class ComplexInt {
private:
    int const real;
    int const image;
public:
    ComplexInt(int real, int image): real(real), image(image) {}
    explicit ComplexInt(int image): ComplexInt(0, image) {}
    ComplexInt(): ComplexInt(0, 0) {}

    int get_real() const{
        return real;
    }

    int get_image() const{
        return image;
    }

    void print() const {
        std::cout << real << " + " << image << 'i';
    }
};

template <typename T> struct Comparator {
    virtual bool operator ()(T const &, T const &) const = 0;
};

struct IntComparator final : Comparator<int> {
    bool operator ()( int const &lha , int const &rha) const override {
        return lha < rha ;
    }
};

struct FloatComparator final : Comparator<float> {
    bool operator ()( float const &lha , float const &rha) const override {
        return lha < rha ;
    }
};

struct DoubleComparator final : Comparator<double> {
    bool operator ()( double const &lha , double const &rha) const override {
        return lha < rha ;
    }
};

struct ComplexComparator final :Comparator<ComplexInt> {
    bool operator ()( ComplexInt const &lha , ComplexInt const &rha) const override {
        const int r1 = lha.get_real();
        const int i1 = lha.get_image();
        const int r2 = rha.get_real();
        const int i2 = rha.get_image();

        return r1*r1 + i1*i1 < r2*r2 + i2*i2 ;
    }
};

template <typename T>
void swap(T* first, T* second) {
    size_t size = sizeof(*first);
    char buffer[size];
    memcpy(buffer, first, size);
    memcpy(first, second, size);
    memcpy(second, buffer, size);
}

template <typename T>
void qsort(T* first, size_t number, Comparator<T> const &comparator) {
    if (number <= 1)
        return;
    size_t num_left = 0;
    T* ptr = first;

    for (size_t i = number-1; i > 0; i--){
        T* tmp = first + i;
        if (comparator(*ptr, *tmp) && i < num_left || !(comparator(*ptr, *tmp)) && i > num_left) {
            swap(ptr, tmp);
            ptr = tmp;
            num_left = i;
        }
    }

    qsort(first, num_left, comparator);
    if (num_left + 1 < number)
        qsort(ptr + 1, number - num_left - 1, comparator);
}

template <typename T>
struct BinaryTree{
    T data = NULL;
    size_t height = 1;
    BinaryTree *left_branch = nullptr;
    BinaryTree *right_branch = nullptr;
};

template <typename T>
class PriorityQueue {
private:
    BinaryTree<T>* tree = nullptr;

    void update_height(BinaryTree<T>* root){
        if (!root)
            return;

        int left_height = root->left_branch ? root->left_branch->height : 0;
        int right_height = root->right_branch ? root->right_branch->height : 0;
        root->height = std::max(left_height, right_height) + 1;
    }

    int diff(BinaryTree<T>* root){
        int left_height = root->left_branch ? root->left_branch->height : 0;
        int right_height = root->right_branch ? root->right_branch->height : 0;
        return left_height - right_height;
    }

    BinaryTree<T>* rotate_left(BinaryTree<T>* root){
        auto root_right = root->right_branch;
        root->right_branch = root_right->left_branch;
        root_right->left_branch = root;
        update_height(root);
        update_height(root_right);
        return root_right;
    }

    BinaryTree<T>* rotate_right(BinaryTree<T>* root){
        auto root_left = root->left_branch;
        root->left_branch = root_left->right_branch;
        root_left->right_branch = root;
        update_height(root);
        update_height(root_left);
        return root_left;
    }

    BinaryTree<T>* balance_tree(BinaryTree<T>* root){
        update_height(root);
        if (diff(root) == 2){
            if (diff(root->left_branch) < 0)
                root->left_branch = rotate_left(root->left_branch);
            return rotate_right(root);
        } else if (diff(root) == -2){
            if (diff(root->right_branch) > 0)
                root->right_branch = rotate_right(root->right_branch);
            return rotate_left(root);
        }
        return root;
    }

    BinaryTree<T>* insert(BinaryTree<T>* root, BinaryTree<T>* branch){
        if (!root)
            return branch;

        if (branch->data > root->data)
            root->left_branch = insert(root->left_branch, branch);
        else
            root->right_branch = insert(root->right_branch, branch);

        return balance_tree(root);
    }

    BinaryTree<T>* search_max(BinaryTree<T>* root){
        return (root->left_branch) ? search_max(root->left_branch) : root;
    }

    BinaryTree<T>* search_prev_max(BinaryTree<T>* root){
        return (root->left_branch) ? (root->left_branch->left_branch) ? search_prev_max(root->left_branch) : root : root;
    }

    void delete_tree(BinaryTree<T>* root){
        if (root){
            delete_tree(root->left_branch);
            delete_tree(root->right_branch);
            delete root;
        }
    }

public:
    PriorityQueue(): tree() {}

    void push(T data) {
        auto branch = new BinaryTree<T>;
        branch->data = data;
        tree = insert(tree, branch);
    }

    T peek() {
        if (tree)
            return search_max(tree)->data;
        return 0;
    }

    void poll(){
        auto max = search_max(tree);
        auto prev_max = search_prev_max(tree);
        auto right = max->right_branch;
        delete max;
        prev_max->left_branch = right ? right : nullptr;

        tree = balance_tree(tree);
    }

    ~PriorityQueue(){
        delete_tree(tree);
    }
};

int main() {
    int vector[] = {11, 13, 21, 11, 20, 221, 89, -13};

    IntComparator int_comp{};
    qsort(vector, 8, int_comp);
    for(double i : vector)
        std::cout << i << ' ';
    PriorityQueue<double> tree{};
    for (double i : vector)
        tree.push(i);
    std::cout << '\n' << tree.peek() << '\n';
    tree.poll();
    std::cout << tree.peek();

    return 0;
}
