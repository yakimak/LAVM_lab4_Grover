#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <string>

using Complex = std::complex<double>;
using State = std::vector<Complex>;

class Qubit {
private:
    State state;  // [амплитуда |0⟩, амплитуда |1⟩]

public:

    Qubit() : state{1.0, 0.0} {}

    void setState(Complex a, Complex b) {
        double norm = sqrt(std::norm(a) + std::norm(b));
        state = {a / norm, b / norm};
    }


    const State& getState() const { return state; }

    std::string toString() const {
    auto formatNumber = [](double num) -> std::string {
        if (std::abs(std::round(num) - num) < 1e-9) {
            return std::to_string(static_cast<int>(std::round(num)));
        } else {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.4f", num);

            // Удаляем лишние нули после запятой
            std::string s(buffer);
            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
            if (s.back() == '.') s.pop_back();
            return s;
        }
    };



    auto formatComplex = [&formatNumber](const Complex& c) -> std::string {
        double real = c.real();
        double imag = c.imag();
        bool has_real = std::abs(real) > 1e-9;
        bool has_imag = std::abs(imag) > 1e-9;

        if (!has_real && !has_imag) return "0";

        std::string result;
        if (has_real) {
            result += formatNumber(real);
        }

        if (has_imag) {
            if (has_real && imag > 0) result += "+";
            if (imag == -1.0) result += "-";
            else if (imag != 1.0) result += formatNumber(imag);
            result += "i";
        }

        return result;
    };

    std::string part0 = formatComplex(state[0]);
    std::string part1 = formatComplex(state[1]);

    return part0 + "|0⟩ + " + part1 + "|1⟩";
}
};

class QuantumGate {
private:
    std::vector<std::vector<Complex>> matrix;

public:

    QuantumGate(int size) : matrix(size, std::vector<Complex>(size, 0.0)) {}

    void set(int i, int j, Complex value) {
        matrix[i][j] = value;
    }

    State apply(const State& state) const {
        State result(state.size(), 0.0);
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                result[i] += matrix[i][j] * state[j];
            }
        }
        return result;
    }

    void apply(Qubit& control, Qubit& target) const {
    
        State composite(4);
        const State& c_state = control.getState();
        const State& t_state = target.getState();
        
        composite[0] = c_state[0] * t_state[0]; // |00⟩
        composite[1] = c_state[0] * t_state[1]; // |01⟩
        composite[2] = c_state[1] * t_state[0]; // |10⟩
        composite[3] = c_state[1] * t_state[1]; // |11⟩

        State result = apply(composite);

        control.setState(result[0] + result[1], result[2] + result[3]);
        target.setState(result[0] + result[2], result[1] + result[3]);
    }


};

QuantumGate createXGate() {
    QuantumGate gate(2);
    gate.set(0, 1, 1.0);
    gate.set(1, 0, 1.0);
    return gate;
}

QuantumGate createYGate() {
    QuantumGate gate(2);
    gate.set(0, 1, Complex(0, -1));
    gate.set(1, 0, Complex(0, 1));
    return gate;
}

QuantumGate createZGate() {
    QuantumGate gate(2);
    gate.set(0, 0, 1.0);
    gate.set(1, 1, -1.0);
    return gate;
}

QuantumGate createHGate() {
    const double invsqrt2 = sqrt(2)/2.0;
    QuantumGate gate(2);
    gate.set(0, 0, invsqrt2);
    gate.set(0, 1, invsqrt2);
    gate.set(1, 0, invsqrt2);
    gate.set(1, 1, -invsqrt2);
    return gate;
}

QuantumGate createCNOT() {
    QuantumGate gate(4);
    gate.set(0, 0, 1.0);
    gate.set(1, 1, 1.0);
    gate.set(2, 3, 1.0);
    gate.set(3, 2, 1.0);
    return gate;
}

///////////////////////////////////////////////////////////////////////////////////////////////

QuantumGate createGroverOracle() {
    QuantumGate oracle(4);
    oracle.set(0, 0, 1.0);
    oracle.set(1, 1, 1.0);
    oracle.set(2, 2, 1.0);
    oracle.set(3, 3, -1.0);
    return oracle;
}

QuantumGate createDiffusionOperator() {
    QuantumGate diffusion(4);
    const double inv2 = 1.0/2.0;
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i == j) {
                diffusion.set(i, j, -inv2);
            } else {
                diffusion.set(i, j, inv2);
            }
        }
    }
    return diffusion;
}

void groverSearch(Qubit& q0, Qubit& q1) {

    QuantumGate H = createHGate();
    QuantumGate oracle = createGroverOracle();
    QuantumGate diffusion = createDiffusionOperator();
    
    State q0_state = H.apply(q0.getState());
    State q1_state = H.apply(q1.getState());
    
    // Оптимальное количество итераций для 2 кубитов (N=4, π/4√N ≈ 1)
    const int iterations = 1;
    
    for (int i = 0; i < iterations; ++i) {
        oracle.apply(q0, q1);   
        diffusion.apply(q0, q1);
    }
}




