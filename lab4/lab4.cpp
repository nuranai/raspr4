// lab4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <fstream>
#include <mutex>
#include <utility>
#include <algorithm>
#include <vector>
#include <functional>


using namespace std;

vector<pair<char, int>> sortedVector;

bool cmp(pair<char, int>& a,
        pair<char, int>& b)
{
    return a.second > b.second;
}
void sort(map<char, int>& M, function<void(vector<pair<char, int>> &vector)> callback)
{
 
	if (sortedVector.size() > 0) { 
		callback(sortedVector);
		return;
	}

    for (auto& it : M) {
        sortedVector.push_back(it);
    }
 
    sort(sortedVector.begin(), sortedVector.end(), cmp);
 
	callback(sortedVector);
}

int main()
{
	map<char, int> mainMap;
	string texts[2] = { "text1.txt", "text2.txt" };
	thread threadArr[2];
	mutex mapMutex;

	for (int i = 0; i < 2; i++) {
		threadArr[i] = thread([text = texts[i], &mapMutex, &mainMap]() mutable {
			map<char, int> threadMap;
		ifstream file(text);
		char curr;
		while (file.get(curr)) {
			threadMap[curr]++;
		}
		unique_lock<mutex> lock(mapMutex);
		for (const auto& pair : threadMap) {
			mainMap[pair.first] += pair.second;
		}
			});
	}
	for (int i = 0; i < 2; i++) {
		threadArr[i].join();
	}

	int total = 0;
	for (auto pair : mainMap) {
		total += pair.second;
	}

	cout << "write commands, type h for list of commands:" << endl;
	bool flag = true;
	while (flag) {
		char command;
		cout << "> ";
		cin >> command;
		switch (command)
		{
		case 'h':
			cout << "h - print this list of commands" << endl;
			cout << "c - find character" << endl;
			cout << "f - find 5 most frequent" << endl;
			cout << "r - find 4 most rare" << endl;
			cout << "x - exit program" << endl;
			break;
		case 'x':
			flag = false;
			break;
		case 'c':
			cout << "enter character: ";
			char toFind;
			cin >> toFind;
			cout << endl << "probability:: " << toFind << " = " << double(mainMap[toFind]) / total << endl;
			break;
		case 'f':
			sort(mainMap, [](vector<pair<char, int>>&vector) {
				for (int i = 0; i < 5; i++) {
					cout << vector[i].first << " - " << vector[i].second << endl;
				}
				});
			break;
		case 'r':
			sort(mainMap, [](vector<pair<char, int>>& vector) {
				for (int i = vector.size() - 4; i < vector.size(); i++) {
					cout << vector[i].first << " - " << vector[i].second << endl;
				}
				});
			break;
		default:
			cout << "unknown command" << endl;
			break;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
