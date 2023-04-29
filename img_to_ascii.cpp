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

	std::string path;
    int num;
    std::cin >> num;

    if (num == 1)
        path = "C:\\Users\\User\\Videos\\Captures\\3.png";
    else if (num == 2)
        path = "C:\\Users\\User\\Programs\\Resources\\Digital Combat Simulator  Black Shark Screenshot 2022.09.30 - 23.03.08.59.png";
    else if (num == 3)
        path = "C:\\Users\\User\\Programs\\Resources\\Red Dead Redemption 2 Screenshot 2023.03.10 - 20.07.43.05.png";
    else if (num == 4)
        path = "C:\\Users\\User\\Programs\\Resources\\Star Wars Jedi  Fallen Order Screenshot 2022.06.19 - 18.31.17.34.png";
	else if (num == 5)
		path = "C:\\Users\\User\\Programs\\Resources\\ULTRAKILL 1_12_2023 10_59_12 PM.png";
	else if (num == 6)
		path = "C:\\Users\\User\\Programs\\Resources\\Forza Horizon 5 Screenshot 2022.02.12 - 15.55.22.69.png";
    else if (num == 7)
        path = "C:\\Users\\User\\Programs\\Resources\\cpp.png";
    else
        path = "C:\\Users\\User\\Programs\\Resources\\lenna.jpg";

	cv::Mat frame = cv::imread(path);
	
	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

	// Resize image to fit the output window
	double aspect_ratio = static_cast<double>(frame.cols) / static_cast<double>(frame.rows);

	// For some reason, when you zoom out, the std output squishes and stretches your image. 2, 2.3, 3.6 are the magic numbers that fix the image at different levels
	if (screen_width <= 556)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2)));
	else if (screen_width <= 786)
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 2.3)));
	else
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio) / 3.6)));
	
	// Grayscale pixel values with corresponding ASCII characters
	std::unordered_map<int, char> brightness_map;
	for (int i = 0; i <= 255; i++)
	{
		if (i < 20)
			brightness_map[i] = ' ';
		else if (i < 45)
			brightness_map[i] = '.';
		else if (i < 75)
			brightness_map[i] = ':';
        else if (i < 110)
            brightness_map[i] = '-';
        else if (i < 130)
			brightness_map[i] = '=';
		else if (i < 150)
			brightness_map[i] = '/';
		else if (i < 170)
			brightness_map[i] = 'o';
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
