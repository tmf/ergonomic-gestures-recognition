#pragma once

#include "dlib/svm.h"
#include <vector>

using namespace std;
using namespace dlib;

typedef matrix<double, 10, 1> sample_fingers_type;
typedef one_vs_one_trainer<any_trainer<sample_fingers_type>, string> ovo_trainer;
typedef polynomial_kernel<sample_fingers_type> poly_kernel; 
typedef radial_basis_kernel<sample_fingers_type> rbf_kernel;
typedef sigmoid_kernel<sample_fingers_type> sig_kernel;
typedef one_vs_one_decision_function<ovo_trainer, decision_function<poly_kernel>> classifier;

classifier dlib_train(std::vector<sample_fingers_type> &samples, std::vector<string> &labels);