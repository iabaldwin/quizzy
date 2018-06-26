
//
// \file    matching.cc
// \author  Sergio Valero, esevalero@gmail.com
// \date    21.06.2018
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// \brief:  You are given three sets of image patches, corresponding to       //
//          portions of a normalized grayscale image (i.e. all values are     //
//          from 0.0-1.0, as opposed to 0-255). For the reference patch,      //
//          which is the closest candidate patch? (For your definition of     //
//          closeness).                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>

constexpr int kPatchSize = 5;

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> referencePatch = {{{0.13562204,0.27552594,0.35980544,0.94924713,0.34222606},
                                                                                   {0.94931411,0.27460949,0.75677189,0.24251008,0.50926451},
                                                                                   {0.97589259,0.46464161,0.52612093,0.95127515,0.37320958},
                                                                                   {0.10335494,0.22910981,0.44470971,0.28351396,0.65846204},
                                                                                   {0.94594718,0.52795235,0.98536625,0.09142491,0.99676918}}};

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> patchCandidate1 = {{{0.65051287,0.0858077 ,0.62365821,0.93836648,0.50158388},
                                                                                    {0.82328337,0.82281573,0.05730619,0.44645271,0.96427283},
                                                                                    {0.82971349,0.80185606,0.96409221,0.98732598,0.28638456},
                                                                                    {0.07682913,0.26305113,0.75847402,0.80500383,0.28722148},
                                                                                    {0.20879353,0.24539516,0.71978387,0.66240491,0.08076461}}};

constexpr std::array<std::array<float, kPatchSize>, kPatchSize> patchCandidate2 = {{{0.13290402,0.96803355,0.8971992 ,0.76052644,0.30922267},
                                                                                    {0.30115749,0.78518142,0.30251533,0.69725331,0.58646293},
                                                                                    {0.03114041,0.15132543,0.60837695,0.18235618,0.74499181},
                                                                                    {0.19450344,0.93216069,0.5751807 ,0.38489764,0.5703268},
                                                                                    {0.92990664,0.22307124,0.63934838,0.38695049,0.21440734}}};


//------------------------------------------------------------------------------
// \brief       This fucntion analize the similarity of two patches if an
//              gray scale image. It computes the Normalized Cross-Correlation
//              between the two patches as a indicator of the similarity.
//
// \return      Ratio-percentage of the normalized cross-correlation
//
float similarity(
    std::array<std::array<float, kPatchSize>, kPatchSize> reference,    //< Referemnce patch
    std::array<std::array<float, kPatchSize>, kPatchSize> candidate     //< Candidate patch
){
    float NCC; //< Normalized cross correlation

    float sumRefxCan = 0.000000000f;
    float sumRefxRef = 0.000000000f;
    float sumCanxCan = 0.000000000f;

    for (int i = 0; i < kPatchSize; i++){
        for (int j = 0; j < kPatchSize; j++){
            sumRefxCan += reference[i][j]*candidate[i][j];
            sumRefxRef += reference[i][j]*reference[i][j];
            sumCanxCan += candidate[i][j]*candidate[i][j];
        }
    }

    NCC = sumRefxCan / ( sqrt(sumRefxRef) * sqrt(sumCanxCan) );

    return NCC*100.0f;
}


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
int main() {

    std::cout << "The candidate 1 is a " << similarity(referencePatch, patchCandidate1) << "% similar to the Reference" << std::endl;
    std::cout << "The candidate 2 is a " << similarity(referencePatch, patchCandidate2) << "% similar to the Reference" << std::endl;

}
