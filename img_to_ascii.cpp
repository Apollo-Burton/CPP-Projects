#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <Windows.h>
#include <fstream>

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

std::string convert_image(cv::Mat image, int num_channels, bool inverted)
{
    std::vector<char> chars = {' ', '.', ':', '-', '=', '/', 'o', '0', '@'};
    std::string ascii_image;
	int index;

    for (int r = 0; r < image.rows; r++)
    {
        for (int c = 0; c < image.cols; c++)
        {
			if (image.channels() == 1)
			{
				uchar pixel_data = image.at<uchar>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			if (image.channels() == 2)
			{
				cv::Vec2b pixel_data = image.at<cv::Vec2b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			else if (image.channels() == 3)
			{
				cv::Vec3b pixel_data = image.at<cv::Vec3b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
			else if (image.channels() == 4)
			{
				cv::Vec4b pixel_data = image.at<cv::Vec4b>(r, c);
				index = static_cast<int>(abs(std::round((get_intensity(pixel_data) * (chars.size() - 1))) - (chars.size() - 1) * inverted));
			}
            ascii_image += chars[index];
        }
        ascii_image += "\n";
    }
    return ascii_image;
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
	std::cout << "Enter image path or a number from 1-10 for sample images:\n";
    std::getline(std::cin, path);

	char invert;
	bool inverted = false;
	std::cout << "Inverted colors? (Y/n) ";
	std::cin >> invert;

	if (invert == 'Y')
		inverted = true;

    if (path == "1")
        path = "C:\\Users\\User\\Programs\\Resources\\lenna.jpg";
    else if (path == "2")
        path = "C:\\Users\\User\\Programs\\Resources\\Digital Combat Simulator  Black Shark Screenshot 2022.09.30 - 23.03.08.59.png";
    else if (path == "3")
        path = "C:\\Users\\User\\Programs\\Resources\\Red Dead Redemption 2 Screenshot 2023.03.10 - 20.07.43.05.png";
    else if (path == "4")
        path = "C:\\Users\\User\\Programs\\Resources\\Star Wars Jedi  Fallen Order Screenshot 2022.06.19 - 18.31.17.34.png";
	else if (path == "5")
		path = "C:\\Users\\User\\Programs\\Resources\\Red Dead Redemption 2 Screenshot 2023.05.26 - 17.35.12.94.png";
	else if (path == "6")
		path = "C:\\Users\\User\\Programs\\Resources\\Forza Horizon 5 Screenshot 2022.02.12 - 15.55.22.69.png";
	else if (path == "7")
		path = "C:\\Users\\User\\Programs\\Resources\\Star Wars Jedi  Fallen Order Screenshot 2022.06.19 - 17.55.37.94.png";
    else if (path == "8")
        path = "C:\\Users\\User\\Programs\\Resources\\cards.jpg";
	else if (path == "9")
		path = "C:\\Users\\User\\Programs\\Resources\\Red Dead Redemption 2 Screenshot 2023.05.26 - 17.35.21.40.png";
	else if (path == "10")
		path = "C:\\Users\\User\\Programs\\Resources\\jakob-rosen-KCXM1vtXvJs-unsplash.jpg";
    else if (path == "richter")
		path = "C:\\Users\\User\\Videos\\Captures\\3.png";

	cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
	if (image.empty())
	{
		std::cout << "Image failed to open. Check file/path integrity\n";
		return -1;
	}

	// Resize image to fit the output window
	double aspect_ratio = static_cast<double>(image.cols) / static_cast<double>(image.rows);

	// Resize image to fit the output window and keep it's original aspect ratio
	cv::resize(image, image, cv::Size(static_cast<int>(std::round(screen_height * aspect_ratio * 2)), screen_height - 1));

	// Outputting ASCII characters based on the brightness of the pixel
	std::cout << "\n\n\n\n\n";

	std::string ascii_image = convert_image(image, image.channels(), inverted);
	std::cout << ascii_image;

	char pref;
	std::cout << "\n\n\nExport to text file? (Y/n) ";
	std::cin >> pref;

	if (pref != 'Y')
		return 0;
	
	std::string file_name;
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '\\') // The file name should not include the file path
		{
			file_name.clear();
			continue;
		}
		else if (path[i] == '.') // The file name should not include .png or .jpg
			break;
		file_name += path[i];
	}
	
	std::ofstream new_file;
	std::string ascii_image_path = "C:\\Users\\User\\ascii_art\\" + file_name + " (ASCII ART).txt";
	new_file.open(ascii_image_path, std::ios::out);

	if (!new_file.is_open())
	{
		std::cout << "Failed to open/create file for writing\n";
		return -1;
	}

	new_file << ascii_image; // Write the ascii image to the file

	if (new_file.fail())
	{
		std::cout << "File succesfully opened/created, but failed to write image\n";
		new_file.close();
		std::remove(ascii_image_path.c_str()); // Delete file
		return -1;
	}

	std::cout << "File succesfully opened/created and written to\n" << "The file path is: " + ascii_image_path << "\n";
	new_file.close();
	return 0;
}
