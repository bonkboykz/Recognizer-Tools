// ImageCropper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

cv::Mat curImg;
cv::Mat croppedImg;
cv::Rect cropRect(0, 0, 0, 0);
cv::Point P1(0, 0), P2(0, 0);

bool clicked = false;
const std::string winName = "Current Image";
const std::string croppingWinName = "Cropping Current Image";
void checkBoundary(){
	//check croping rectangle exceed image boundary
	if (cropRect.width > curImg.cols - cropRect.x)
		cropRect.width = curImg.cols - cropRect.x;

	if (cropRect.height > curImg.rows - cropRect.y)
		cropRect.height = curImg.rows - cropRect.y;

	if (cropRect.x < 0)
		cropRect.x = 0;

	if (cropRect.y < 0)
		cropRect.height = 0;
}

bool doneCropping()
{
	/*
	std::cout << "Done cropping? y/n" << std::endl;
	
	switch (cv::waitKey())
	{
	case 27:
		exit(0);
		break;
	case (int)'y':
		clicked = true;
		break;
	case (int) 'n':
		clicked = false;
		return 0;
		break;
	default:
		break;
	}
	*/
	checkBoundary();
	if (P1.x > P2.x){
		cropRect.x = P2.x;
		cropRect.width = P1.x - P2.x;
	}
	else {
		cropRect.x = P1.x;
		cropRect.width = P2.x - P1.x;
	}
	std::cout << cropRect.width << " " << cropRect.height << std::endl;
	if (cropRect.width > 0 && cropRect.height > 0){
		croppedImg = curImg(cropRect);
		//cv::destroyWindow(croppingWinName);
		return 1;
		//imshow("cropped", ROI);
	}
	
	return 0;

}
cv::Mat watImg;
void onMouse(int event, int x, int y, int f, void*)
{
	switch (event)
	{

	case  CV_EVENT_LBUTTONDOWN:
		clicked = true;

		P1.x = x;
		P1.y = y;
		P2.x = x;
		P2.y = y;
		//cv::namedWindow(croppingWinName, CV_WINDOW_NORMAL);
		//cv::resizeWindow(croppingWinName, 640, 480);
	//	cv::rectangle(curImg,
		//	P1,
			//P2,
			//cv::Scalar(0, 255, 255));
		break;

	case  CV_EVENT_LBUTTONUP:
		P2.x = x;
		P2.y = y;
		//std::cout << "Here" << std::endl;
		//std::cout << P1.x << " " << P1.y << std::endl;
		//std::cout << P2.x << " " << P2.y << std::endl;
		watImg = curImg.clone();
		cv::rectangle(watImg,
			P1,
			P2,
			cv::Scalar(0, 255, 255), 
			3);
		
		//cv::imshow(croppingWinName, watImg);
		//cv::waitKey(10);
		cv::imshow(winName, watImg);
		cv::waitKey(30);
		clicked = false;
		break;

	case  CV_EVENT_MOUSEMOVE:
		if (clicked)
		{
			P2.x = x;
			P2.y = y;
			watImg = curImg.clone();
			cv::rectangle(watImg,
				P1,
				P2,
				cv::Scalar(0, 255, 255),
				3);

			//cv::imshow(croppingWinName, watImg);
			//cv::waitKey(10);
			cv::imshow(winName, watImg);
			cv::waitKey(1);
		}
		break;

	default:   break;

	}
	if (clicked)
	{
		if (P1.x > P2.x){
			cropRect.x = P2.x;
			cropRect.width = P1.x - P2.x;
		}
		else {
			cropRect.x = P1.x;
			cropRect.width = P2.x - P1.x;
		}

		if (P1.y > P2.y){
			cropRect.y = P2.y;
			cropRect.height = P1.y - P2.y;
		}
		else {
			cropRect.y = P1.y;
			cropRect.height = P2.y - P1.y;
		}
	}
}
/*
int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}
*/
using namespace boost::filesystem;
void iterateThroughDir(std::string dirName)
{
	path dir(dirName);
	directory_iterator end;
	for (directory_iterator file(dir); file != end; file++)
	{
		std::cout << file->path().filename().string() << std::endl;
		if (is_regular_file(*file))
		{
			std::string curFileName = file->path().filename().string();
			curImg = cv::imread(file->path().string(), CV_LOAD_IMAGE_COLOR);
			cv::imshow(winName, curImg);
			l:
			std::cout << "Do crop and press 'd'" << std::endl;
			switch (cv::waitKey()) 
			{
			case 27:
				exit(0);
				break;
			case (int)'d':
				if (!doneCropping()) goto l;
				break;
			default:
				break;
			}
			//std::cout << dirName + "Cropped" << std::endl;
			std::string curPathName = dirName + "Cropped";
			boost::filesystem::path curDir(curPathName);
			if (boost::filesystem::create_directory(curDir))
				std::cout << "Created folder: " << curPathName << std::endl;
			curPathName += "\\" + curFileName;
			cv::imwrite(curPathName, croppedImg);
		}
	}
}
using namespace std;
int main(int argc, char argv[])
{
	std::string curDir;
	cout << "Input directory name: "; cin >> curDir; cout << endl;
	cv::namedWindow(winName, CV_WINDOW_NORMAL);
	cv::resizeWindow(winName, 1024, 768);
	cv::setMouseCallback(winName, onMouse, NULL);
	iterateThroughDir(curDir);
	cout << "Finished" << endl;
	return 0;
}

