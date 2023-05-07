#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <chrono>
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

	std::string path = "C:\\Users\\User\\Programs\\Resources\\Star Wars Jedi  Fallen Order 2022.06.19 - 00.42.18.03.DVR.mp4";
	//path = "C:\\Users\\User\\Programs\\Resources\\Titanfall2 2022.05.13 - 23.24.44.02.DVR.mp4";
	path = "C:\\Users\\User\\Programs\\Resources\\Digital Combat Simulator  Black Shark 2022.11.23 - 21.32.24.02.DVR.mp4";
	//path = "C:\\Users\\User\\Programs\\Resources\\Desktop 2023.03.17 - 23.26.28.02.DVR.mp4";
	//path = "C:\\Users\\User\\Programs\\Resources\\Chivalry 2 2022.10.22 - 15.40.58.02.DVR_Trim.mp4";
	//path = "C:\\Users\\User\\Programs\\Resources\\Desktop 2023.03.17 - 23.27.16.03.DVR.mp4";
	//path = "C:\\Users\\User\\Programs\\Resources\\Blade and Sorcery 2023.05.06 - 16.37.12.07.mp4";

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

	cv::VideoCapture stream(path);
	double fps = stream.get(cv::CAP_PROP_FPS);
	int ms_between_frames = static_cast<int>(1000.0 / fps);
	cv::Mat frame;
	double aspect_ratio;
	cv::uint8_t gray;
	std::string ascii_frame;
	std::string row;
	std::stringstream output;

	std::cout << "\n\n\n";
	while (stream.read(frame))
	{
		if (!stream.read(frame))
			break;

		auto start_time = std::chrono::high_resolution_clock::now();

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

		// Resize image to fit the output window
		aspect_ratio = static_cast<double>(frame.cols) / static_cast<double>(frame.rows);

		// For some reason, when you zoom out, the std output squishes and stretches your image. 2.2, 3.7 are the magic numbers that (almost) fix the image at different zoom levels
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
		
		for (int r = 0; r < frame.rows; r++)
		{
			for (int c = 0; c < frame.cols; c++)
			{
				gray = frame.at<uint8_t>(r, c);
				output << brightness_map[gray];
			}
			output << "\n";
		}

		std::cout << output.str() << std::flush;
		output.str("");
		COORD cursor_pos = {0, 0};
		SetConsoleCursorPosition(hStdout, cursor_pos);

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		if (duration < ms_between_frames * 2)
			Sleep(static_cast<DWORD>((ms_between_frames * 2) - duration));
	}
	
	system("cls");
	std::cout << "Playback finished\n";
}
