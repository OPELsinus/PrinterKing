#include <SFML/Graphics.hpp>
#include <fstream>
#include <thread>
#include <queue>
#include <vector>
using namespace std;
using namespace sf;/*
void loading() {
	if (localPosition.x >= 200 && localPosition.x <= 340 && localPosition.y >= 100 && localPosition.y <= 170 && !isLoaded) {
		isLoaded = true;
		ifstream load("SaveLoad/" + to_string(floorNumber) + ".txt");
		bool offices = false, printers1 = false, stairs1 = false, door1 = false, floor1 = false;
		int num, num1 = 0;
		while (load) {
			string s;
			getline(load, s);
			if (s == "Offices") {
				offices = true;
				continue;
			}

			if (s == "Printers") {
				printers1 = true;
				offices = false;
				continue;
			}

			if (s == "Stairs") {
				stairs1 = true;
				offices = false;
				printers1 = false;
				continue;
			}

			if (s == "Doors") {
				door1 = true;
				stairs1 = false;
				offices = false;
				printers1 = false;
				continue;
			}

			if (s == "Floor") {
				floor1 = true;
				door1 = false;
				stairs1 = false;
				offices = false;
				printers1 = false;
				continue;
			}
			if (offices) {
				string g;
				if (s.length() < 3) {
					for (int i = 0; i < s.length(); i++) {
						if (s[i] <= 32)break;
						g[i] = s[i];
					}
					g = s;
					num = toInt(g);
					num1 = 0;
					officeCount++;
				}
				else if (s.length() > 5) {
					bool secondSpace = false;
					g = s;
					for (int i = 0; i < 15; i++)
						g[i] = 0;

					for (int j = 0; j < s.length(); j++) {
						int point = 0;
						for (int i = 0; i < 15; i++)
							g[i] = 0;
						for (int h = j;; h++, j++) {
							if (s[h] <= 32)break;
							g[point++] = s[h];
						}
						if (!secondSpace) {
							office[officeCount - 1].x[num1] = toInt(g);
							secondSpace = true;
						}
						else {
							office[officeCount - 1].y[num1] = toInt(g);
							break;
						}
					}
					num1++;
				}
				else {
					office[officeCount - 1].number = s;
				}
			}
			if (printers1) {
				string g;
				int num2;
				bool secondSpace = false, thirdSpace = false, fourthSpace = false, fivthSpace = false, sixthSpace = false, seventhSpace = false;

				for (int i = 0; i < s.length(); i++) {
					if (s[i] <= 32) {
						break;
					}
					g[i] = s[i];
				}
				num = toInt(g);
				int len = g.length() + 2;
				g = s;
				for (int i = 0; i < g.length(); i++)
					g[i] = 0;
				for (int j = 0; j < s.length(); j++) {
					for (int i = 0; i < 15; i++)
						g[i] = 0;
					int point = 0;
					for (int i = 0; i < g.length(); i++)
						g[i] = 0;
					for (int h = j;; h++, j++) {
						if (s[h] <= 32)break;
						g[point++] = s[h];
					}
					if (!secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
						printerCount = toInt(g) + 1;
						secondSpace = true;
					}
					else if (secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
						printer[printerCount - 1].x = toInt(g);
						secondSpace = false;
						thirdSpace = true;
					}
					else if (!secondSpace && thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
						printer[printerCount - 1].y = toInt(g);
						thirdSpace = false;
						secondSpace = false;
						fourthSpace = true;
					}
					else if (!secondSpace && !thirdSpace && fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
						printer[printerCount - 1].infoX = toInt(g);
						thirdSpace = false;
						secondSpace = false;
						fourthSpace = false;
						fivthSpace = true;
					}
					else if (!secondSpace && !thirdSpace && !fourthSpace && fivthSpace && !sixthSpace && !seventhSpace) {
						printer[printerCount - 1].infoY = toInt(g);
						thirdSpace = false;
						secondSpace = false;
						fourthSpace = false;
						fivthSpace = false;
						sixthSpace = true;
					}
					else if (!secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && sixthSpace && !seventhSpace) {
						printer[printerCount - 1].image = g;
						thirdSpace = false;
						secondSpace = false;
						fourthSpace = false;
						fivthSpace = false;
						sixthSpace = false;
						seventhSpace = true;
					}
					else if (!secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && seventhSpace) {
						printer[printerCount - 1].name = g;
						for (int h = printer[printerCount - 1].name.length() - 1; h >= 0; h--) {
							if (printer[printerCount - 1].name[h] <= 32)
								printer[printerCount - 1].name.pop_back();
						}
						thirdSpace = false;
						secondSpace = false;
						fourthSpace = false;
						fivthSpace = false;
						sixthSpace = false;
						seventhSpace = true;
						break;
					}
				}
			}
			if (stairs1) {
				string g;
				int num2;
				bool secondSpace = false, thirdSpace = false, fourthSpace = false;

				for (int i = 0; i < s.length(); i++) {
					if (s[i] <= 32) {
						break;
					}
					g[i] = s[i];
				}
				num = toInt(g);
				int len = g.length() + 2;
				g = s;
				for (int i = 0; i < g.length(); i++)
					g[i] = 0;
				for (int j = 0; j < s.length(); j++) {
					for (int i = 0; i < 15; i++)
						g[i] = 0;
					int point = 0;
					for (int i = 0; i < g.length(); i++)
						g[i] = 0;
					for (int h = j;; h++, j++) {
						if (s[h] <= 32)break;
						g[point++] = s[h];
					}
					if (!secondSpace && !thirdSpace && !fourthSpace) {
						stairsCount = toInt(g) + 1;
						secondSpace = true;
					}
					else if (secondSpace && !thirdSpace && !fourthSpace) {
						stairs[stairsCount - 1].x = toInt(g);
						secondSpace = false;
						thirdSpace = true;
					}
					else if (!secondSpace && thirdSpace && !fourthSpace) {
						stairs[stairsCount - 1].y = toInt(g);
						thirdSpace = false;
						fourthSpace = true;
					}
					else if (!secondSpace && !thirdSpace && fourthSpace) {
						stairs[stairsCount - 1].angle = toInt(g);
						thirdSpace = false;
						fourthSpace = true;
					}
				}
			}

			if (door1) {
				string g;
				int num2;
				bool secondSpace = false, thirdSpace = false, fourthSpace = false;

				for (int i = 0; i < s.length(); i++) {
					if (s[i] <= 32) {
						break;
					}
					g[i] = s[i];
					doorCount++;
				}
				num = toInt(g);
				int len = g.length() + 2;
				g = s;
				for (int i = 0; i < g.length(); i++)
					g[i] = 0;
				for (int j = 0; j < s.length(); j++) {
					for (int i = 0; i < 15; i++)
						g[i] = 0;
					int point = 0;
					for (int i = 0; i < g.length(); i++)
						g[i] = 0;
					for (int h = j;; h++, j++) {
						if (s[h] <= 32)break;
						g[point++] = s[h];
					}
					if (!secondSpace && !thirdSpace && !fourthSpace) {
						doorCount = toInt(g) + 1;
						secondSpace = true;
					}
					else if (secondSpace && !thirdSpace && !fourthSpace) {
						door[doorCount - 1].x = toInt(g);
						secondSpace = false;
						thirdSpace = true;
					}
					else if (!secondSpace && thirdSpace && !fourthSpace) {
						door[doorCount - 1].y = toInt(g);
						thirdSpace = false;
						fourthSpace = true;
					}
					else if (!secondSpace && !thirdSpace && fourthSpace) {
						door[doorCount - 1].angle = toInt(g);
						thirdSpace = false;
						fourthSpace = true;
					}
				}
			}
			if (floor1) {
				string g;
				int num2;
				bool secondSpace = false;

				for (int i = 0; i < s.length(); i++) {
					if (s[i] <= 32) {
						break;
					}
					g[i] = s[i];
					floorCount++;
				}
				num = toInt(g);
				int len = g.length() + 2;
				g = s;
				for (int i = 0; i < g.length(); i++)
					g[i] = 0;
				for (int j = len; j < s.length(); j++) {
					for (int i = 0; i < 15; i++)
						g[i] = 0;
					int point = 0;
					for (int i = 0; i < g.length(); i++)
						g[i] = 0;
					for (int h = j;; h++, j++) {
						if (s[h] <= 32)break;
						g[point++] = s[h];
					}
					if (!secondSpace) {
						floorX[floorCount - 1] = toInt(g);
						secondSpace = true;
					}
					else {
						floorY[floorCount - 1] = toInt(g);
						break;
					}
				}
			}

		}
		load.close();

		for (int i = 0; i < officeCount; i++) {
			int minX = 2000, minY = 2000, maxX = -1, maxY = -1;
			for (int j = 0; j < 8; j++) {
				if (office[i].x[j] < minX)minX = office[i].x[j];
				if (office[i].y[j] < minY)minY = office[i].y[j];
				if (office[i].x[j] > maxX)maxX = office[i].x[j];
				if (office[i].y[j] > maxY)maxY = office[i].y[j];
			}
			office[i].left[0] = minX;
			office[i].left[1] = minY;
			office[i].right[0] = maxX;
			office[i].right[1] = maxY;
		}

	}
}*/