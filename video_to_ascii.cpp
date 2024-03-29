#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <Windows.h>
#include <algorithm>

const int MAX_CHANNEL_VALUE = 255;

// Get intensity of a pixel with 1 channel
double get_intensity(uchar& pixel)
{
	return pixel / MAX_CHANNEL_VALUE;
}

// Get intensity of a pixel with 2 channels
double get_intensity(cv::Vec2b& pixel)
{
	double sum = 0;

	for (int i = 0; i < 2; i++)
		sum += pixel[i];

	return sum / (MAX_CHANNEL_VALUE * 2);
}

// Get intensity of a pixel with 3 channels
double get_intensity(cv::Vec3b& pixel)
{
	double sum = 0;

	for (int i = 0; i < 3; i++)
		sum += pixel[i];

	return sum / (MAX_CHANNEL_VALUE * 3);
}

// Get intensity of a pixel with 4 channels
double get_intensity(cv::Vec4b& pixel)
{
	double sum = 0;

	for (int i = 0; i < 4; i++)
		sum += pixel[i];

	return sum / (MAX_CHANNEL_VALUE * 4);
}

void frame_to_ascii(cv::Mat frame, std::string& ascii_frame, int screen_width, int screen_height, bool inverted)
{
	if (frame.empty()) // Stop if there are no frames left
		return;

	std::vector<char> chars = {' ', '.', ':', '-', '=', '/', 'o', '0', '@'}; // Characters that ascii_frame will consist of
	int index;

	double aspect_ratio = static_cast<double>(frame.cols) / static_cast<double>(frame.rows);

	// Resize frame to fit the output window and keep it's original aspect ratio
	if (std::round(screen_height * aspect_ratio * 2) <= screen_width)
		cv::resize(frame, frame, cv::Size(static_cast<int>(std::round(screen_height * aspect_ratio * 2)), screen_height - 1));
	else
		cv::resize(frame, frame, cv::Size(screen_width, static_cast<int>(std::round(screen_width / aspect_ratio / 2))));

	ascii_frame.clear(); // Make sure output string is empty

	for (int r = 0; r < frame.rows; r++)
	{
		for (int c = 0; c < frame.cols; c++)
		{
			// Adding a character to ascii_frame based on the corresponding pixels intensity
			if (frame.channels() == 1)
			{
				uchar pixel_data = frame.at<uchar>(r, c);
				// If "inverted" is true, it will equal 1. If we subtract the size of "chars" from the index, we get the negative reverse value, so we use abs() to turn it positive. If "inverted" is false,
				// it equals 0, and we subtract: chars.size() - 1 * 0 from the index, which wont invert the value
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			else if (frame.channels() == 2)
			{
				cv::Vec2b pixel_data = frame.at<cv::Vec2b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			else if (frame.channels() == 3)
			{
				cv::Vec3b pixel_data = frame.at<cv::Vec3b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			else if (frame.channels() == 4)
			{
				cv::Vec4b pixel_data = frame.at<cv::Vec4b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}

			ascii_frame += chars[index];
		}

		ascii_frame += "\n"; // Create new row
	}
}

void detect_inputs(int fps, int frames_size, int& frame_index)
{
	while (frame_index < frames_size)
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (frame_index - fps * 5 >= 0)
				frame_index -= fps * 5;
			else
				frame_index = 0;

			Sleep(250);
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			if (frame_index + fps * 5 < frames_size)
				frame_index += fps * 5;
			else
				frame_index = frames_size - 1;

			Sleep(250);
		}
	}
}

// std::min just likes to cause 74 PROBLEMS in the program for no reason
int minimum(int a, int b)
{
	return (b < a) ? b : a;
}

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	// get the handle of the standard output window
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// get the dimensions of the standard output window
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int screen_width = csbi.dwSize.X;
	int screen_height = csbi.dwSize.Y;

	std::string path;
	std::cout << "Enter video path:\n";
	std::getline(std::cin, path);

	// Set up variables
	cv::VideoCapture video(path);

	if (!video.isOpened())
	{
		std::cout << "Video failed to open. Check file/path integrity\n";
		return -1;
	}

	char invert;
	bool inverted = false;
	std::cout << "Inverted colors? (Y/n) ";
	std::cin >> invert;

	if (invert == 'Y')
		inverted = true;

	cv::Mat frame;
	int count = 0;
	uint dot_count = 0;
	int percentage_processed = 0;
	int total_frames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));
	double fps = video.get(cv::CAP_PROP_FPS);
	int ms_between_frames = static_cast<int>(std::round(1000.0 / fps));
	int limit = 0;
	bool error_processing = false;
	unsigned int num_system_threads = std::thread::hardware_concurrency(); // Get number of available threads

	if (num_system_threads == 0)
	{
		std::cout << "Unable to get number of system threads\n";
		return -1;
	}

	std::vector<std::thread> threads;
	std::vector<std::string> frames;
	std::vector<std::string> temp_frames(static_cast<int>(num_system_threads * 0.75)); // "temp_frames" will be the vector the threads write to before those rendered frames get sent to the "threads" vector. 1 string (frame) per thread

	std::cout << "Enter how many seconds of the video to process (0 or less is the whole video): ";
	std::cin >> limit;

	limit = static_cast<int>(std::round(limit * fps));

	if (limit <= 0 || limit > total_frames)
		limit = total_frames;

	system("cls"); // Clear screen
	std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now(); // Start processing timer

	while (count < limit)
	{
		// Start threads in parallel
		for (int i = 0; i < minimum(static_cast<int>(temp_frames.size()), limit - count); i++) // So basically, I want to die
		{
			// Load next frame
			if (!video.read(frame))
			{
				count = limit; // If something went wrong, break out of the loop after joining the threads
				error_processing = true;
				break;
			}

			threads.emplace_back(frame_to_ascii, frame, std::ref(temp_frames[i]), screen_width, screen_height, inverted); // Start thread using it's designated frame slot in "temp_frames" as the output
			frame.release(); // Clear any remnants of the previous frame
		}

		for (int i = 0; i < temp_frames.size(); i++)
		{
			if (i < threads.size())
				threads[i].join();

			frames.push_back(temp_frames[i]); // Write the completed frames to the frames vector
			temp_frames[i].clear(); // Clear temporary frames
			count++;
		}

		threads.clear();

		// Display progress bar and "Processing..." animation
		percentage_processed = static_cast<int>(static_cast<double>(count) / limit * 100);

		if (percentage_processed > 100)
			percentage_processed = 100;

		SetConsoleCursorPosition(hStdout, {0, 0}); // Set cursor posititon to the top left of the console

		// Display progress bar
		std::cout << std::string(percentage_processed, '@') << std::string (100 - percentage_processed, '-') << "\n";
		std::cout << std::string(percentage_processed, '@') << std::string (100 - percentage_processed, '-') << "\n";
		std::cout << std::string(percentage_processed, '@') << std::string (100 - percentage_processed, '-') << "\n";
		std::cout << std::string(48, ' ') << percentage_processed << "%\n"; // Display percent progressed in the middle of the progress bar

		if (dot_count >= 3)
			dot_count = 0;
		else
			dot_count++;

		std::cout << "\nProcessing" + std::string(dot_count, '.') + std::string(3 - dot_count, ' ') << "\n";
	}

	system("cls");

	// Display diagnostic information
	if (error_processing)
	{
		std::cout << "There was an error processing the video, however, " << count / fps << " seconds were still processed\n";
	}

	std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "It took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0  << " seconds to render " << count << " frames\n";
	std::cout << "It took about " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / count << " milliseconds to render 1 frame\n";
	std::cout << "It took about " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 / count * fps << " seconds to render 1 second of the video\n";
	std::cout << "At this rate, it would take " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 / count * fps * 300 / 60 << " minutes to render a 5 minute video\n";

	// Set up variables
	long long duration;
	double total = 0;
	int p_count = 0;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
	char c;
	std::cout << "\nVideo done processing. Enter any key to play video ";
	std::cin >> c;

	int frame_index = 0;
	std::thread inputs_thread(detect_inputs, static_cast<int>(std::round(fps)), static_cast<int>(frames.size()), std::ref(frame_index));

	std::cout << "\n\n\n";

	for (; frame_index < frames.size(); frame_index++)
	{
		start_time = std::chrono::high_resolution_clock::now(); // Start display timer

		std::cout << frames[frame_index]; // Output frame

		SetConsoleCursorPosition(hStdout, {0, 0}); // Set cursor position to the top left of the console

		end_time = std::chrono::high_resolution_clock::now(); // End display timer
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count(); // Total duration of displaying frame

		// Taking average of time taken to display frame
		total += duration;
		p_count++;

		// This makes sure the video retains it's original speed. The new milliseconds between frames is ms_between_frames + duration.
		// If we want to retain the original speed of the video, we need to keep the original milliseconds between frames, which is ms_between_frames - duration
		if (duration < ms_between_frames)
			Sleep(static_cast<DWORD>(std::round(ms_between_frames - duration))); // What even is a DWORD? I'm sleep deprived it's 1:06 AM.
	}

	inputs_thread.join();

	// Clear screen of the last frame
	system("cls");
	std::cout << "Playback finished\n";
	std::cout << "The average time taken to output 1 frame was " << total / p_count << " milliseconds\n";
}
