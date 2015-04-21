// PrepareImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define ATTRIBUTES_PER_SAMPLE 576

using namespace std;
cv::Size goalSize;
bool keepAspectRatio = true;
string directoryPath = "";


vector <cv::Mat> imagesOrigin;
#define TRAIN_FILE_NAME "train.txt"
#define TEST_FILE_NAME "test.txt"
std::string trainFileName, testFileName;
void readImages(string filename, bool isTest) 
{
	ofstream myfile;
	myfile.open((isTest) ? trainFileName : testFileName);
	myfile.clear();
	cout << "Reading Images" << endl;
	using namespace boost::filesystem;
	assert(exists(filename));
	//cout << ch << endl;
	path targetPath(filename);
	
	directory_iterator end;

	
	for (directory_iterator file(targetPath); file != end; file++)
	{
		if (is_regular_file(*file))
		{
			cv::Mat curImg = cv::imread(file->path().string(), CV_LOAD_IMAGE_GRAYSCALE);
			cv::Mat processed;
			cv::resize(curImg, processed, goalSize);
			//processed.convertTo(processed, CV_8UC1, 1.0 / 255.5);
			//processed.convertTo(processed, CV_32FC1);
			cv::Mat pointMat = cv::Mat(processed).reshape(1, 1);
			string filestring = file->path().filename().string();
			int classlabel = stoi(filestring.substr(filestring.length() - 6, 1));
			classlabel--;
			//cout << classlabel << endl;
			//todo realize another classes
			if (classlabel <= 4)
			{
				for (int i = 0; i < pointMat.rows; i++)
				{
					for (int j = 0; j < pointMat.cols; j++)
					{
						//std::cout << pointMat.at<uchar>(i, j) << " " << pointMat.at<uchar>(i, j) << " " << pointMat.at<uchar>(i, j) << std::endl;
						try
						{
							//cout << (float)pointMat.at<uchar>(i, j) << " ";
							myfile << (float)pointMat.at<uchar>(i, j) << " ";
						}
						catch (cv::Exception& e)
						{
							cout << e.what() << endl;
						}
					}
					//cout << endl;
					//myfile << endl;
				}
				myfile << classlabel << endl;
			}

		}
	}

	myfile.close();
	
}


int main(int argc, char** argv)
{
	std::string trainDir, testDir;
	goalSize = cv::Size(24, 24);
	if (argc < 3)
	{
		cout << "Usage:" << endl;
		cout << "-trainDir <dir>" << endl;
		cout << "-testDir <dir>" << endl;
		return -1;
	}
	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-trainDir"))
		{
			trainDir = argv[++i];
		}
		else if (!strcmp(argv[i], "-testDir"))
		{
			testDir = argv[++i];
		}
	}
	//cout << testfilename << " " << trainfilename<< endl;
	cout << "INFO: CSV file will be called \"dir_name\" + txt" << endl;
	trainFileName = trainDir + ".txt";
	testFileName = testDir + ".txt";
	readImages(trainDir, false);
	readImages(testDir, true);
	//system("pause");
	return 0;
}

