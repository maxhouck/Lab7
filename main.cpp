/*
 * main.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: Max
 */

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

using std::vector;

void error_message();
void display_menu();

class Signal{
	private:
		std::string filename;
		void read_file();
		void set_maximum();
		void set_average();
	public:
		int length;
		double maximum;
				double average;
		vector<double> signal;
		void operator+(double offset);
		void operator*(double scale);
		Signal(); //take a default input file and reads signal and allocate memory accordingly
		Signal(int filenumber); //takes "Raw_data_nn.txt" read signal and allocate memory
		Signal(char* filename);//take filename and inputs and allocates
		~Signal(); //destructor
		void display();
		void offset(double offset);
		void scale(double scale);
		void center();
		void normalize();
		void statistics();
		void Sig_info();
		void Save_file(char* savefilename);
};

Signal operator+(Signal s1, Signal s2);
Signal operator+(Signal s1, Signal s2) {
	if(s1.length!=s2.length) {
		std::cout << "Wrong length. Returning first argument" << std::endl;
		return s1;
	}
	else {
		Signal s3;
		int i;
		int maximum = s1.signal[1];
		for(i=0;i<s1.length;i++) {
			if(s1.signal[i] > maximum) maximum = s1.signal[i]; //functionality described in lab document for finding maximum
			if(s2.signal[i] > maximum) maximum = s2.signal[i];
			s3.signal.push_back(s1.signal[i]+s2.signal[i]);
		}
		std::cout << "Maximum from addition operation is: " << maximum << std::endl;
		return s3;
	}
}

Signal::Signal() { //empty constructor
	length = 0;
	average = 0;
	maximum = 0;
}

Signal::Signal(char* infilename) { //filename construcxtor
	this->filename = infilename;
	read_file();
}

Signal::Signal(int filenumber) { //filenumber constructor
	//parse file number argument
	filename = "Raw_data_00.txt";
	if(filenumber < 9) { //one digit file number
		filename.at(10)=filenumber;
	}
	else if (filenumber < 100) { //two digit file number
		filename.at(10) = filenumber % 10; //second digit
		filename.at(9) = filenumber / 10;  //first digit
	}
	else if (filenumber >= 100) { //more than two digits = error
		std::cout << "Error. File number too big";
		exit(1);
	}
	read_file();
}

void Signal::operator+(double offset) {
	int i;
	for(i=0;i<length;i++)
		signal[i]+=offset;
}

void Signal::operator*(double scale) {
	int i;
	std::cout<<"scale: "<<scale<<std::endl;
	for(i=0;i<length;i++) {

		std::cout<<signal[i]<<std::endl;
		signal[i]*=scale;
		std::cout<<signal[i]<<std::endl;
	}
}

void Signal::display() {
	std::cout << std::endl; //prints out data in signal array
	int i;
	for(i=0;i<length;i++) {
		std::cout << signal[i] <<std::endl;
	}
}
void Signal::set_maximum() { //searches array for maximum number
	int i;
	maximum = signal[0];
	for(i=0;i<length;i++) {
		if(signal[i] > maximum)
			maximum = signal[i];
	}
}

int main(int argc, char* argv[]) {
	//parse input
	char* filename;
	char* filenumber;
	char* argument;
	Signal signal;
	int i;
	if(argc==1)
		signal = Signal();
	else {
		for(i=1;i<argc;i++){
			argument = argv[i];
			if(*argument != '-') error_message();
			switch(*(argument+1)) {
				case 'n':  //file number
					argument = argv[++i];
					filenumber = argument;
					signal = Signal(atoi(filenumber));
					break;

				case 'f': //file name
					argument = argv[++i];
					filename = argument;
					signal = Signal(filename);
					break;

				default: error_message();
			}
		}
	}
	char choice; //enter menu sequence
	char* savename;
	while(choice != 'e') {
		display_menu();
		std::cin.clear();
		std::cin >> choice;
		switch(choice) {
			case 'd':
				signal.display();
				break;

			case 'o':
				double offset;
				std::cout << "Enter offset: ";
				std::cin >> offset;
				signal.operator+(offset);
				break;

			case 's':
				double scale;
				std::cout << std::endl;
				std::cout << "Enter scale: ";
				std::cin >> scale;
				signal.operator*(scale);
				break;

			case 'c':
				signal.operator+(-signal.average/2);
				break;

			case 'n':
				signal.operator*(1/signal.maximum);
				break;

			case 'i':
				signal.Sig_info();
				break;

			case 'a':
				std::cout << "Enter name of saved file: ";
				savename = (char*) malloc(sizeof(char)*20);
				std::cin >> savename;
				signal.Save_file(savename);
				break;

			case 'e':
				exit(1);
				break;

			default:
				std::cout << "Not a valid option" << std::endl;
		}
	}
//
//	Signal signal1 = "Raw_data_00.txt";
//	Signal signal2 = "Raw_data_00.txt";
//	Signal signal3 = signal1 + signal2;

	return 1;
}

void Signal::read_file() {
	FILE *readfile;
	readfile = fopen(filename.c_str(),"r"); //opening file
	if(readfile == NULL) { //error checking
		std::cout << "Error opening file.";
		exit(1);
	}
	fscanf(readfile,"%d %lf",&length,&maximum);
	double n;
	for(int i = 0; i < length; i++) {//scanning in data
 		fscanf(readfile,"%lf",&n);
 		signal.push_back(n);
	}
	fclose(readfile); //close file
}

void Signal::set_average() { //sets average
	int i;
	double sum=0;
	for(i = 0; i < length; i++) {
		sum += signal[i];
	}
	average = sum / length;
}

void Signal::offset(double offset) { //offset signal by argument
	int i;
	for(i = 0; i < length; i++)
		signal[i] += offset;
}

void Signal::scale(double scale) { //scale signal by argument
	int i;
	for(i = 0; i < length; i++)
		signal[i] *= scale;
}

void Signal::center() { //center signal on 0
	set_maximum();
	this->offset(-maximum/2);
}

void Signal::normalize() { //normalize between 1 and 0
	set_maximum();
	//TODO call operator from here
}

void Signal::Sig_info() {
	set_maximum(); //display stats about signal
	set_average();
	std::cout << "Length: " << length << std::endl;
	std::cout << "Maximum: " << maximum << std::endl;
	std::cout << "Average: " << average << std::endl;
}

void Signal::Save_file(char* savefilename) {
	set_maximum();
	FILE *writefile; //same as rename_data
	writefile = fopen(savefilename,"w"); //open file
	if(writefile == NULL) {
		std::cout << "Error opening write file." << std::endl;
		exit(0);
	}
	fprintf(writefile,"%d %.4f\n", length, maximum); //write max and length
	for(int i = 0; i < length; i++) //write the rest of the data
		fprintf(writefile,"%.4f\n",signal[i]);
	fclose(writefile);
}

Signal::~Signal() { //destructor

}

void error_message() {
	std::cout << "ERROR. Usage: \n \t-n <file number> \n\t-f <file name>" << std::endl;
	exit(0);
}

void display_menu() {
	std::cout << "Menu:" << std::endl;
	std::cout << "\t(d)isplay" << std::endl;
	std::cout << "\t(o)ffset" << std::endl;
	std::cout << "\t(s)cale" << std::endl;
	std::cout << "\t(c)enter" << std::endl;
	std::cout << "\t(n)ormalize" << std::endl;
	std::cout << "\tshow (i)nfo" << std::endl;
	std::cout << "\ts(a)ve" <<std::endl;
	std::cout << "\t(e)xit" << std::endl;
	std::cout << "Make a selection:" << std::endl;
}
