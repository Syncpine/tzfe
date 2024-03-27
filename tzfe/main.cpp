#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <random>
#include <fstream>
#include <windows.h>

using namespace std;


typedef unsigned int ItemType;

const static unsigned int PAGE_SIZE = 4;

const static ItemType WHITE_CELL = 0;


class Score
{
public:
	static Score* GetInstance()
	{
		Score score;
		return &score;
	}

	static void AddScore(unsigned long increment)
	{
		_score += increment;
	}

	static unsigned long GetScore()
	{
		return _score;
	}
private:
	static unsigned long _score;
};

unsigned long Score::_score = 0;


void ShowUI(const vector<vector<ItemType>>& page, const ItemType newItemRow, const ItemType newItemColumn);
void Delay(double time);
vector<int> GetNewItem(const vector<vector<ItemType>>& page);
bool IsFull(const vector<vector<ItemType>>& page);
bool IsNoRepeat(const vector<vector<ItemType>>& page);

vector<ItemType> Transform(vector<ItemType>& nums);

void MoveUp(vector<vector<ItemType>>& page);
void MoveDown(vector<vector<ItemType>>& page);
void MoveLeft(vector<vector<ItemType>>& page);
void MoveRight(vector<vector<ItemType>>& page);


int main()
{
	srand((unsigned int)time(0));

	vector<vector<ItemType>> page;
	for (int i = 0; i < PAGE_SIZE; ++i)
	{
		page.push_back(vector<ItemType>(PAGE_SIZE, WHITE_CELL));
	}

	while (1)
	{
		if (IsFull(page) && IsNoRepeat(page))
		{
			cout << "Can't continue !" << endl << endl;
			break;
		}

		vector<int> location = GetNewItem(page);
		if (-1 == location[0] && -1 == location[1])
		{
			cout << ">> No free location !" << endl << endl;
			break;
		}

		page[location[0]][location[1]] = (ItemType)location[2];

		system("CLS");
		ShowUI(page, location[0], location[1]);

		if (IsFull(page) && IsNoRepeat(page))
		{
			cout << "Can't continue !" << endl << endl;
			break;
		}

		while (1)
		{
			char ch = '\0';
			bool inputAgain = false;
			cin >> ch;

			switch (ch)
			{
			case 'w':
			case 'W':
				MoveUp(page);
				break;
			case 's':
			case 'S':
				MoveDown(page);
				break;
			case 'a':
			case 'A':
				MoveLeft(page);
				break;
			case 'd':
			case 'D':
				MoveRight(page);
				break;
			case 'q':
			case 'Q':
				cout << ">> Game Over." << endl << endl;
				exit(0);
			default:
				inputAgain = true;
				break;
			}

			if (!inputAgain)
			{
				break;
			}
		}

		Delay(0.03);
	}

	return 0;
}

void ShowUI(const vector<vector<ItemType>>& page, const ItemType newItemRow, const ItemType newItemColumn)
{
	int row, column;

	cout << endl;
	for (row = 0; row < PAGE_SIZE; ++row)
	{
		for (column = 0; column < PAGE_SIZE; ++column)
		{
			if (newItemRow == row && newItemColumn == column)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << setw(8) << page[row][column];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
				continue;
			}

			if (WHITE_CELL == page[row][column])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				cout << setw(8) << page[row][column];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
				continue;
			}

			cout << setw(8) << page[row][column];
		}

		cout << endl << endl;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << setw(30) << "Score: " << Score::GetInstance()->GetScore() << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
}

void Delay(double time)
{
	clock_t now = clock();
	while (clock() - now < (unsigned short)(time * 1000));
}

vector<int> GetNewItem(const vector<vector<ItemType>>& page)
{
	vector<vector<int>> free;
	int row, column;

	for (row = 0; row < PAGE_SIZE; ++row)
	{
		for (column = 0; column < PAGE_SIZE; ++column)
		{
			if (WHITE_CELL == page[row][column])
			{
				free.push_back({ row,column });
			}
		}
	}

	unsigned int freeSize = (unsigned int)free.size();

	if (0 == freeSize)
	{
		return { -1, -1, -1 };
	}

	int location = rand() % freeSize;

	int newItem = (rand() % 2 + 1) * 2;

	return { (int)free[location][0], (int)free[location][1], newItem };
}

bool IsFull(const vector<vector<ItemType>>& page)
{
	for (const auto& row : page)
	{
		for (const auto& cell : row)
		{
			if (WHITE_CELL == cell)
			{
				return false;
			}
		}
	}

	return true;
}

bool IsNoRepeat(const vector<vector<ItemType>>& page)
{
	int row, column;
	for (row = 0; row < PAGE_SIZE; ++row)
	{
		for (column = 0; column < PAGE_SIZE; ++column)
		{
			if (row > 0 && page[row][column] == page[row - 1][column])
			{
				return false;
			}

			if (row < PAGE_SIZE - 1 && page[row][column] == page[row + 1][column])
			{
				return false;
			}

			if (column > 0 && page[row][column] == page[row][column - 1])
			{
				return false;
			}

			if (column < PAGE_SIZE - 1 && page[row][column] == page[row][column + 1])
			{
				return false;
			}
		}
	}

	return true;
}

/*
[ 2 2 2 2 ]  -->  [ 4 4 0 0 ]
[ 2 0 2 0 ]  -->  [ 4 0 0 0 ]

*/
vector<ItemType> Transform(vector<ItemType>& nums)
{
	vector<ItemType> noFrees;
	int cur = 0;
	for (int index = 0; index < PAGE_SIZE; ++index)
	{
		noFrees.clear();
		cur = index;
		while (cur < PAGE_SIZE && noFrees.size() < 2)
		{
			if (WHITE_CELL != nums[cur])
			{
				noFrees.push_back(cur);
			}

			++cur;
		}

		int move = 0;
		switch (noFrees.size())
		{
		case 0:
			break;
		case 1:
			move = nums[noFrees[0]];
			nums[noFrees[0]] = WHITE_CELL;
			nums[index] = move;
			break;
		case 2:
			if (nums[noFrees[0]] != nums[noFrees[1]])
			{
				move = nums[noFrees[0]];
				nums[noFrees[0]] = WHITE_CELL;
				nums[index] = move;
			}
			else
			{
				move = nums[noFrees[0]] + nums[noFrees[1]];
				Score::GetInstance()->AddScore(nums[noFrees[1]]);
				nums[noFrees[0]] = WHITE_CELL;
				nums[noFrees[1]] = WHITE_CELL;
				nums[index] = move;
			}
			break;
		}
	}

	return nums;
}

void MoveUp(vector<vector<ItemType>>& page)
{
	int column, index;
	vector<ItemType> nums(PAGE_SIZE, WHITE_CELL);

	for (column = 0; column < PAGE_SIZE; ++column)
	{
		std::fill(nums.begin(), nums.end(), WHITE_CELL);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			nums[index] = page[index][column];
		}

		nums = Transform(nums);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			page[index][column] = nums[index];
		}
	}
}

void MoveDown(vector<vector<ItemType>>& page)
{
	int column, index;
	vector<ItemType> nums(PAGE_SIZE, WHITE_CELL);

	for (column = 0; column < PAGE_SIZE; ++column)
	{
		std::fill(nums.begin(), nums.end(), WHITE_CELL);

		for (index = PAGE_SIZE - 1; index >= 0; --index)
		{
			nums[index] = page[PAGE_SIZE - (int)(index + 1)][column];
		}

		nums = Transform(nums);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			page[PAGE_SIZE - (int)(index + 1)][column] = nums[index];
		}
	}
}

void MoveLeft(vector<vector<ItemType>>& page)
{
	int row, index;
	vector<ItemType> nums(PAGE_SIZE, WHITE_CELL);

	for (row = 0; row < PAGE_SIZE; ++row)
	{
		std::fill(nums.begin(), nums.end(), WHITE_CELL);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			nums[index] = page[row][index];
		}

		nums = Transform(nums);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			page[row][index] = nums[index];
		}
	}
}

void MoveRight(vector<vector<ItemType>>& page)
{
	int row, index;
	vector<ItemType> nums(PAGE_SIZE, WHITE_CELL);

	for (row = 0; row < PAGE_SIZE; ++row)
	{
		std::fill(nums.begin(), nums.end(), WHITE_CELL);

		for (index = PAGE_SIZE - 1; index >= 0; --index)
		{
			nums[index] = page[row][PAGE_SIZE - (int)(index + 1)];
		}

		nums = Transform(nums);

		for (index = 0; index < PAGE_SIZE; ++index)
		{
			page[row][PAGE_SIZE - (int)(index + 1)] = nums[index];
		}
	}
}

