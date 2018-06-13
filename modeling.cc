#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>
#include <cassert>

constexpr int kDimensions = 2;
constexpr int kDataPoints = 20;

constexpr std::array<std::array<float, kDimensions>, kDataPoints> points = {{{3.8686992545234347, 1.3559682753431659},
                                                                                    {0.5180993375153686, 5.117486709676035},
                                                                                    {3.5386436080165806, 1.5271125008781592},
                                                                                    {4.728401339990676, 1.114753166423772},
                                                                                    {9.023721046916886, 4.338066212680383},
                                                                                    {0.01162719298319903, 6.076772652356936},
                                                                                    {5.991635210658819, 1.2966680782036724},
                                                                                    {6.065836085221114, 1.3272013121118933},
                                                                                    {8.116481067645926, 3.0424908489990976},
                                                                                    {3.4596129148699015, 1.5745584744071195},
                                                                                    {3.311476661626893, 1.6702222128461326},
                                                                                    {5.877037623043535, 1.2538389984467715},
                                                                                    {1.292978422219696, 3.8484017956257546},
                                                                                    {4.721539277238149, 1.11550807482421},
                                                                                    {8.506311207767968, 3.5588436571438553},
                                                                                    {0.6057636006285494, 4.961862706712194},
                                                                                    {7.104743838802512, 1.985989325395428},
                                                                                    {4.479413884955205, 1.1542019806354862},
                                                                                    {3.0128605724511037, 1.8897446209038709},
                                                                                    {1.9357873494944466, 2.9778798335036534}}};

class SquaredLoss {
protected:
    unsigned long dim;
public:
    explicit SquaredLoss(const unsigned long dim) : dim(dim) {
    }

    virtual double computeLoss(const std::vector<std::vector<double >> &X, const std::vector<double> &Y,
                               std::vector<double> &W) {
        // compute squared loss
        unsigned long L = X.size();
        if (L == 0) return 0.0;
        double answer = 0.0;
        double y;
        for (int i = 0; i < L; i++) {
            y = 0.0;
            for (int j = 0; j < dim; j++) y += W[j] * X[i][j];
            answer += (Y[i] - y) * (Y[i] - y);
        }

        return answer;
    }
};

class L2SquaredLoss : public SquaredLoss {
    double lambda;
public:
    L2SquaredLoss(const unsigned long dim, const double lambda) : SquaredLoss(dim) {
        this->lambda = lambda;
    };

    double computeLoss(const std::vector<std::vector<double >> &X, const std::vector<double> &Y,
                       std::vector<double> &W) override {
        // compute L2 regularized loss

        double answer = SquaredLoss::computeLoss(X, Y, W);

        for (int j = 0; j < dim; j++) answer += lambda * W[j] * W[j];

        return answer;
    }
};

class L1SquaredLoss : public SquaredLoss {
    double lambda;
public:
    L1SquaredLoss(const unsigned long dim, const double lambda) : SquaredLoss(dim) {
        this->lambda = lambda;
    };

    double computeLoss(const std::vector<std::vector<double >> &X, const std::vector<double> &Y,
                       std::vector<double> &W) override {
        // compute L1 regularized loss

        double answer = SquaredLoss::computeLoss(X, Y, W);

        for (int j = 0; j < dim; j++) answer += lambda * abs(W[j]);

        return answer;
    }
};

void solver(const std::vector<std::vector<double >> &X, const std::vector<double> &Y, const unsigned long dim,
                std::vector<double> &W, const double step, SquaredLoss *lossF) {
    // parameter optimization: coordinate descent

    unsigned long L = X.size();
    if (L == 0) return;
    // ensure every data point to have the same dimensionality
    for (int i = 0; i < L; i++) assert(X[i].size() == dim);
    double prev_loss, curr_loss, l0, li;
    bool to_iterate;


    prev_loss = lossF->computeLoss(X, Y, W);


    for (int epoch = 1; epoch <= 1000; epoch++) {

        for (int d = 0; d < dim; d++) {

            l0 = lossF->computeLoss(X, Y, W);
            to_iterate = true;
            // try to increase coordinate value
            while (to_iterate) {
                W[d] += step;
                li = lossF->computeLoss(X, Y, W);
                if (li < l0) l0 = li;
                else {
                    W[d] -= step;
                    to_iterate = false;
                }
            }
            to_iterate = true;
            // try to decrease coordinate value
            while (to_iterate) {
                W[d] -= step;
                li = lossF->computeLoss(X, Y, W);
                if (li < l0) l0 = li;
                else {
                    W[d] += step;
                    to_iterate = false;
                }
            }
        }
        curr_loss = lossF->computeLoss(X, Y, W);
        // if we do not make any significant progress - lets terminate
        if ((prev_loss - curr_loss) / prev_loss <= 0.001)
            return;
        prev_loss = curr_loss;
    }
}

std::vector<double> getX(const double x, const unsigned long dim) {
    assert(dim >= 1);
    auto X = std::vector<double>(dim, 1.0);
    for (int d = 1; d < dim; d++) X[d] = X[d - 1] * x;
    return X;
}

int main() {
    /*
     *Use a model of your choice to approximate the relationship between the
     independent/dependent variable pairs above. Model choice is up to you - just
     print out coefficients!
     */


    auto X = std::vector<std::vector<double >>();
    auto Y = std::vector<double>();

    unsigned long dim = 4; // feature dimensions
    double lambda = 0.1; // regularization parameter
    double step = 0.0001; // for coordinate descent
    auto W = std::vector<double>(dim, 0.0); // weights
    for (auto p: points) {
        auto x = getX(p[0], dim);
        X.push_back(x);
        Y.push_back(p[1]);
    }

    auto sq_loss = new SquaredLoss(dim); // least squares loss
    auto l2_loss = new L2SquaredLoss(dim, lambda); // L2 regularized least squares loss
    auto l1_loss = new L1SquaredLoss(dim, lambda); // L1 regularized least squares loss

    // compute regression coefficients
    solver(X, Y, dim, W, step, l2_loss);

    // output
    std::cout << "Residual standard error = " << sqrt(sq_loss->computeLoss(X, Y, W)/kDataPoints) << std::endl;
    std::cout << "Fitted f(x) = ";
    for (int d = 0; d < dim; d++) {
        if (d >= 1 && W[d] >= 0) std::cout << " + ";
        if (d >= 1 && W[d] < 0) std::cout << " - ";
        std::cout << abs(W[d]);
        if (d >= 1) std::cout << "*x";
        if (d >= 2) std::cout << "^" << d;
    }
    std::cout << std::endl;

}
