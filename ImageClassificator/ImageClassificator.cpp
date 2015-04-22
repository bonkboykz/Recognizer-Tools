// ImageClassificator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace boost::filesystem;
std::string concatStrings(std::string curStr, char curClass)
{
	std::string curClassStr = "";
	curClassStr += '(';
	curClassStr += curClass;
	curClassStr += ')';
	curStr.insert(curStr.length() - 7, curClassStr);
	//std::cout << curClassStr << std::endl;
	//std::cout << curStr << std::endl;
	return curStr;
}
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
			cv::Mat curImg = cv::imread(file->path().string(), CV_LOAD_IMAGE_COLOR);
			cv::imshow("Current Image", curImg);
			switch (cv::waitKey(30000))
			{
			case 27:
				exit(0);
				break;
			case (int)'0':
				curFileName = concatStrings(curFileName, '0');
				break;
			case (int)'1':
				curFileName = concatStrings(curFileName, '1');
				break;
			case (int)'2':
				curFileName = concatStrings(curFileName, '2');
				break;
			case (int)'3':
				curFileName = concatStrings(curFileName, '3');
				break;
			case (int)'4':
				curFileName = concatStrings(curFileName, '4');
				break;
			case (int)'s':
				continue;
				break;
			case (int)'S':
				continue;
				break;
			}
			std::cout << dirName + "Changed" << std::endl;
			std::string curPathName = dirName + "Changed";
			boost::filesystem::path curDir(curPathName);
			if (boost::filesystem::create_directory(curDir))
				std::cout << "Created folder: " << curPathName << std::endl;
			curPathName += "\\" + curFileName;
			cv::imwrite(curPathName, curImg);
		}
	}
}
using namespace std;
int main(int argc, char argv[])
{
	std::string curDir;
	cout << "Input directory name: "; cin >> curDir; cout << endl;
	iterateThroughDir(curDir);
	cout << "Finished" << endl;
	return 0;
}