#include "egr_dlib.h"

#include <sstream>

classifier dlib_train(std::vector<sample_fingers_type> &samples, std::vector<string> &labels){
	ovo_trainer trainer;
	
	svm_nu_trainer<poly_kernel> svm_trainer; 
	svm_trainer.set_kernel(poly_kernel(0.1, 1, 2));
	trainer.set_trainer(svm_trainer);

	randomize_samples(samples, labels);
	cross_validate_multiclass_trainer(trainer, samples, labels, 3);
	one_vs_one_decision_function<ovo_trainer> df = trainer.train(samples, labels);

	classifier c;
	c = df;
	return c;
}

/*
//cross validation and parameter estimation for one vs one with K kernel and T trainer
template <typename T>
ovo_trainer &dlib_ovo_cv_pe(std::vector<sample_fingers_type> &samples, std::vector<string> &labels, int kernel_type ){
	ovo_trainer ovo;
	switch(kernel_type){
		case DLIB_POLY_KERNEL:
			T<poly_kernel> poly_trainer;
			poly_trainer.set_kernel(poly_kernel(0.1, 1, 2));
			ovo.set_trainer(poly_trainer);
			break;
		case DLIB_RBF_KERNEL:
			T<rbf_kernel> rbf_trainer;
			rbf_trainer.set_kernel(rbf_kernel(0.1));
			break;
		case DLIB_SIG_KERNEL:
			T<sig_kernel> sig_trainer;
			sig_trainer.set_kernel(sig_kernel(0.1, -1));
			break;
	}
	return ovo;

}
std::string dlib_train(std::vector<sample_fingers_type> &samples, std::vector<string> &labels){
	ovo_trainer trainer;
	
	svm_nu_trainer<poly_kernel> svm_trainer; 
	//svm_trainer.set_kernel(poly_kernel(0.1, 1, 2));

	//trainer.set_trainer(svm_trainer);
	
	randomize_samples(samples, labels);
	stringstream ss;

	//ss << "cross validation: " << endl << cross_validate_multiclass_trainer(trainer, samples, labels, 3) << endl;
//const double max_nu = maximum_nu(labels);

for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
    {
        
            // tell the trainer the parameters we want to use
            svm_trainer.set_kernel(poly_kernel(gamma, 1, 2));
            //svm_trainer.set_nu(nu);
			trainer.set_trainer(svm_trainer);
            ss << "gamma: " << gamma << endl;
            // Print out the cross validation accuracy for 3-fold cross validation using the current gamma and nu.  
            // cross_validate_trainer() returns a row vector.  The first element of the vector is the fraction
            // of +1 training examples correctly classified and the second number is the fraction of -1 training 
            // examples correctly classified.
            ss << "     cross validation accuracy: " << cross_validate_multiclass_trainer(trainer, samples, labels, 3) <<endl;
        
    }


	/*one_vs_one_decision_function<ovo_trainer> df = trainer.train(samples, labels);

	

	one_vs_one_decision_function<ovo_trainer, decision_function<poly_kernel>> df2;


	// Put df into df2 and then save df2 to disk.  Note that we could have also said
	// df2 = trainer.train(samples, labels);  But doing it this way avoids retraining.
	df2 = df;
	ofstream fout("c:\\egr\\df.dat", ios::binary);
	serialize(df2, fout);
	fout.close();
	int incorrect = 0;
	ss << "Missclassifications: " <<endl;
	for(int i=0;i<samples.size(); i++){
		if(labels[i].compare(string(df(samples[i])))==0){
			
		}else{
			//ss << "(" << labels[i] << "->" << df(samples[i]) << ")" << endl;
			incorrect++;
		}

	}
	ss << test_multiclass_decision_function(df, samples, labels);
	ss << endl << "Correct: " << 100*(float)(samples.size() - incorrect)/samples.size() << "% (" << (samples.size()-incorrect) << "/" << samples.size() << ")" << endl;
	ss << "Incorrect: " << 100*(float)(incorrect)/samples.size() << "% (" << (incorrect) << "/" << samples.size() << ")" << endl;
	*
	return ss.str();
}*/