// TestHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/
using namespace std;

boost::posix_time::ptime tick, now;

std::string algoName;
int algoNum;
vector <cv::Mat> training_data;
vector <int> training_classes;

vector <cv::Mat> testing_data;
vector <int> testing_classes;
cv::Size goalSize(24, 24);

#define NUMBER_OF_CLASSES 5

void readDataFromDir(int* numOfSamples, std::string trainDir, std::string testDir)
{
	using namespace boost::filesystem;
	path trainPath(trainDir);
	path testPath(testDir);
	directory_iterator end;
	for (directory_iterator file(trainPath); file != end; file++)
	{
		//cout << file->path().string() << endl;
		if (is_regular_file(*file))
		{
			string filestring = file->path().filename().string();
			int classlabel = stoi(filestring.substr(filestring.length() - 6, 1));
			//classlabel--;
			if (classlabel > 4) continue;
			training_classes.push_back(classlabel);

			cv::Mat curImg = cv::imread(file->path().string(), CV_LOAD_IMAGE_GRAYSCALE);
			//cv::Mat processed;
			
			//cv::resize(curImg, processed, goalSize);
			training_data.push_back(curImg);
			//cv::imshow("Check", cv::imread(file->path().string()));
			//cv::waitKey(100);
			
		}
	}
	(*numOfSamples) = training_data.size();

	for (directory_iterator file(testPath); file != end; file++)
	{
		//cout << file->path().string() << endl;
		if (is_regular_file(*file) && file->path().extension().string() == ".bmp")
		{
			string filestring = file->path().filename().string();
			int classlabel = stoi(filestring.substr(filestring.length() - 6, 1));
			//classlabel--;
			if (classlabel > 4) continue;
			testing_classes.push_back(classlabel);
			
			cv::Mat curImg = cv::imread(file->path().string(), CV_LOAD_IMAGE_GRAYSCALE);
			//cv::Mat processed;
			//cv::resize(curImg, processed, goalSize);
			testing_data.push_back(curImg);
			
		}
	}
}
void eigenFaceCheck()
{

	double threshold = 0.0;
	int numOfSamples = 0;
	std::string trainDir, testDir;
	cout << "Write train and test directory" << endl;
	cout << "Train Dir: "; cin >> trainDir;
	cout << "Test Dir: "; cin >> testDir;
	readDataFromDir(&numOfSamples, trainDir, testDir);
	//cout << "Write threshold for recognizer" << endl;
	//cout << "Threshold: "; cin >> threshold;
	cv::Ptr<cv::FaceRecognizer> fr = cv::createEigenFaceRecognizer(numOfSamples);
	tick = boost::posix_time::second_clock::local_time();
	cout << "Using training database: " << trainDir << "\tNumber of training samples: " << training_data.size() << endl;
	fr->train(training_data, training_classes);
	cv::Mat test_sample;
	int correct_class = 0;
	int wrong_class = 0;

	int false_positives[NUMBER_OF_CLASSES] = { 0, 0, 0, 0, 0 };

	cout << "Using testing database: " << testDir << endl;
	for (unsigned int tsample = 0; tsample < testing_data.size(); tsample++)
	{

		int predicted_label = -1;
		double predicted_confidence = 0.0;
		test_sample = testing_data[tsample];

		fr->predict(test_sample, predicted_label, predicted_confidence);

		cout << "Testing sample " << tsample << " -> " << "class result (Emotion " << predicted_label << ")" << endl;

		if (predicted_label != testing_classes[tsample])
		{

			wrong_class++;

			false_positives[predicted_label]++;

		}
		else
		{
			correct_class++;
		}
	}

	cout << "Results on the testing database: " << testDir << endl;
	cout << "\tCorrect classification: " << correct_class << " ";
	cout << "(" << (double)correct_class * 100 / testing_data.size() << ")" << endl;
	cout << "\tWrong classifications: " << wrong_class << " ";
	cout << "(" << (double)wrong_class * 100 / testing_data.size() << ")" << endl;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		printf("\tClass (Emotion %d) false postives 	%d (%g%%)\n", i,
			false_positives[i],
			(double)false_positives[i] * 100 / testing_data.size());
	}

	now = boost::posix_time::second_clock::local_time();
	__int64 diff = now.time_of_day().total_milliseconds() - tick.time_of_day().total_milliseconds();
	cout << "Executing time (train + predict): " << diff << " msec" << endl;

}
void lbpCheck()
{
	double threshold = 0.0;
	int numOfSamples = 0;
	std::string trainDir, testDir;
	cout << "Write train and test directory" << endl;
	cout << "Train Dir: "; cin >> trainDir;
	cout << "Test Dir: "; cin >> testDir;
	readDataFromDir(&numOfSamples, trainDir, testDir);
	//cout << "Write threshold for recognizer" << endl;
	//cout << "Threshold: "; cin >> threshold;
	cv::Ptr<cv::FaceRecognizer> fr = cv::createLBPHFaceRecognizer();
	tick = boost::posix_time::second_clock::local_time();
	cout << "Using training database: " << trainDir << "\tNumber of training samples: " << training_data.size() << endl;
	fr->train(training_data, training_classes);

	cv::Mat test_sample;
	int correct_class = 0;
	int wrong_class = 0;

	int false_positives[NUMBER_OF_CLASSES] = { 0, 0, 0, 0, 0 };

	cout << "Using testing database: " << testDir << endl;
	for (unsigned int tsample = 0; tsample < testing_data.size(); tsample++)
	{

		int predicted_label = -1;
		double predicted_confidence = 0.0;
		test_sample = testing_data[tsample];

		fr->predict(test_sample, predicted_label, predicted_confidence);

		cout << "Testing sample " << tsample << " -> " << "class result (Emotion " << predicted_label << ")" << endl;

		if (predicted_label != testing_classes[tsample])
		{

			wrong_class++;

			false_positives[predicted_label]++;

		}
		else
		{
			correct_class++;
		}
	}

	cout << "Results on the testing database: " << testDir << endl;
	cout << "\tCorrect classification: " << correct_class << " ";
	cout << "(" << (double)correct_class * 100 / testing_data.size() << ")" << endl;
	cout << "\tWrong classifications: " << wrong_class << " ";
	cout << "(" << (double)wrong_class * 100 / testing_data.size() << ")" << endl;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		printf("\tClass (Emotion %d) false postives 	%d (%g%%)\n", i,
			false_positives[i],
			(double)false_positives[i] * 100 / testing_data.size());
	}

	now = boost::posix_time::second_clock::local_time();
	__int64 diff = now.time_of_day().total_milliseconds() - tick.time_of_day().total_milliseconds();
	cout << "Executing time (train + predict): " << diff << " msec" << endl;
}

using namespace cv;

#define NUMBER_OF_TRAINING_SAMPLES 42
int numberOfTrainingSamples;
#define ATTRIBUTES_PER_SAMPLE 576
#define NUMBER_OF_TESTING_SAMPLES 3
int numberOFTestingSamples;
void countTheNumberOfLines(std::string filename, bool isTest)
{
	int numberOfLines = 0;
	std::string curLine;
	ifstream filestream(filename);
	while (std::getline(filestream, curLine))
	{
		//qDebug() << QString::fromStdString(line);
		numberOfLines++;
	}
	filestream.clear();
	filestream.seekg(0, std::ios::beg);
	//numberOfLines--;

	if (isTest) numberOFTestingSamples = numberOfLines;
	else numberOfTrainingSamples = numberOfLines;
}
int readDataFromCSV(std::string filename, Mat data, Mat classes, int n_samples)
{

	int classlabel; // the class label
	float tmpf;
	
	

	ifstream filestream(filename);
	
	if (!filestream.is_open())
	{
		printf("ERROR: cannot read file %s\n", filename);
		return 0; // all not OK
	}
	
	using namespace std;

	for (int line = 0; line < n_samples; line++)
	{

		//cout << line << endl;
		// for each attribute on the line in the file

		for (int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
		{
			//cout << attribute << " ";
			if (attribute < ATTRIBUTES_PER_SAMPLE)
			{

				// first 256 elements (0-255) in each line are the attributes

				//fscanf(f, "%f,", &tmpf);
				filestream >> tmpf;
				//cout << tmpf << " ";
				data.at<float>(line, attribute) = tmpf;

			}
			else if (attribute == ATTRIBUTES_PER_SAMPLE)
			{


				//fscanf(f, "%i,", &classlabel);
				filestream >> classlabel;
				//cout << "classlabel " << classlabel << endl;
				//cout << "classlabel:"<< classlabel << endl;
				classes.at<float>(line, classlabel) = 1.0;
			}
		}
		//cout << endl;
	}

	filestream.close();
	return 1; // all OK
}

void nnCheck()
{




	Mat classificationResult = Mat(1, NUMBER_OF_CLASSES, CV_32FC1);
	Point max_loc = Point(0, 0);
	std::string trainCSV ,testCSV;

	cout << "Write train and test CSV names" << endl;
	cout << "Train CSV: "; cin >> trainCSV;
	cout << "Test CSV: "; cin >> testCSV;
	
	countTheNumberOfLines(trainCSV, false);
	countTheNumberOfLines(testCSV, true);
	
	Mat training_data = Mat(numberOfTrainingSamples, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
	Mat training_classifications = Mat(numberOfTrainingSamples, NUMBER_OF_CLASSES, CV_32FC1);


	Mat testing_data = Mat(numberOFTestingSamples, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
	Mat testing_classifications = Mat::zeros(numberOFTestingSamples, NUMBER_OF_CLASSES, CV_32FC1);

	if (readDataFromCSV(trainCSV, training_data, training_classifications, numberOfTrainingSamples) &&
		readDataFromCSV(testCSV, testing_data, testing_classifications, numberOFTestingSamples))
	{


		int layers_d[] = { ATTRIBUTES_PER_SAMPLE, 5, NUMBER_OF_CLASSES };
		Mat layers = Mat(1, 3, CV_32SC1);
		layers.at<int>(0, 0) = layers_d[0];
		layers.at<int>(0, 1) = layers_d[1];
		layers.at<int>(0, 2) = layers_d[2];

		CvANN_MLP* nnetwork = new CvANN_MLP;
		nnetwork->create(layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

		CvANN_MLP_TrainParams params = CvANN_MLP_TrainParams(
			cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001),
			CvANN_MLP_TrainParams::BACKPROP,
			0.1,
			0.1);
		tick = boost::posix_time::second_clock::local_time();
		cout << "Using training database: " << trainCSV << "\tNumber of training samples: " << training_data.rows << endl;
		int iterations = nnetwork->train(training_data, training_classifications, Mat(), Mat(), params);

		cout << "Training iterations " << iterations << endl;

		Mat test_sample;
		int correct_class = 0;
		int wrong_class = 0;

		int false_positives[NUMBER_OF_CLASSES] = { 0, 0, 0, 0, 0};

		cout << "Using testing database: " << testCSV << endl;
		
		for (int tsample = 0; tsample < numberOFTestingSamples; tsample++)
		{


			test_sample = testing_data.row(tsample);

			nnetwork->predict(test_sample, classificationResult);


			minMaxLoc(classificationResult, 0, 0, 0, &max_loc);

			printf("Testing Sample %i -> class result (Emotion %d)\n", tsample, max_loc.x);

			if (!(testing_classifications.at<float>(tsample, max_loc.x)))
			{

				wrong_class++;

				false_positives[(int)max_loc.x]++;

			}
			else
			{

				correct_class++;
			}
		}


		cout << "Results on the testing database: " << testCSV << endl;
		cout << "\tCorrect classification: " << correct_class << " ";
		cout << "(" << (double)correct_class * 100 / numberOFTestingSamples << ")" << endl;
		cout << "\tWrong classifications: " << wrong_class << " ";
		cout << "(" << (double)wrong_class * 100 / numberOFTestingSamples << ")" << endl;
		for (int i = 0; i < NUMBER_OF_CLASSES; i++)
		{
			printf("\tClass (Emotion %d) false postives 	%d (%g%%)\n", i,
				false_positives[i],
				(double)false_positives[i] * 100 / numberOFTestingSamples);
		}
		now = boost::posix_time::second_clock::local_time();
		__int64 diff = now.time_of_day().total_milliseconds() - tick.time_of_day().total_milliseconds();
		cout << "Executing time (train + predict): " << diff << " msec" << endl;
	}
}

int main(int argc, char** argv)
{

	if (argc < 2)
	{
		cout << "Usage:" << endl;
		cout << "-type <algo_num>" << endl;
		cout << "Algorithm numbers:" << endl;
		cout << "0 - Eigenface" << endl;
		cout << "1 - LBP" << endl;
		cout << "2 - Neural Network" << endl;
		return 0;
	}

	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-type"))
		{
			algoName = argv[++i];
		}
	}
	algoNum = std::stoi(algoName);
	cout << "Checking ";
	
	switch (algoNum)
	{
	case 0:
		cout << "Eigenface" << endl;
		eigenFaceCheck();
		break;
	case 1:
		cout << "LBP" << endl;
		lbpCheck();
		break;
	case 2:
		cout << "Neural Network" << endl;
		nnCheck();
		break;
	}
	return 0;
}
