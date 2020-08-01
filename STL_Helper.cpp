#define applyUnary(F, Range) F(Range.begin(), Range.end())
#define applyUnaryOp(F, Range, Op) F(Range.begin(), Range.end(), Op)
#define applyBinary(F, Range1, Range2) F(Range1.begin(), Range1.end(), Range2.begin())
#define applyBinaryOp(F, Range1, Range2, Op) F(Range1.begin(), Range1.end(), Range2.begin(), Op)

template <typename... Args>
auto make_lambda_array(Args &&... l1)
{
    using Type = decltype(std::function{l1});
    return std::array<Type, sizeof...(l1)>{l1...};
}