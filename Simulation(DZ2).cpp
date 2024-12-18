#include <bits/stdc++.h>

using namespace std;

constexpr std::array<pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

// General Fixed-point template
template <size_t N, size_t K>
class Fixed {
public:
    using ValueType = typename std::conditional<N <= 16, int16_t,
                      typename std::conditional<N <= 32, int32_t,
                      typename std::conditional<N <= 64, int64_t, void>::type>::type>::type;

    constexpr Fixed() : value(0) {}
    constexpr Fixed(float f) : value(f * (1 << K)) {}
    constexpr Fixed(double f) : value(f * (1 << K)) {}
    constexpr Fixed(ValueType raw) : value(raw) {}

    static constexpr Fixed from_raw(ValueType raw) {
        return Fixed(raw);
    }

    auto operator<=>(const Fixed&) const = default;

    Fixed operator+(Fixed other) const {
        return Fixed::from_raw(value + other.value);
    }

    Fixed operator-(Fixed other) const {
        return Fixed::from_raw(value - other.value);
    }

    Fixed operator*(Fixed other) const {
        return Fixed::from_raw((static_cast<int64_t>(value) * other.value) >> K);
    }

    Fixed operator/(Fixed other) const {
        return Fixed::from_raw((static_cast<int64_t>(value) << K) / other.value);
    }

    Fixed& operator+=(Fixed other) {
        value += other.value;
        return *this;
    }

    Fixed& operator-=(Fixed other) {
        value -= other.value;
        return *this;
    }

    Fixed& operator*=(Fixed other) {
        value = (*this * other).value;
        return *this;
    }

    Fixed& operator/=(Fixed other) {
        value = (*this / other).value;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Fixed& f) {
        os << static_cast<double>(f.value) / (1 << K);
        return os;
    }

private:
    ValueType value;
};

// FastFixed specialization
template <size_t N, size_t K>
using FastFixed = Fixed<N, K>;

// Runtime Type Dispatcher
class TypeDispatcher {
public:
    enum class DataType { FLOAT, DOUBLE, FIXED, FAST_FIXED };

    TypeDispatcher(DataType pType, DataType vType, DataType vFlowType)
        : pType(pType), vType(vType), vFlowType(vFlowType) {}

    template <typename Func>
    void dispatch(Func&& func) {
        if (pType == DataType::FLOAT) {
            func(float{});
        } else if (pType == DataType::DOUBLE) {
            func(double{});
        } else if (pType == DataType::FIXED) {
            func(Fixed<32, 16>{});
        } else if (pType == DataType::FAST_FIXED) {
            func(FastFixed<32, 16>{});
        } else {
            throw runtime_error("Unsupported type for p");
        }
    }

private:
    DataType pType;
    DataType vType;
    DataType vFlowType;
};

// Simulation class
class FluidSimulation {
public:
    FluidSimulation(size_t rows, size_t cols, float gravity, TypeDispatcher::DataType pType)
        : N(rows), M(cols), g(gravity), typeDispatcher(pType, TypeDispatcher::DataType::FLOAT, TypeDispatcher::DataType::FLOAT) {
        field.resize(N, string(M, ' '));
    }

    void loadField(const vector<string>& inputField) {
        if (inputField.size() != N) {
            throw runtime_error("Field size mismatch");
        }
        field = inputField;
    }

    void simulate() {
        typeDispatcher.dispatch([this](auto pTypeInstance) {
            using P = decltype(pTypeInstance);
            cout << "Simulating with type " << typeid(P).name() << " and gravity = " << g << "...\n";
            // Placeholder for type-specific simulation logic
        });
    }

    void displayField() const {
        for (const auto& row : field) {
            cout << row << "\n";
        }
    }

private:
    size_t N, M;
    float g; // Gravity
    vector<string> field;
    TypeDispatcher typeDispatcher;
};

int main(int argc, char* argv[]) {
    // Parse command line arguments for types
    TypeDispatcher::DataType pType = TypeDispatcher::DataType::FLOAT;
    if (argc > 1 && string(argv[1]) == "--p-type=FIXED") {
        pType = TypeDispatcher::DataType::FIXED;
    }

    FluidSimulation simulation(10, 10, 9.8, pType);

    vector<string> initialField = {
        "##########",
        "#        #",
        "#        #",
        "#        #",
        "#   .... #",
        "#   .... #",
        "#        #",
        "#        #",
        "#        #",
        "##########",
    };

    simulation.loadField(initialField);
    simulation.displayField();
    simulation.simulate();

    return 0;
}