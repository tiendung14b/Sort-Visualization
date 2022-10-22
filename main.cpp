#include <SDL.h>
#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>
#include <chrono>

void genNums();
void draw_array(unsigned int i, unsigned int j, bool oki = false);
void draw_final();
void draw_interchangeSort();
void draw_bubbleSort();
void draw_selectionSort();
void draw_insertionSort();
void render(int i, int j);

SDL_Renderer* renderer = nullptr;
std::vector<int> nums;
int speed = 10;
int q_sort = -1;

void render(int i, int j) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	draw_array(i, j);
	SDL_RenderPresent(renderer);
	SDL_Delay(speed);
}

void genNums() {
	for (int i = 1; i <= 99; i++) {
		nums.push_back(i);
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(nums.begin(), nums.end(), std::default_random_engine(seed));
}

void draw_array(unsigned int i, unsigned int j, bool oki) {
	int index = 0;
	for (int num : nums) {
		if ((index == i && !oki) || (oki && index <= i)) {
			if (oki) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
		}
		else if (index == j) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		}
		else if (index == q_sort) {
			SDL_SetRenderDrawColor(renderer, 100, 220, 100, 255);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_RenderDrawLine(renderer, index, 99, index, num);
		index++;
	}
}

void draw_final() {
	for (int i = 0; i < nums.size(); i++) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw_array(i, i, true);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
}

void draw_interchangeSort() {
	for (unsigned int i = 0; i < nums.size() - 1; i++) {
		for (unsigned int j = i; j < nums.size(); j++) {
			if (nums[j] < nums[i]) {
				std::swap(nums[j], nums[i]);
			}
			// set up screen
			render(i, j);
		}
	}
	draw_final();
}

void draw_bubbleSort() {
	for (int i = 0; i < nums.size() - 1; i++) {
		for (int j = 0; j < nums.size() - i - 1; j++) {
			if (nums[j] > nums[j + 1]) {
				std::swap(nums[j], nums[j + 1]);
			}
			render(j, j + 1);
		}
	}
	draw_final();
}

void draw_selectionSort() {
	for (int i = 0; i < nums.size() - 1; i++) {
		int min = i;
		q_sort = i;
		for (int j = i + 1; j < nums.size(); j++) {
			if (nums[min] > nums[j]) {
				min = j;
			}
			render(min, j);
		}
		std::swap(nums[min], nums[i]);
	}
	draw_final();
	q_sort = -1;
}

void draw_insertionSort() {
	for (int i = 1; i < nums.size(); i++)
	{
		int key = nums[i];
		int j = i - 1;
		while (j >= 0 && nums[j] > key)
		{
			nums[j + 1] = nums[j];
			j = j - 1;
			render(i, j);
		}
		nums[j + 1] = key;
	}
	draw_final();
	q_sort = -1;
}

int partition(int start, int end)
{
	int pivot = nums[start];

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (nums[i] <= pivot)
			count++;
	}

	// Giving pivot element its correct position
	int pivotIndex = start + count;
	std::swap(nums[pivotIndex], nums[start]);

	// Sorting left and right parts of the pivot element
	int i = start, j = end;

	while (i < pivotIndex && j > pivotIndex) {

		while (nums[i] <= pivot) {
			i++;
			render(i, j);
		}

		while (nums[j] > pivot) {
			j--;
			render(i, j);
		}

		if (i < pivotIndex && j > pivotIndex) {
			std::swap(nums[i++], nums[j--]);
		}
	}

	return pivotIndex;
}

void drawQuickSort(int start, int end, int temp = -1)
{
	if (start >= end)
		return;
	int p = partition(start, end);
	temp = p;
	q_sort = temp;
	drawQuickSort(start, p - 1, temp);
	q_sort = temp;
	drawQuickSort(p + 1, end, temp);
	q_sort = -1;
}

int renderSelectionForConsole() {
	system("cls");
	std::cout << "Please select algorithm you want to watch: " << std::endl
		<< "1. Selection sort." << std::endl
		<< "2. Bubble sort." << std::endl
		<< "3. Interchange sort." << std::endl
		<< "4. Insertion sort." << std::endl
		<< "5. Quick sort" << std::endl
		<< "6. Change speed sort." << std::endl
		<< "7. Quit program." << std::endl;
	std::cout << "Select: ";
	int n;
	std::cin >> n;
	switch (n)
	{
	case 1:
		std::cout << "Selection sort algorithm\nRunning.............\n";
		break;
	case 2:
		std::cout << "Bubble sort algorithm\nRunning.............\n";
		break;
	case 3:
		std::cout << "Interchange sort algorithm\nRunning.............\n";
		break;
	case 4:
		std::cout << "Insertion sort algorithm\nRunning.............\n";
		break;
	case 5: 
		std::cout << "Quicksort sort algorithm\nRunning.............\n";
		break;
	case 6:
		std::cout << "Change delay: ";
		int n;
		std::cin >> n;
		if (n > 0) {
			speed = n;
		}
		break;
	default:
		return 7;
	}
	return n;
}

#undef main
int main() {
	SDL_Window* window = nullptr;
	SDL_CreateWindowAndRenderer(100 * 10, 100 * 10, 0, &window, &renderer);
	SDL_RenderSetScale(renderer, 10, 10);

	SDL_Event event;
	while (1) {
		int selector = renderSelectionForConsole();
		nums.clear();
		genNums();
		draw_array(-1, -1);
		if (selector == 1) {
			draw_selectionSort();
			system("pause");
		}
		else if (selector == 2) {
			draw_bubbleSort();
			system("pause");
		}
		else if (selector == 3) {
			draw_interchangeSort();
			system("pause");
		}
		else if (selector == 4) {
			draw_insertionSort();
			system("pause");
		}
		else if (selector == 5) {
			drawQuickSort(0, nums.size() - 1);
			draw_final();
		}
		else if (selector == 7) {
			std::cout << "See you again, mata~~~" << std::endl;
			return 0;
		}
	}

	SDL_Quit();
	system("pause");
	return 0;
}
