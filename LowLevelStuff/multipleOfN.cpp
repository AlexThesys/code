inline size_t multipleOfN(const double sr, size_t N)
{
    size_t sRate = static_cast<size_t>(sr);
    size_t adjust = N - (sRate & (N - 1));
       return (sRate + adjust);
}

