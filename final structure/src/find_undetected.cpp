/* Finds the frame in which ball is undetected 
	Takes the output file from detection program as input*/


#include <detection.h>
#include <point_details.h>

using namespace cv;
using namespace std;

void correctFormat();

int main(int argc, char** argv){
	correctFormat();
	cout << "Check whether input is in above format.\nContinue(y/n)?";
	char correct = 'n';
	cin >> correct;
	if (correct != 'y')
		return 1;
	fstream input(argv[1], ios::in);
	float start, end;
	cout << "Start position?";			//start and end inclusive
	cin >> start;
	cout << "End position?";
	cin >> end;
	point_details temp;
	input.seekg(start * sizeof(temp));
	for (int i = 0; i < end - start + 1; i++){
		input.read((char*)&temp, sizeof(temp));
		if (temp.pt3d == Point3d(0,0,0)){
			cout << temp.frame_no[0] <<' ' << temp.frame_no[1] << endl;
		}
	}

}

void correctFormat(){
	cout << "First argument: " << "file containing detected 3d coordinates" << endl;
}