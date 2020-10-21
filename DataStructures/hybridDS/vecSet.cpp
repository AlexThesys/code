#include <unordered_set>
#include "small_vector.h"
#include <iostream>

template <typename T, int N,
         typename VectorT = SmallVector<T, N>,
         typename SetT = std::unordered_set<T>>
class SmallSetVector
{
    SetT S;
    VectorT V;
public:
    bool insert(const T& x)
    {
        bool result = S.insert(x).second;
        if (result) V.push_back(x);
        return result;
    }
    typename VectorT::iterator begin() {return V.begin();}
    typename VectorT::iterator end() {return V.end();}
};

int main()
{
    SmallSetVector<int, 128> ssv;
    ssv.insert(5);
    ssv.insert(10);
    ssv.insert(7);
    ssv.insert(5);

    for (const auto& v : ssv)
        std::cout << v << "\n";
}
