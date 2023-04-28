#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>

int main()
{
	// get the handle of the standard output window
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // get the dimensions of the standard output window
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdout, &csbi);
    int width = csbi.dwSize.X;
	int height = csbi.dwSize.Y;

	std::string path = "C:\\Users\\User\\Lenna.jpg";

	cv::Mat frame = cv::imread(path);
	
	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

	// Resize image to fit the output window
	if (frame.cols > width)
		cv::resize(frame, frame, cv::Size(width, static_cast<int>(height * 1.25)));
	else
		cv::resize(frame, frame, cv::Size(frame.cols, static_cast<int>(height * 1.25))); // A character's height is significantly larger than a pixel, so smush the image vertically to make it look better

	// Grayscale pixel values with corresponding ASCII characters
	std::unordered_map<int, char> brightness_map;
	for (int i = 0; i <= 255; i++)
	{
		if (i < 20)
			brightness_map[i] = ' ';
		else if (i < 45)
			brightness_map[i] = '.';
		else if (i < 85)
			brightness_map[i] = ':';
        else if (i < 110)
            brightness_map[i] = 'o';
        else if (i < 135)
			brightness_map[i] = '2';
		else if (i < 150)
			brightness_map[i] = 'O';
		else if (i < 210)
			brightness_map[i] = '0';
		else
			brightness_map[i] = '@';
	}

	// Outputting ASCII characters based on the brightness of the pixel
	int count = 1;
	cv::uint8_t gray;
	std::cout << "\n\n\n\n\n";
	
	for (int r = 0; r < frame.rows; r++)
		for (int c = 0; c < frame.cols; c++)
		{
			gray = frame.at<uint8_t>(cv::Point(c, r));
			std::cout << brightness_map[gray];

			if (count >= frame.cols)
			{
				std::cout << "\n";
				count = 1;
			}
			else
				count++;
		}
}
