#include "small_vector.h"
#include <unordered_map>

template <typename KeyT, typename ValueT, int N,
         typename VectorT = SmallVector<ValueT, N>,
         typename MapT = std::unordered_map<KeyT, int>>
class SmallMapVector
{
    MapT M; // map with a key and a value = an index into the vector
    VectorT V;
public:
    //add value
    ValueT& operator[](const KeyT K)
    {
        /*If there is already such a key, then get a ref to a vector cell with this keys value (index),
         * else create a new one with the index = the end of the vector and get back the ref to it.*/
        auto insertResult = M.insert({K, 0}); 
        int& index = insertResult.first->second;
        if (insertResult.second) {
            V.push_back(ValueT());
            index = V.size() - 1;
        }
        return V[index];
    }
    typename VectorT::iterator begin() {return V.begin();}
    typename VectorT::iterator end() {return V.end();}
    //...
};

int main()
{
    SmallMapVector<const char*, float, 128> smv;
    smv["one"] = 1.0f;
    smv["two"] = 2.0f;
    smv["three"] = 3.0f;
    smv["one"] = 5.0f;

    for (const auto& v : smv)
        std::cout << v << "\n";
}
