
//
// \file    modeling.cc
// \author  Sergio Valero, esevalero@gmail.com
// \date    21.06.2018
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// \brief:  You are given a list of input data points corresponding to        //
//          a independent/depenent variable pair. Estimate the relationship   //
//          between the dependent and independent variable with a polynomial  //
//          model of your choice, and report how well that model fits         //
//          (the residual error).                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <array>
#include <set>
#include <algorithm>
#include "../../Eigen-3.3.4/Eigen/Dense"

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------
using namespace Eigen;


//------------------------------------------------------------------------------
// Dummy data
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
// Helpers Fuctions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Eigen Matrix push_back
template <typename DynamicEigenMatrix>
void push_back(DynamicEigenMatrix& m, Vector3f&& values, std::size_t row)
{
     if(row >= m.rows()) {
         m.conservativeResize(row + 1, Eigen::NoChange);
     }
     m.row(row) = values;
}

//------------------------------------------------------------------------------
// STD::vector Median
float median(std::vector<float> &n) {
    std::sort(n.begin(), n.end());
    double result = 0;
    auto size = n.size();

    if ((size % 2) == 0) {
        result = (n[size/2] + n[(size/2) - 1])/2.0;
    }
    else {
        result = n[size/2];
    }

    return result;
}

//------------------------------------------------------------------------------
// \brief       Insert path of the text file to analize
//
// \return      String container to the file path
//
class PolynomicRANSAC {

    public:

        //----------------------------------------------------------------------
        // \brief   constructor
        //
        PolynomicRANSAC(
            const std::vector<float> x,     //< Values of the dependent variable
            const std::vector<float> y,     //< Values of the independent variable
            const int         order         //< Polynomial order
        );

        //----------------------------------------------------------------------
        // \brief   destructor
        //
        ~PolynomicRANSAC(){};

        //----------------------------------------------------------------------
        // \brief   Fit model
        //
        void fit();

        //----------------------------------------------------------------------
        // \brief   Get model coefficients
        //
        std::vector<float> get_coefficient();

        //----------------------------------------------------------------------
        // \brief   Get model residual Error
        //
        float get_residualError();

    private:
        //----------------------------------------------------------------------
        std::vector<float> m_coefficients;      //< Model coefficients
        //----------------------------------------------------------------------
        MatrixXf m_goodCoef;                      //< Model coefficients
        //----------------------------------------------------------------------
        std::vector<float> m_x;                 //< Model coefficients
        //----------------------------------------------------------------------
        std::vector<float> m_y;                 //< Model coefficients
        //----------------------------------------------------------------------
        int m_order;                            //< Model coefficients
        //----------------------------------------------------------------------
        float m_resError;
        //----------------------------------------------------------------------
        const int MIN_POINTS = 4;               //< Model coefficients
        const int RANSAC_ITER= 70;              //< Model coefficients
};


//------------------------------------------------------------------------------
// \brief Constructor
//
PolynomicRANSAC::PolynomicRANSAC(
    const std::vector<float> x,     //< Values of the dependent variable
    const std::vector<float> y,     //< Values of the independent variable
    int         order         //< Polynomial order
){
    //--------------------------------------------------------------------------
    m_x = x;
    m_y = y;
    m_order = order;
    //--------------------------------------------------------------------------
    m_coefficients.push_back(0.0f);
    m_coefficients.push_back(0.0f);
    m_coefficients.push_back(0.0f);

    m_resError = 9999999999.0f;
}

//------------------------------------------------------------------------------
// \brief Fit the model
//
void PolynomicRANSAC::fit(){

    //--------------------------------------------------------------------------
    /// Populating A and b matrices with all the observations
    int dataLen = m_x.size();

    MatrixXf A(dataLen, (m_order+1));
    VectorXf b(dataLen);
    for ( int i = 0; i < dataLen; i++ ){
        push_back(A, Vector3f( m_x[i]*m_x[i] , m_x[i], 1.0), i);
        b[i] = m_y[i];
    }

    //--------------------------------------------------------------------------
    // RANSAC 4-points
    //--------------------------------------------------------------------------
    for ( int i_ = 0; i_ < RANSAC_ITER; i_++){

        MatrixXf A4P(MIN_POINTS, (m_order+1));
        VectorXf b4P(MIN_POINTS);

        /// Find 4 random points
        std::set<int> idx;
        int lenRand = 0;
        while (lenRand < MIN_POINTS){
            int p1 = rand() % (points.size()-1) + 0 ;
            idx.insert(p1);
            lenRand = idx.size();
        }

        /// Populate 4 points matrices
        int32_t iter = 0;
        for ( std::set<int32_t>::iterator i=idx.begin(); i != idx.end(); ++i ){
            push_back(A4P, Vector3f( m_x[*i]*m_x[*i] , m_x[*i], 1.0), iter);
            b4P[iter] = m_y[*i];
            iter++;
        }

        /// Solve least-squares with SVD
        MatrixXf coeff = A4P.bdcSvd( ComputeThinU | ComputeThinV ).solve(b4P);

        /// Errors: squares error
        MatrixXf errors = b - A * coeff;

        std::vector<float> vecSquareDist;
        for ( int i = 0; i < points.size(); i++ ){
            vecSquareDist.push_back( (errors(i)*errors(i)) );
        }

        if (median(vecSquareDist) < m_resError){
            m_resError = median(vecSquareDist);
            m_goodCoef = coeff;
        }
    }


}


//------------------------------------------------------------------------------
// \brief   Get coeficients
//
std::vector<float> PolynomicRANSAC::get_coefficient()
{
    for ( int i = 0; i < m_goodCoef.size(); i++ ){
        m_coefficients[i] = m_goodCoef(i);
    }
    return m_coefficients;
}

//------------------------------------------------------------------------------
// \brief   Get residual error
//
float PolynomicRANSAC::get_residualError(){
    return m_resError;
}



//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------
int main() {

    // -------------------------------------------------------------------------
    // adapting data
    std::vector<float> x, y;
    for ( int i = 0; i < points.size(); i++   ){
        x.push_back(points[i][0]);
        y.push_back(points[i][1]);
    }

    // -------------------------------------------------------------------------
    // Initialize and train model
    PolynomicRANSAC *model = new PolynomicRANSAC(x,y,2);

    // -------------------------------------------------------------------------
    // Fit data
    model->fit();

    // -------------------------------------------------------------------------
    // Display coefficients
    std::vector<float> coef;
    coef = model->get_coefficient();
    std::cout << " This si the polynomial: Y = Ax*x + Bx + C" << std::endl;
    std::cout << " This are the Coefficients: " << std::endl;
    std::cout << "  Coefficient A: " << coef[0] << std::endl;
    std::cout << "  Coefficient B: " << coef[1] << std::endl;
    std::cout << "  Coefficient C: " << coef[2] << std::endl;

    std::cout << " -----------------------------------------------" << std::endl;
    std::cout << " Residual Error: " << model->get_residualError() << std::endl;

    return 0;
}
