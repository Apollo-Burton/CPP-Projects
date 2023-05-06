// NOTE
// Program developed using the VSCode standard output. I believe that it uses the font "Consolas"
// If you use a standard output with a different formatting or font, the proportions of the
// image might be off

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>

int main()
{
	// get the handle of the standard output window
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    // get the dimensions of the standard output window
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdout, &csbi);
    int screen_width = csbi.dwSize.X;
	int screen_height = csbi.dwSize.Y;
	
    std::string path = "C:\\Users\\User\\Programs\\Resources\\lenna.jpg";

	cv::Mat frame = cv::imread(path);

	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

	// Resize image to fit the output window
	double aspect_ratio = static_cast<double>(frame.cols) / static_cast<double>(frame.rows);

	// For some reason, when you zoom out, the std output squishes and stretches your image. 2.3, 2.4, 2.9, 3.9 are the magic numbers that (almost) fix the image at different zoom levels
	if (screen_width == 618)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2.8)));
	else if (screen_width <= 365)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2.3)));
	else if (screen_width <= 621)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2.5)));
	else if (screen_width <= 941)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2.9)));
	else
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 3.9)));
	
	// Grayscale pixel values with corresponding ASCII characters
	std::unordered_map<int, char> brightness_map;
	for (int i = 0; i <= 255; i++)
	{
		if (i < 30)
			brightness_map[i] = ' ';
		else if (i < 55)
			brightness_map[i] = '.';
		else if (i < 85)
			brightness_map[i] = ':';
        else if (i < 115)
            brightness_map[i] = '-';
        else if (i < 140)
			brightness_map[i] = '=';
		else if (i < 165)
			brightness_map[i] = '/';
		else if (i < 180)
			brightness_map[i] = 'o';
		else if (i < 210)
			brightness_map[i] = '0';
		else
			brightness_map[i] = '@';
	}

	// Outputting ASCII characters based on the brightness of the pixel
	cv::uint8_t gray;
	std::cout << "\n\n\n\n\n";

	for (int r = 0; r < frame.rows; r++)
		for (int c = 0; c < frame.cols; c++)
		{
			gray = frame.at<uint8_t>(cv::Point(c, r));
			std::cout << brightness_map[gray];

			// If I change >= to ==, then I know that everything we know about reality will break, c will increase by 2, and then the program will shit itself, spontaniously combust, and explode
			if (c >= frame.cols)
				std::cout << "\n";
		}
}
