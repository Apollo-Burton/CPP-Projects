#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

double get_pixel_intensity(const cv::Vec3b& pixel)
{
	double sum = 0;

	for (int i = 0; i < pixel.channels; i++)
		sum += static_cast<double>(pixel[i]);

	return sum;
}

double get_pixel_hue(const cv::Vec3b& pixel)
{
	double blue = static_cast<double>(pixel[0]);
	double green = static_cast<double>(pixel[1]);
	double red = static_cast<double>(pixel[2]);

	blue /= 255.0;
	green /= 255.0;
	red /= 255.0;

	double max_value = std::max({ red, green, blue });
	double min_value = std::min({ red, green, blue });

	if (max_value == min_value)
		return 0.0;

	double hue;

	if (max_value == red)
		hue = (green - blue) / (max_value - min_value);
	else if (max_value == green)
		hue = 2.0 + (blue - red) / (max_value - min_value);
	else
		hue = 4.0 + (red - green) / (max_value - min_value);

	hue *= 60;

	if (hue < 0)
		hue += 360;

	return hue;
}

double get_pixel_luminance(const cv::Vec3b& pixel)
{
	double blue = static_cast<double>(pixel[0]);
	double green = static_cast<double>(pixel[1]);
	double red = static_cast<double>(pixel[2]);

	blue /= 255.0;
	green /= 255.0;
	red /= 255.0;

	double max_value = std::max({ red, green, blue });
	double min_value = std::min({ red, green, blue });

	return (min_value + max_value) / 2.0;
}

double get_pixel_saturation(const cv::Vec3b& pixel)
{
	double blue = static_cast<double>(pixel[0]);
	double green = static_cast<double>(pixel[1]);
	double red = static_cast<double>(pixel[2]);

	blue /= 255.0;
	green /= 255.0;
	red /= 255.0;

	double max_value = std::max({ red, green, blue });
	double min_value = std::min({ red, green, blue });

	if (max_value == min_value)
		return 0.0;

	double luminance = (min_value + max_value) / 2.0;

	if (luminance <= 0.5)
		return (max_value - min_value) / (max_value + min_value);
	else
		return (max_value - min_value) / (2.0 - max_value - min_value);
}

int median(int a, int b, int c, std::vector<cv::Vec3b>& span, double (*sorting_mode)(const cv::Vec3b&))
{
	std::vector<int> values = { a, b, c };

	double min_value = std::min({ sorting_mode(span[a]), sorting_mode(span[b]), sorting_mode(span[c]) });
	double max_value = std::max({ sorting_mode(span[a]), sorting_mode(span[b]), sorting_mode(span[c]) });
	
	for (int i = 0; i < 3; i++)
	{
		if (sorting_mode(span[values[i]]) != min_value && sorting_mode(span[values[i]]) != max_value)
			return values[i];
	}

	return a;
}

int partition_span(int low, int high, std::vector<cv::Vec3b>& span, double (*sorting_mode)(const cv::Vec3b&))
{
	int i = low;
	int j = high;

	// TODO
	// Do that thing where the pivot can be the beginning, middle, or end of the list, depending on which is the median of three

	//int pivot = low + (high - low) / 2;
	//pivot = median(low, pivot, high, span);
	int pivot = low;

	while (i < j)
	{
		do
			i++;
		while (i < high && sorting_mode(span[i]) <= sorting_mode(span[pivot]));

		do
			j--;
		while (j >= low && sorting_mode(span[j]) > sorting_mode(span[pivot]));


		if (i < j)
			std::swap(span[i], span[j]);
	}

	std::swap(span[j], span[pivot]);
	return j;
}

void sort_span(int low, int high, std::vector<cv::Vec3b>& span, double (*sorting_mode)(const cv::Vec3b&))
{
	if (low < high)
	{
		int pivot = partition_span(low, high, span, sorting_mode);
		sort_span(low, pivot, span, sorting_mode);
		sort_span(pivot + 1, high, span, sorting_mode);
	}
}

void update_image(std::vector<cv::Vec3b*> pixel_addresses, std::vector<cv::Vec3b> span)
{
	for (int i = 0; i < span.size(); i++)
	{
		(*pixel_addresses[i])[0] = span[i][0];
		(*pixel_addresses[i])[1] = span[i][1];
		(*pixel_addresses[i])[2] = span[i][2];
	}
}

std::string remove_trailing_zeroes(std::string s)
{
	int i = static_cast<int>(s.size() - 1);

	while (i >= 0 && s[i] == '0')
	{
		s.pop_back();
		i--;
	}

	if (s.back() == '.')
		s.pop_back();

	return s;
}

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	std::cout << "\nSorting mode (\n1: intensity,\n2: hue,\n3: luminance,\n4: saturation): ";
	int mode;
	std::cin >> mode;

	double (*sorting_mode)(const cv::Vec3b&);

	if (mode == 4)
		sorting_mode = get_pixel_saturation;
	else if (mode == 3)
		sorting_mode = get_pixel_luminance;
	else if (mode == 2)
		sorting_mode = get_pixel_hue;
	else
	{
		sorting_mode = get_pixel_intensity;
		mode = 1;
	}

	std::unordered_map<int, std::pair<int, int>> modes_to_values =
	{
		{1, {0, 765}},
		{2, {0, 360}},
		{3, {0, 1}},
		{4, {0, 1}}
	};

	std::cout << "\nThreshholds should be in ranges: " << modes_to_values[mode].first << "-" << modes_to_values[mode].second << "\nThreshhold low: ";
	double threshhold_low;
	std::cin >> threshhold_low;

	std::cout << "\nThreshhold high: ";
	double threshhold_high;
	std::cin >> threshhold_high;

	std::pair<double, double> threshholds = { threshhold_low, threshhold_high };

	std::cout << "\nSorting direction (\n0: vertical,\n1: horizontal): ";
	int sort_direction;
	std::cin >> sort_direction;

	if (sort_direction != 0 && sort_direction != 1)
		sort_direction = 0;

	std::cout << "\nSpan length (0-100% of image sort direction length): ";
	double span_length_percentage;
	std::cin >> span_length_percentage;

	std::cout << "\nSpan length randomness (0-100): ";
	double span_length_randomness;
	std::cin >> span_length_randomness;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string path;
	std::cout << "\nEnter image path or a number from 1-10 for sample images:" << std::endl;
	std::getline(std::cin, path);

	std::cout << "\nAcquiring image...\n";

	if (path == "1")
		path = "C:/Users/User/Programs/Resources/Star Wars Jedi  Fallen Order Screenshot 2022.06.19 - 18.31.17.34.png";
	else if (path == "2")
		path = "C:/Users/User/Programs/Resources/Digital Combat Simulator  Black Shark Screenshot 2022.09.30 - 23.03.08.59.png";
	else if (path == "3")
		path = "C:/Users/User/Programs/Resources/lenna.jpg";
	else if (path == "4")
		path = "C:/Users/User/Programs/Resources/Forza Horizon 5 Screenshot 2022.02.12 - 15.55.22.69.png";
	else if (path == "5")
		path = "C:/Users/User/Programs/Resources/test2.jpg";
	else if (path == "6")
		path = "C:/Users/User/Programs/Resources/whiplash_sample.png";
	else if (path == "7")
		path = "C:/Users/User/Programs/Resources/Star Wars Jedi  Fallen Order Screenshot 2022.06.19 - 18.56.57.45.png";
	else if (path == "8")
		path = "C:/Users/User/Videos/Digital Combat Simulator  Black Shark/Digital Combat Simulator  Black Shark Screenshot 2023.07.23 - 00.06.18.65.png";

	cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

	double span_length = span_length_percentage / 100 * img.rows;

	std::cout << "Sorting pixels...\n";

	auto start = std::chrono::high_resolution_clock::now();

	int num_img_cols = img.cols;
	int num_img_rows = img.rows;

	int& x_axis_max = sort_direction == 0 ? num_img_cols : num_img_rows;
	int& y_axis_max = sort_direction == 0 ? num_img_rows : num_img_cols;

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> distribution(span_length - span_length_randomness / 100 * y_axis_max >= 0 ? span_length - span_length_randomness / 100 * y_axis_max : 0, span_length + span_length_randomness / 100 * y_axis_max);

	for (int x = 0; x < x_axis_max; x++)
	{
		std::vector<std::thread> threads;
		std::vector<std::vector<cv::Vec3b*>> pixel_addresses = { {} };
		std::vector<std::vector<cv::Vec3b>> pixel_spans = { {} };
		double max_span_length = distribution(generator);

		for (int y = 0; y < y_axis_max; y++)
		{
			cv::Vec3b* pixel_address = &img.at<cv::Vec3b>(&y_axis_max == &num_img_rows ? y : x, &x_axis_max == &num_img_cols ? x : y);
			double threshhold_value = sorting_mode(*pixel_address);

			if (threshhold_value >= threshholds.first && threshhold_value <= threshholds.second && pixel_spans.back().size() < max_span_length && y < y_axis_max - 1)
			{
				pixel_addresses.back().push_back(pixel_address);
				pixel_spans.back().push_back(*pixel_address);
			}
			else if (!pixel_addresses.back().empty() && !pixel_spans.back().empty())
			{
				pixel_spans.push_back({});
				pixel_addresses.push_back({});
			}
		}

		if (pixel_spans.back().empty())
			pixel_spans.pop_back();

		for (int i = 0; i < pixel_spans.size(); i++)
		{
			if (threads.size() < std::thread::hardware_concurrency())
				threads.emplace_back(sort_span, 0, static_cast<int>(pixel_spans[i].size()), std::ref(pixel_spans[i]), sorting_mode);
			else
				sort_span(0, static_cast<int>(pixel_spans[i].size()), pixel_spans[i], sorting_mode);
		}

		for (int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
			update_image(pixel_addresses[i], pixel_spans[i]);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

	std::cout << "Pixels sorted\n";

	std::string file_name;

	for (int i = static_cast<int>(path.size() - 1); path[i] != '/' && path[i] != '\\' && i >= 0; i--)
		file_name += path[i];

	std::reverse(file_name.begin(), file_name.end());

	std::string threshhold_low_s = remove_trailing_zeroes(std::to_string(threshhold_low));
	std::string threshhold_high_s = remove_trailing_zeroes(std::to_string(threshhold_high));
	std::string span_length_percentage_s = remove_trailing_zeroes(std::to_string(span_length_percentage));
	std::string span_length_randomness_s = remove_trailing_zeroes(std::to_string(span_length_randomness));

	cv::imshow("Time: " + duration + "ms", img);
	cv::waitKey(0);
	
	std::cout << "\nSave image? (0/1) ";
	bool save;
	std::cin >> save;

	if (save)
	{
		cv::imwrite("C:/Users/User/pixel_sorting/" + std::to_string(mode) + ", " + threshhold_low_s + ", " + threshhold_high_s + ", " + std::to_string(sort_direction) + ", " + span_length_percentage_s + ", " + span_length_randomness_s + ", " + file_name, img);
		std::cout << "Image saved to C:/Users/User/pixel_sorting\n";
	}
}
