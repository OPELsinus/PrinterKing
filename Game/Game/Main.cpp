#pragma warning(disable : 4996)
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <thread>
#include <queue>
#include <vector>
#include "KeyboardInput.cpp"

using namespace std;
using namespace sf;

int toInt(string s) {
	int a = 0, y = 1;
	for (int i = s.length() - 1; i >= 0; i--) {
		if (s[i] <= 32)continue;
		a += int(s[i] - 48) * y;
		y *= 10;
	}
	return a;
}

void take_screenshot(const sf::RenderWindow& window, const std::string& filename)
{
	sf::Texture texture;
	texture.create(window.getSize().x, window.getSize().y);
	texture.update(window);
	if (texture.copyToImage().saveToFile(filename))
	{
		std::cout << "Screenshot saved to " << filename << std::endl;
	}
	else
	{
		// Error handling
	}
}

struct Office {
	int x[10];
	int y[10];
	int height;
	int width;
	int left[2];
	int right[2];
	int angle = 0;
	string color = "                       ";
	string number = "                                                                                ";
	bool used_printer_color = 0;
	bool used_printer_black = 1;
};

struct Door {
	int x;
	int y;
	int leftX;
	int leftY;
	int rightX;
	int rightY;
	int width = 27;
	int height = 56;
	int angle = 0;
};

struct Floor {
	int x[300];
	int y[300];
};

struct Printer {
	int x;
	int y;
	int infoX;
	int infoY;
	int id;
	int color;
	int printedPages = 0;
	int expectedPages = 0;
	string name;
	string image;
};

struct Stairs {
	int x;
	int y;
	int leftX;
	int leftY;
	int rightX;
	int rightY;
	int width = 59;
	int height = 210;
	int angle = 270;
};


struct Pos {
	int i;
	int j;
	Pos(const int i, const int j) : i(i), j(j) {}
};

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "russian");

	Vector2i blabla = Mouse::getPosition();
	Mouse::setPosition(Vector2i(0xFFFF, 0xFFFF));
	Vector2i blabla1 = Mouse::getPosition();
	Mouse::setPosition(Vector2i(blabla.x, blabla.y));


	RenderWindow window(sf::VideoMode(blabla1.x, blabla1.y), "");
	View view(FloatRect(0, 0, blabla1.x, blabla1.y));
	window.setView(view);
	Mouse::setPosition(Vector2i(10, 50), window);
	Time t1 = milliseconds(230);
	Time t2 = milliseconds(80);
	Time timeForPrinting = milliseconds(150);

	RectangleShape mode(Vector2f(140, 50));

	VertexArray room(LineStrip, 8);
	VertexArray floorD(LineStrip, 300);
	VertexArray designBar(LineStrip, 4);

	Text textMode;
	Font font;
	font.loadFromFile("arial.ttf");
	textMode.setFont(font);

	int stHeight = 100, stWidth = 80, officeCount = 1, angle = 90, coordsX[10], coordsY[10], floorCount = 0, printerCount = 1, printerInfo = 1, stairsCount = 1, doorCount = 1;
	int floorNumberX[5], floorNumberY[5], floorNumber = 0;
	vector <int> floorX(300);
	vector <int> floorY(300);
	int activeOffice = 0, activeFloor = 0, activePrinter = 0, editing1 = 0, editing2 = 0, activeStairs = 0, stairsEdit1 = 0, stairsEdit2 = 0, activeDoor = 0;
	bool found = false, floorPoint = false, edit1 = false, edit2 = false, foundPrinter = false, foundPrInfo = false, foundStairs = false, foundDoor = false;
	bool printerInfoEditingName = false, printerInfoEditingId = false, printerInfoEditingColor = false, printerInfoEditingExpPages = false, printerInfoEditingPrintPages = false, printerInfoEditingImage = false;

	vector<vector<char>> ch(1000, vector<char>(1600, '+'));
	vector<vector<vector<int>>> number(30, vector<vector<int>>(1000, vector<int>(1600, -1)));
	queue<Pos> q;
	int start[40][2];
	bool bol = false;
	int optCount = 1;
	int x_opt[30] = { 0 }, y_opt[30] = { 0 };

	//Office* office = new Office[1000];
	vector <Office> office(100);
	vector <Floor> floor(1);
	vector <Printer> printer(20);
	vector <Door> door(20);
	vector <Stairs> stairs(20);

	stairs[0].x = 750;
	stairs[0].y = 80;
	stairs[0].leftX = 750;
	stairs[0].leftY = 80;
	stairs[0].rightX = stairs[0].x + stairs[0].height;
	stairs[0].rightY = stairs[0].y;
	stairs[0].angle = 270;

	door[0].x = 700;
	door[0].y = 64;
	door[0].leftX = 700;
	door[0].leftY = 64;
	door[0].rightX = door[0].x + door[0].width;
	door[0].rightY = door[0].y + door[0].height;

	printer[0].x = 700;
	printer[0].y = 20;
	Texture texturePrinter;
	texturePrinter.loadFromFile("images//Printer.png", IntRect(0, 0, 29, 25));
	Sprite spritePrint;
	spritePrint.setTexture(texturePrinter);
	spritePrint.setPosition(printer[0].x, printer[0].y);

	coordsX[0] = 600;
	coordsY[0] = 20;
	coordsX[1] = coordsX[0] + stWidth;
	coordsY[1] = coordsY[0];
	coordsX[2] = coordsX[1];
	coordsY[2] = coordsY[0] + stHeight;
	coordsX[3] = (coordsX[0] + coordsX[1]) / 2 + stWidth / 4;
	coordsY[3] = coordsY[2];

	coordsX[4] = (coordsX[0] + coordsX[1]) / 2 + stWidth / 4;
	coordsY[4] = coordsY[2] - 20;
	coordsX[5] = (coordsX[0] + coordsX[1]) / 2 + stWidth / 4 - 20;
	coordsY[5] = coordsY[2];
	coordsX[6] = coordsX[0];
	coordsY[6] = coordsY[0] + stHeight;
	coordsX[7] = coordsX[0];
	coordsY[7] = coordsY[0];


	for (int i = 0; i < 8; i++) {
		office[0].x[i] = coordsX[i];
		office[0].y[i] = coordsY[i];
	}
	office[0].height = stHeight;
	office[0].width = stWidth;
	office[0].left[0] = office[0].x[0];
	office[0].left[1] = office[0].y[0];
	office[0].right[0] = office[0].x[2];
	office[0].right[1] = office[0].y[2];


	floorNumberX[0] = blabla1.x - 300;
	floorNumberY[0] = 80;

	for (int i = 1; i < 5; i++) {
		floorNumberX[i] = floorNumberX[i - 1] + 50;
		floorNumberY[i] = floorNumberY[0];
	}


	int mouseX, mouseY, deletion = 1, a = 0;
	bool draw = false, magnetism = true, isLoaded = false;


	while (window.isOpen()) {
		Vector2i localPosition = Mouse::getPosition(window);
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			if (draw)draw = false;
			else window.close();
			for (int i = 1; i < officeCount; i++) {
				office.erase(office.begin() + i);
				officeCount--;
			}
		}
		mode.setPosition(0, 0);
		mode.setFillColor(Color::White);
		mode.setOutlineColor(Color::White);
		mode.setSize(Vector2f(2000, 1200));
		window.draw(mode);

		if (draw) {

			for (int i = 1; i < officeCount; i++) {
				if (abs(localPosition.x - office[i].x[0]) <= 15 && abs(localPosition.y - office[i].y[0]) <= 15) {
					localPosition.x = office[i].x[0];
					localPosition.y = office[i].y[0];
				}

				if (abs(localPosition.x - office[i].x[1]) <= 15 && abs(localPosition.y - office[i].y[1]) <= 15) {
					localPosition.x = office[i].x[1];
					localPosition.y = office[i].y[1];
				}

				if (abs(localPosition.x - office[i].x[2]) <= 15 && abs(localPosition.y - office[i].y[2]) <= 15) {
					localPosition.x = office[i].x[2];
					localPosition.y = office[i].y[2];
				}

				if (abs(localPosition.x - office[i].x[6]) <= 15 && abs(localPosition.y - office[i].y[6]) <= 15) {
					localPosition.x = office[i].x[6];
					localPosition.y = office[i].y[6];
				}
			}

			for (int i = 0; i < floorCount; i++) {
				if (abs(localPosition.x - floorX[i]) <= 15)
					localPosition.x = floorX[i];

				if (abs(localPosition.y - floorY[i]) <= 15)
					localPosition.y = floorY[i];
			}
		}
		for (int i = 1; i < officeCount; i++) {
			office[i].x[7] = office[i].x[0];
			office[i].y[7] = office[i].y[0];
		}

		/// Дверь офиса
		/*for (int i = 0; i < officeCount; i++) {
			if (office[i].angle == 0 || office[i].angle == 180 || office[i].angle == 360) {
				office[i].x[3] = abs((office[i].x[2] - office[i].width / 4));
				office[i].y[3] = abs((office[i].y[2]));

				office[i].x[4] = office[i].x[3];
				office[i].y[4] = abs((office[i].y[2] - office[i].height / 5));

				office[i].x[5] = abs((office[i].x[3] - office[i].width / 4));
				office[i].y[5] = office[i].y[3];
			}

			else {
				office[i].x[3] = office[i].x[2];
				office[i].y[3] = abs((office[i].y[2] - office[i].width / 4));

				office[i].x[4] = abs((office[i].x[2] + office[i].width / 4));
				office[i].y[4] = office[i].y[3];

				office[i].x[5] = office[i].x[3];
				office[i].y[5] = abs((office[i].y[3] - office[i].width / 4));
			}
		}*/
		
		if (Mouse::isButtonPressed(Mouse::Left)) {

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			/// Очистка
			if (localPosition.x >= 380 && localPosition.x <= 520 && localPosition.y >= 100 && localPosition.y <= 150) {
				for (int i = 1; i < officeCount; i++)
					office.erase(office.begin() + i);
				for (int i = 1; i < printerCount; i++)
					printer.erase(printer.begin() + i);

				/// Проверить объект floor на баги
				for (int i = 0; i < floorCount; i++) {
					floorX.pop_back();
					floorY.pop_back();
				}
				for (int i = 1; i < doorCount; i++)
					door.erase(door.begin() + i);
				for (int i = 1; i < stairsCount; i++)
					stairs.erase(stairs.begin() + i);
				doorCount = officeCount = printerCount = stairsCount = floorCount = 1;
				cout << "Clearing\n";
				sleep(timeForPrinting);
			}

			///////////////////////
			/// Работа с принтерами
			printerInfoEditingName = printerInfoEditingId = printerInfoEditingColor = printerInfoEditingExpPages = printerInfoEditingPrintPages = printerInfoEditingImage = false;
			if (foundPrinter && activePrinter > 0) {
				if (localPosition.x >= 20 && localPosition.x <= 370) {
					if (localPosition.y >= 220 && localPosition.y <= 250) {
						printerInfoEditingName = true;
						goto A;
					}
					if (localPosition.y > 250 && localPosition.y <= 280) {
						printerInfoEditingId = true;
						goto A;
					}
					if (localPosition.y > 280 && localPosition.y <= 310) {
						printerInfoEditingColor = true;
						goto A;
					}
					if (localPosition.y > 310 && localPosition.y <= 340) {
						printerInfoEditingExpPages = true;
						goto A;
					}
					if (localPosition.y > 340 && localPosition.y <= 370) {
						printerInfoEditingPrintPages = true;
						goto A;
					}
					if (localPosition.y > 370 && localPosition.y <= 400) {
						printerInfoEditingImage = true;
						goto A;
					}
				}

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Выбор точки этажа
			if (activeFloor >= 0 && !draw) {
				floorX[activeFloor] += localPosition.x - mouseX;
				floorY[activeFloor] += localPosition.y - mouseY;
				if (magnetism) {
					for (int i = 1; i < officeCount; i++) {
						for (int j = 0; j < 7; j++) {
							if (j > 2 && j < 6)continue;
							if (abs(floorX[activeFloor] - office[i].x[j]) <= 10 && abs(floorY[activeFloor] - office[i].y[j]) <= 10) {
								floorX[activeFloor] = office[i].x[j];
								floorY[activeFloor] = office[i].y[j];
							}
						}
					}
				}
				
			}

			floorPoint = false;
			for (int i = 0; i < floorCount; i++) {
				if (abs(localPosition.x - floorX[i]) <= 25 && abs(localPosition.y - floorY[i]) <= 25 && !draw && activeOffice == -1 && activePrinter == -1 && activeDoor == -1 && activeStairs == -1 && activeOffice == -1 && activePrinter == -1) {
					floorPoint = true;
					activeDoor = -1;
					activeOffice = -1;
					activePrinter = -1;
					activeStairs = -1;
					activeFloor = i;
					break;
				}
			}
			if (!floorPoint)activeFloor = -1;
			else goto A;

			/// Если выбран рисунок принтера
			foundPrInfo = false;
			for (int i = printerCount; i >= 0; i--) {
				if (localPosition.x >= printer[i].infoX - 5 && localPosition.x <= printer[i].infoX + 53 && localPosition.y >= printer[i].infoY - 5 && localPosition.y <= printer[i].infoY + 94) {
					foundPrInfo = true;
					printerInfo = i;
					break;
				}
			}
			if (foundPrInfo) {
				printer[printerInfo].infoX += localPosition.x - mouseX;
				printer[printerInfo].infoY += localPosition.y - mouseY;
				goto A;
			}
			////////////

			for (int i = printerCount; i >= 0; i--) {

				activePrinter = -1;
				if (localPosition.x >= printer[i].x - 5 && localPosition.x <= printer[i].x + 34 && localPosition.y >= printer[i].y - 5 && localPosition.y <= printer[i].y + 29 && activeDoor == -1 && activeOffice == -1 && activeStairs == -1 && activeFloor == -1) {

					draw = false;
					bool near = false;

					for (int j = 1; j < printerCount; j++) {
						if (abs((printer[j].x + printer[j].x + 29) / 2 - (printer[0].x + printer[0].x + 29) / 2) <= 30 && abs((printer[j].y + printer[j].y + 25) / 2 - (printer[0].y + printer[0].y + 25) / 2) <= 30) {
							near = true;
							break;
						}
					}

					/// Создание принтера
					/// 

					if (i == 0 && !near && printerCount > 1) {
						printer[printerCount].x = printer[0].x;
						printer[printerCount].y = printer[0].y;
						printer[printerCount].infoX = printer[0].x;
						printer[printerCount].infoY = printer[0].y + 150;
						printer[printerCount].id = printerCount;
						printer[printerCount].name = "HP LJ MFP 725";
						printer[printerCount].image = "images/printers/HP-LJ-MFP-725.png";

						printerCount++;
						activePrinter = printerCount - 1;
					}
					foundPrinter = true;

					if (i == 0 && printerCount == 1) {
						printer[printerCount].x = printer[0].x;
						printer[printerCount].y = printer[0].y;
						printer[printerCount].infoX = printer[0].x;
						printer[printerCount].infoY = printer[0].y + 150;
						printer[printerCount].id = printerCount;
						printer[printerCount].name = "HP LJ MFP 725";
						printer[printerCount].image = "images/printers/HP-LJ-MFP-725.png";

						printerCount++;
						foundPrinter = true;
					}
					activePrinter = i;

					if (activePrinter > 0) {
						printer[activePrinter].x += localPosition.x - mouseX;
						printer[activePrinter].y += localPosition.y - mouseY;
					}
					foundDoor = false;
					activeDoor = -1;
					goto A;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////




			/// Работа с офисами
			/// 
			/// Обновление значений офисов
			if (activeOffice >= 0)
				for (int i = 0; i < officeCount; i++) {
					int minX = 2000, minY = 2000, maxX = 0, maxY = 0;
					for (int j = 0; j < 8; j++) {
						if (office[activeOffice].x[j] < minX)minX = office[activeOffice].x[j];
						if (office[activeOffice].y[j] < minY)minY = office[activeOffice].y[j];
						if (office[activeOffice].x[j] > maxX)maxX = office[activeOffice].x[j];
						if (office[activeOffice].y[j] > maxY)maxY = office[activeOffice].y[j];
					}
					office[activeOffice].left[0] = minX;
					office[activeOffice].left[1] = minY;
					office[activeOffice].right[0] = maxX;
					office[activeOffice].right[1] = maxY;
				}
			///

			bool empty = true;
			for (int i = 0; i < officeCount; i++) {

				if (localPosition.x >= office[i].left[0] - 10 && localPosition.x <= office[i].right[0] + 10 && localPosition.y >= office[i].left[1] - 10 && localPosition.y <= office[i].right[1] + 10 && activePrinter == -1 && !draw && activeDoor == -1 && activePrinter == -1 && activeStairs == -1 && activeFloor == -1) {

					empty = false;
					draw = false;
					bool near = false;

					for (int j = 1; j < officeCount; j++) {
						if (abs((office[j].left[0] + office[j].right[0]) / 2 - (office[0].left[0] + office[0].right[0]) / 2) <= 100 && abs((office[j].left[1] + office[j].right[1]) / 2 - (office[0].left[1] + office[0].right[1]) / 2) <= 100) {
							near = true;
							break;
						}
					}

					/// Создание офиса
					/// 
					if (i == 0 && !near && officeCount > 1) {
						for (int i = 0; i < 8; i++) {
							office[officeCount].x[i] = coordsX[i];
							office[officeCount].y[i] = coordsY[i];
						}
						office[officeCount].height = stHeight;
						office[officeCount].width = stWidth;
						office[officeCount].left[0] = office[officeCount].x[0];
						office[officeCount].left[1] = office[officeCount].y[0];
						office[officeCount].right[0] = office[officeCount].x[2];
						office[officeCount].right[1] = office[officeCount].y[2];
						office[officeCount].number = to_string(officeCount + 100);

						officeCount++;
						activeOffice = officeCount - 1;
					}
					found = true;

					if (i == 0 && officeCount == 1 && !draw) {
						for (int i = 0; i < 8; i++) {
							office[officeCount].x[i] = coordsX[i];
							office[officeCount].y[i] = coordsY[i];
						}
						office[officeCount].height = stHeight;
						office[officeCount].width = stWidth;
						office[officeCount].left[0] = office[officeCount].x[0];
						office[officeCount].left[1] = office[officeCount].y[0];
						office[officeCount].right[0] = office[officeCount].x[2];
						office[officeCount].right[1] = office[officeCount].y[2];
						office[officeCount].number = "101";

						officeCount++;
						activeOffice++;
						found = true;
					}
					activeOffice = i;
				}
			}
			if (empty) activeOffice = -1;

			/// Передвижение офиса
			/// 
			if (activeOffice > 0 && !edit1 && !edit2 && activePrinter == -1 && !draw)
				for (int i = 0; i < 8; i++) {
					office[activeOffice].x[i] += localPosition.x - mouseX;
					office[activeOffice].y[i] += localPosition.y - mouseY;

					int minX = 2000, minY = 2000, maxX = 0, maxY = 0;
					for (int j = 0; j < 8; j++) {
						if (office[activeOffice].x[j] < minX)minX = office[activeOffice].x[j];
						if (office[activeOffice].y[j] < minY)minY = office[activeOffice].y[j];
						if (office[activeOffice].x[j] > maxX)maxX = office[activeOffice].x[j];
						if (office[activeOffice].y[j] > maxY)maxY = office[activeOffice].y[j];
					}
					office[activeOffice].left[0] = minX;
					office[activeOffice].left[1] = minY;
					office[activeOffice].right[0] = maxX;
					office[activeOffice].right[1] = maxY;
				}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			/// FLOOR

			bool drawCh = draw;
			if (localPosition.x >= 20 && localPosition.x <= 160 && localPosition.y >= 20 && localPosition.y <= 70) {
				if (!draw)draw = true;
				else draw = false;
				sf::sleep(t2 + t2);
			}
			if (drawCh && draw) {
				floor[0].x[floorCount] = localPosition.x;
				floor[0].y[floorCount] = localPosition.y;
				floorX[floorCount] = localPosition.x;
				floorY[floorCount] = localPosition.y;
				floorCount++;
				sf::sleep(t2 + t2);
			}

			//poisk

			int minx = 2000, miny = 1500;
			int maxx = 0, maxy = 0;

			//Poisk
			if (localPosition.x >= floorNumberX[0] + 30 && localPosition.x <= floorNumberX[0] + 288 && localPosition.y >= floorNumberY[0] - 50 && localPosition.y <= floorNumberY[0]) {

				for (int k = 1; k < officeCount; k++) {
					int ran_print = rand() % 2;
					office[k].used_printer_color = (ran_print == 1) ? 1 : 0;
				}
				for (int k = 0; k < floorCount - 1; k++) {
					if (minx > floorX[k]) {
						if (miny > floorY[k]) {
							minx = floorX[k];
							miny = floorY[k];
						}
					}
					if (maxx < floorX[k]) {
						if (maxy < floorY[k]) {
							maxx = floorX[k];
							maxy = floorY[k];
						}
					}
					if (floorX[k] == floorX[k + 1]) {
						if (floorY[k] > floorY[k + 1]) {
							for (int y = floorY[k]; y >= floorY[k + 1]; y--) {
								ch[y - 200][floorX[k] - 400] = 'X';
							}
						}
						else if (floorY[k] < floorY[k + 1]) {
							for (int y = floorY[k]; y <= floorY[k + 1]; y++) {
								ch[y - 200][floorX[k] - 400] = 'X';
							}
						}

						for (int i = 1; i < doorCount; i++) {//for the vistavlenie pod dveri
							if (door[i].angle == 0) {
								if (abs(door[i].rightX - floorX[k]) < 10 && ((door[i].rightY >= floorY[k] && door[i].rightY <= floorY[k + 1]) || (door[i].rightY <= floorY[k] && door[i].rightY >= floorY[k + 1]))) {
									ch[door[i].rightX - door[i].height / 2 - 200][floorX[k] - 400] = '+';
								}
							}
							else if (door[i].angle == 180) {
								if (abs(door[i].leftX - floorX[k]) < 10 && ((door[i].leftY >= floorY[k] && door[i].leftY <= floorY[k + 1]) || (door[i].leftY <= floorY[k] && door[i].leftY >= floorY[k + 1]))) {
									ch[door[i].leftX + door[i].height / 2 - 200][floorX[k] - 400] = '+';
								}
							}
						}
					}
					else if (floorY[k] == floorY[k + 1]) {
						if (floorX[k] > floorX[k + 1]) {
							for (int x = floorX[k]; x >= floorX[k + 1]; x--) {
								ch[floorY[k] - 200][x - 400] = 'X';
							}
						}
						else if (floorX[k] < floorX[k + 1]) {
							for (int x = floorX[k]; x <= floorX[k + 1]; x++) {
								ch[floorY[k] - 200][x - 400] = 'X';
							}
						}

						for (int i = 1; i < doorCount; i++) {//for the vistavlenie pod dveri
							if (door[i].angle == 90) {
								if (abs(door[i].rightY - floorY[k]) < 10 && ((door[i].rightY >= floorX[k] && door[i].rightY <= floorX[k + 1]) || (door[i].rightY <= floorX[k] && door[i].rightY >= floorX[k + 1]))) {
									ch[floorY[k] - 200][door[i].rightY - door[i].width / 2 - 400] = '+';
								}
							}
							else if (door[i].angle == 270) {
								if (abs(door[i].leftY - floorY[k]) < 10 && ((door[i].leftY >= floorX[k] && door[i].leftY <= floorX[k + 1]) || (door[i].leftY <= floorX[k] && door[i].leftY >= floorX[k + 1]))) {
									ch[floorY[k] - 200][door[i].leftY + door[i].width / 2 - 400] = '+';
								}
							}
						}
					}
				}
				for (int k = 1; k < officeCount; k++) {
					if (office[k].angle == 0 || office[k].angle == 360) {
						for (int j = office[k].x[0]; j < office[k].x[1]; j++) {
							ch[office[k].y[0] - 200][j - 400] = 'X';
						}
						for (int j = office[k].x[2]; j >= office[k].x[6]; j--) {
							ch[office[k].y[2] - 200][j - 400] = 'X';
						}
						for (int i = office[k].y[1]; i < office[k].y[2]; i++) {
							ch[i - 200][office[k].x[1] - 400] = 'X';
						}
						for (int i = office[k].y[6]; i >= office[k].y[0]; i--) {
							ch[i - 200][office[k].x[6] - 400] = 'X';
						}
						ch[office[k].y[3] - 200][office[k].x[3] - 400] = '+';
						ch[office[k].y[3] - 200 - 1][office[k].x[3] - 400] = 'X';
						number[k][office[k].y[3] - 200][office[k].x[3] - 400] = 0;
						start[k][0] = office[k].y[3] - 200;
						start[k][1] = office[k].x[3] - 400;
					}
					else if (office[k].angle == 90) {
						for (int j = office[k].x[6]; j < office[k].x[0]; j++) {
							ch[office[k].y[0] - 200][j - 400] = 'X';
						}
						for (int j = office[k].x[1]; j >= office[k].x[2]; j--) {
							ch[office[k].y[2] - 200][j - 400] = 'X';
						}
						for (int i = office[k].y[0]; i < office[k].y[1]; i++) {
							ch[i - 200][office[k].x[1] - 400] = 'X';
						}
						for (int i = office[k].y[2]; i >= office[k].y[6]; i--) {
							ch[i - 200][office[k].x[2] - 400] = 'X';
						}
						ch[office[k].y[3] - 200][office[k].x[3] - 400] = '+';
						ch[office[k].y[3] - 200][office[k].x[3] - 400 + 1] = 'X';
						number[k][office[k].y[3] - 200][office[k].x[3] - 400] = 0;
						start[k][0] = office[k].y[3] - 200;
						start[k][1] = office[k].x[3] - 400;
					}
					else if (office[k].angle == 180) {
						for (int j = office[k].x[2]; j < office[k].x[6]; j++) {
							ch[office[k].y[2] - 200][j - 400] = 'X';
						}
						for (int j = office[k].x[0]; j >= office[k].x[1]; j--) {
							ch[office[k].y[0] - 200][j - 400] = 'X';
						}
						for (int i = office[k].y[6]; i < office[k].y[0]; i++) {
							ch[i - 200][office[k].x[6] - 400] = 'X';
						}
						for (int i = office[k].y[1]; i >= office[k].y[2]; i--) {
							ch[i - 200][office[k].x[1] - 400] = 'X';
						}
						ch[office[k].y[3] - 200][office[k].x[3] - 400] = '+';
						ch[office[k].y[3] - 200 + 1][office[k].x[3] - 400] = 'X';
						number[k][office[k].y[3] - 200][office[k].x[3] - 400] = 0;
						start[k][0] = office[k].y[3] - 200;
						start[k][1] = office[k].x[3] - 400;
					}
					else if (office[k].angle == 270) {
						for (int j = office[k].x[1]; j < office[k].x[2]; j++) {
							ch[office[k].y[1] - 200][j - 400] = 'X';
						}
						for (int j = office[k].x[6]; j >= office[k].x[0]; j--) {
							ch[office[k].y[6] - 200][j - 400] = 'X';
						}
						for (int i = office[k].y[2]; i < office[k].y[6]; i++) {
							ch[i - 200][office[k].x[2] - 400] = 'X';
						}
						for (int i = office[k].y[0]; i >= office[k].y[1]; i--) {
							ch[i - 200][office[k].x[0] - 400] = 'X';
						}
						ch[office[k].y[3] - 200][office[k].x[3] - 400] = '+';
						ch[office[k].y[3] - 200][office[k].x[3] - 400 - 1] = 'X';
						number[k][office[k].y[3] - 200][office[k].x[3] - 400] = 0;
						start[k][0] = office[k].y[3] - 200;
						start[k][1] = office[k].x[3] - 400;
					}
				}
				int nu = 0;
				for (int k = 1; k < officeCount; k++) {
					q.push(Pos(start[k][0], start[k][1]));
					while (!q.empty()) {
						Pos cur = q.front();
						q.pop();
						for (int di = -1; di <= 1; di++) {
							for (int dj = -1; dj <= 1; dj++) {
								if (di * di + dj * dj == 1) {
									int ni = di + cur.i;
									int nj = dj + cur.j;
									if (ch[ni][nj] == '+' && number[k][ni][nj] == -1) {
										number[k][ni][nj] = number[k][cur.i][cur.j] + 1;
										q.push(Pos(ni, nj));
										nu++;
									}
								}
							}
						}
					}
				}
				int min_x = 2000, min_y = 1500;
				int max_x = 0, max_y = 0;
				for (int t = 1; t < officeCount; t++) {
					cout << start[t][0] << " " << start[t][1] << endl;
					if (start[t][0] < min_y) {
						min_y = start[t][0];
						//cout << "min_y " << min_y << endl;
					}
					if (start[t][1] < min_x) {
						min_x = start[t][1];
						//cout << "min_x" << min_x << endl;
					}
					if (start[t][0] > max_y) {
						max_y = start[t][0];
						//cout << "max_y" << max_y << endl;
					}
					if (start[t][1] > max_x) {
						max_x = start[t][1];
						//cout << "max_x" << max_x << endl;
					}
				}
				int max_len = 0, min_len = 100000;
				for (int k = 1; k < printerCount; k++) {

					for (int i = miny; i < maxy; i++) {
						for (int j = minx; j < maxx; j++) {
							if (number[1][i - 200][j - 400] != -1) {
								max_len = 0;
								for (int t = 1; t < officeCount; t++) {
									if (k == 2) {
										if (number[t][i - 200][j - 400] > max_len && office[t].used_printer_color) {
											max_len = number[t][i - 200][j - 400];
										}
									}
									else if (k == 1) {
										if (number[t][i - 200][j - 400] > max_len) {
											max_len = number[t][i - 200][j - 400];
										}
									}
									else {
										int random = rand() % officeCount + 1;
										if (office[random].angle == 0 || office[random].angle == 360) {
											x_opt[optCount] = office[random].x[4];
											y_opt[optCount] = office[random].y[4];
										}
										else if (office[random].angle == 90) {
											x_opt[optCount] = office[random].x[3];
											y_opt[optCount] = office[random].y[3];
										}
										else if (office[random].angle == 180) {
											x_opt[optCount] = office[random].x[2];
											y_opt[optCount] = office[random].y[2];
										}
										else if (office[random].angle == 270) {
											x_opt[optCount] = office[random].x[2] - 20;
											y_opt[optCount] = office[random].y[2];
										}
									}
								}
								if (min_len > max_len && k < 3) {
									min_len = max_len;
									x_opt[optCount] = j;
									y_opt[optCount] = i;
								}
							}

						}
					}
					optCount++;
				}
				sf::sleep(t2 + t2);
				if (x_opt && y_opt) {
					//std::cout << "     " << y_opt << " " << x_opt << "here\n";
					//ch[y_opt - 200][x_opt - 400] = 'T';
					bol = true;
					//cout << "ris\n";
				}

				/*cout << "\nyes did " << nu << endl;
				for (int i = miny - 1; i <= maxy; i++) {
					for (int j = minx; j <= maxx; j++) {
						cout << ch[i - 200][j - 400];
					}
					cout << endl;
				}*/
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			/// Magnetism check

			if (localPosition.x >= 20 && localPosition.x <= 160 && localPosition.y >= 100 && localPosition.y <= 150) {
				if (magnetism)magnetism = false;
				else magnetism = true;
				sf::sleep(t2 + t2);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Saving

			if (localPosition.x >= 200 && localPosition.x <= 340 && localPosition.y >= 20 && localPosition.y <= 70) {

				ofstream save("SaveLoad/" + to_string(floorNumber + 1) + ".txt");
				save << "Offices\n";
				for (int i = 1; i < officeCount; i++) {
					save << i << endl;
					for (int j = 0; j < 8; j++) {
						save << office[i].x[j] << " " << office[i].y[j] << endl;
					}
					save << "number " << office[i].number << endl;
				}

				save << "Printers\n";
				for (int i = 1; i < printerCount; i++) {
					save << i << " " << printer[i].x << " " << printer[i].y << " " << printer[i].infoX << " " << printer[i].infoY << " " << printer[i].image << " " << printer[i].name << endl;
				}

				save << "Stairs\n";
				for (int i = 1; i < stairsCount; i++) {
					save << i << " " << stairs[i].x << " " << stairs[i].y << " " << stairs[i].angle << endl;
				}

				save << "Doors\n";
				for (int i = 1; i < doorCount; i++) {
					save << i << " " << door[i].x << " " << door[i].y << " " << door[i].angle << endl;
				}

				save << "Floor\n";
				for (int i = 0; i < floorCount; i++) {
					save << i << " " << floorX[i] << " " << floorY[i] << endl;
				}
				save.close();
				//sf::sleep(t2 + t2);
			}



			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Loading

			if (localPosition.x >= 200 && localPosition.x <= 340 && localPosition.y >= 100 && localPosition.y <= 170 && !isLoaded) {
				isLoaded = true;
				ifstream load("SaveLoad/" + to_string(floorNumber + 1) + ".txt");
				bool offices = false, printers1 = false, stairs1 = false, door1 = false, floor1 = false, numberFound = false;
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
							numberFound = false;
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
									if (g[0] != 'n')office[officeCount - 1].x[num1] = toInt(g);
									else numberFound = true;
									secondSpace = true;
								}
								else {
									if (!numberFound)office[officeCount - 1].y[num1] = toInt(g);
									else {
										office[officeCount - 1].number = g;
										numberFound = false;
									}
									break;
								}
							}
							num1++;
						}
					}
					if (printers1) {
						string g;
						int num2;
						bool secondSpace = false, thirdSpace = false, fourthSpace = false, fivthSpace = false, sixthSpace = false, seventhSpace = false, eightSpace = false;

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
								printerCount = toInt(g);
								printer[printerCount].id = printerCount;
								secondSpace = true;
							}
							else if (secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
								printer[printerCount].x = toInt(g);
								secondSpace = false;
								thirdSpace = true;
							}
							else if (!secondSpace && thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
								printer[printerCount].y = toInt(g);
								thirdSpace = false;
								secondSpace = false;
								fourthSpace = true;
							}
							else if (!secondSpace && !thirdSpace && fourthSpace && !fivthSpace && !sixthSpace && !seventhSpace) {
								printer[printerCount].infoX = toInt(g);
								thirdSpace = false;
								secondSpace = false;
								fourthSpace = false;
								fivthSpace = true;
							}
							else if (!secondSpace && !thirdSpace && !fourthSpace && fivthSpace && !sixthSpace && !seventhSpace) {
								printer[printerCount].infoY = toInt(g);
								thirdSpace = false;
								secondSpace = false;
								fourthSpace = false;
								fivthSpace = false;
								sixthSpace = true;
							}
							else if (!secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && sixthSpace && !seventhSpace) {
								printer[printerCount].image = g;
								for (int h = printer[printerCount].image.length() - 1; h >= 0 ; h--) {
									//cout << printer[printerCount].image[h] << " " << (int)printer[printerCount].image[h] << endl;
									if (printer[printerCount].image[h] <= 32) {
										printer[printerCount].image.pop_back();
									}
								}
								thirdSpace = false;
								secondSpace = false;
								fourthSpace = false;
								fivthSpace = false;
								sixthSpace = false;
								seventhSpace = true;
							}
							else if (!secondSpace && !thirdSpace && !fourthSpace && !fivthSpace && !sixthSpace && seventhSpace) {
								printer[printerCount].name = g;
								for (int h = printer[printerCount].name.length() - 1; h >= 0; h--) {
									if (printer[printerCount].name[h] == '_')printer[printerCount].name[h] = 32;
									else if (printer[printerCount].name[h] < 32)
										printer[printerCount].name.pop_back();
								}
								thirdSpace = false;
								secondSpace = false;
								fourthSpace = false;
								fivthSpace = false;
								sixthSpace = false;
								seventhSpace = false;
							}
							/*if (eightSpace) {
								printer[printerCount].name = g;
								for (int i = 0; i < printer[printerCount].name.length(); i++) {
									if (printer[printerCount].name[i] == '_')printer[printerCount].name[i] = 32;
									if (printer[printerCount].name[i] == 0)printer[printerCount].name.erase(printer[printerCount].name.begin() + i);
								}
								eightSpace = false;
							}*/
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
								floorX[floorCount] = toInt(g);
								secondSpace = true;
							}
							else {
								floorY[floorCount] = toInt(g);
								floorCount++;
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
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Работа с лестницами
			foundStairs = false;
			for (int i = stairsCount - 1; i >= 0; i--) {

				activeStairs = -1;
				if (localPosition.x >= stairs[i].leftX - 5 && localPosition.x <= stairs[i].rightX + 5 && localPosition.y >= stairs[i].leftY - 5 && localPosition.y <= stairs[i].rightY + 5 && activeDoor == -1 && activeOffice == -1 && activePrinter == -1 && activeFloor == -1) {

					draw = false;
					bool near = false;

					for (int j = 1; j < stairsCount; j++) {
						if (abs((stairs[j].leftX + stairs[j].rightX) / 2 - (stairs[0].leftX + stairs[0].rightX) / 2) <= stairs[0].width && abs((stairs[j].leftY + stairs[j].rightY) / 2 - (stairs[0].leftY + stairs[0].rightY) / 2) <= stairs[0].width) {
							near = true;
							break;
						}
					}

					/// Создание лестницы
					/// 

					if (i == 0 && !near && stairsCount > 1) {
						stairs[stairsCount].x = stairs[0].x;
						stairs[stairsCount].y = stairs[0].y;
						stairs[stairsCount].leftX = stairs[0].x;
						stairs[stairsCount].leftY = stairs[0].y;
						stairs[stairsCount].rightX = stairs[0].x + stairs[0].width;
						stairs[stairsCount].rightY = stairs[0].y + stairs[0].height;

						stairsCount++;
						activeStairs = activeStairs - 1;
					}
					foundStairs = true;

					if (i == 0 && stairsCount == 1) {
						stairs[stairsCount].x = stairs[0].x;
						stairs[stairsCount].y = stairs[0].y;
						stairs[stairsCount].leftX = stairs[0].x;
						stairs[stairsCount].leftY = stairs[0].y;
						stairs[stairsCount].rightX = stairs[0].x + stairs[0].width;
						stairs[stairsCount].rightY = stairs[0].y + stairs[0].height;

						activeStairs++;
						stairsCount++;
						foundStairs = true;
					}
					activeStairs = i;

					if (activeStairs > 0) {
						stairs[activeStairs].x += localPosition.x - mouseX;
						stairs[activeStairs].y += localPosition.y - mouseY;
						stairs[activeStairs].leftX += localPosition.x - mouseX;
						stairs[activeStairs].leftY += localPosition.y - mouseY;
						stairs[activeStairs].rightX += localPosition.x - mouseX;
						stairs[activeStairs].rightY += localPosition.y - mouseY;
					}
					break;
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Работа с дверьми
			foundDoor = false;
			for (int i = doorCount; i >= 0; i--) {

				activeDoor = -1;
				if (localPosition.x >= door[i].leftX - 5 && localPosition.x <= door[i].rightX + 5 && localPosition.y >= door[i].leftY - 5 && localPosition.y <= door[i].rightY + 5 && activePrinter == -1 && activeOffice == -1 && activeStairs == -1 && activeFloor == -1) {

					draw = false;
					bool near = false;

					for (int j = 1; j < doorCount; j++) {
						if (abs((door[j].leftX + door[j].rightX) / 2 - (door[0].leftX + door[0].rightX) / 2) <= 60 && abs((door[j].leftY + door[j].rightY) / 2 - (door[0].leftY + door[0].rightY) / 2) <= 60) {
							near = true;
							break;
						}
					}

					/// Создание лестницы
					/// 

					if (i == 0 && !near && doorCount > 1) {
						door[doorCount].x = door[0].x;
						door[doorCount].y = door[0].y;
						door[doorCount].leftX = door[0].x;
						door[doorCount].leftY = door[0].y;
						door[doorCount].rightX = door[0].x + door[0].width;
						door[doorCount].rightY = door[0].y + door[0].height;

						doorCount++;
						activeDoor = activeDoor - 1;
					}
					foundDoor = true;

					if (i == 0 && doorCount == 1) {
						door[doorCount].x = door[0].x;
						door[doorCount].y = door[0].y;
						door[doorCount].leftX = door[0].x;
						door[doorCount].leftY = door[0].y;
						door[doorCount].rightX = door[0].x + door[0].width;
						door[doorCount].rightY = door[0].y + door[0].height;

						doorCount++;
						activeDoor++;
						foundDoor = true;
					}
					activeDoor = i;

					if (activeDoor > 0) {
						door[activeDoor].x += localPosition.x - mouseX;
						door[activeDoor].y += localPosition.y - mouseY;
						door[activeDoor].leftX += localPosition.x - mouseX;
						door[activeDoor].leftY += localPosition.y - mouseY;
						door[activeDoor].rightX += localPosition.x - mouseX;
						door[activeDoor].rightY += localPosition.y - mouseY;
					}
					break;
				}
			}

			//////////////////////////////////////////////////////////////////////
			/// Изменение размеров офиса
			if (activeOffice > 0 && (edit1 || edit2) && (editing1 >= 0 && editing2 >= 0)) {
				if (editing2 == 7)editing2 = 0;
				if (edit1 && !edit2) {
					office[activeOffice].y[editing1] += localPosition.y - mouseY;
					office[activeOffice].y[editing2] += localPosition.y - mouseY;
					//office[activeOffice].y[editing1] += localPosition.y - mouseY;
					office[activeOffice].height += localPosition.x - mouseX;
				}



				if (edit2 && !edit1) {
					//office[activeOffice].x[editing2] += localPosition.x - mouseX;
					office[activeOffice].x[editing1] += localPosition.x - mouseX;
					office[activeOffice].x[editing2] += localPosition.x - mouseX;
					office[activeOffice].width += localPosition.y - mouseY;
				}

				///
				/// Размеры двери относительно размеров офиса
				/// 
				if (office[0].angle == 0 || office[0].angle == 180 || office[0].angle == 360) {
					office[activeOffice].x[3] = abs((office[activeOffice].x[2] - office[activeOffice].width / 4));
					office[activeOffice].y[3] = abs((office[activeOffice].y[2]));

					office[activeOffice].x[4] = office[activeOffice].x[3];
					office[activeOffice].y[4] = abs((office[activeOffice].y[2] - office[activeOffice].height / 5));

					office[activeOffice].x[5] = abs((office[activeOffice].x[3] - office[activeOffice].width / 4));
					office[activeOffice].y[5] = office[activeOffice].y[3];
				}

				else {
					office[activeOffice].x[3] = office[activeOffice].x[2];
					office[activeOffice].y[3] = abs((office[activeOffice].y[2] - office[activeOffice].width / 4));

					office[activeOffice].x[4] = abs((office[activeOffice].x[2] + office[activeOffice].width / 4));
					office[activeOffice].y[4] = office[activeOffice].y[3];

					office[activeOffice].x[5] = office[activeOffice].x[3];
					office[activeOffice].y[5] = abs((office[activeOffice].y[3] - office[activeOffice].width / 4));
				}

				/////
				/////
				/////

			}

			edit1 = edit2 = false;
			for (int i = 0; i < 8; i++) {
				if (i > 1 && i < 6 || i == 7 || activeOffice <= 0)continue;
				if (abs(localPosition.y - office[activeOffice].y[i]) <= 10 && localPosition.x >= office[activeOffice].x[i] && localPosition.x <= office[activeOffice].x[i + 1]) {
					edit1 = true;
					editing1 = i;
					editing2 = i + 1;
				}
				if (abs(localPosition.x - office[activeOffice].x[i]) <= 10 && ((localPosition.y >= office[activeOffice].y[i] && localPosition.y <= office[activeOffice].y[i + 1]) || (localPosition.y >= office[activeOffice].y[i + 1] && localPosition.y <= office[activeOffice].y[i]))) {
					edit2 = true;
					editing1 = i;
					editing2 = i + 1;
				}
			}
			if (!edit1 && !edit2) {
				editing1 = -1;
				editing2 = -1;
			}


			////////////////////////////////////////////////
			/// Синхронизация точек лестниц и дверей
			for (int i = 1; i < stairsCount; i++) {
				if (stairs[i].angle == 0) {
					stairs[i].leftX = stairs[i].x;
					stairs[i].leftY = stairs[i].y;
					stairs[i].rightX = stairs[i].x + stairs[i].width;
					stairs[i].rightY = stairs[i].y + stairs[i].height;
				}
				if (stairs[i].angle == 90) {
					stairs[i].leftX = stairs[i].x - stairs[i].height;
					stairs[i].leftY = stairs[i].y;
					stairs[i].rightX = stairs[i].x;
					stairs[i].rightY = stairs[i].y + stairs[i].width;
				}
				if (stairs[i].angle == 180) {
					stairs[i].leftX = stairs[i].x - stairs[i].width;
					stairs[i].leftY = stairs[i].y - stairs[i].height;
					stairs[i].rightX = stairs[i].x;
					stairs[i].rightY = stairs[i].y;
				}
				if (stairs[i].angle == 270) {
					stairs[i].leftX = stairs[i].x;
					stairs[i].leftY = stairs[i].y - stairs[i].width;
					stairs[i].rightX = stairs[i].x + stairs[i].height;
					stairs[i].rightY = stairs[i].y;
				}
			}

			for (int i = 1; i < doorCount; i++) {
				if (door[i].angle == 0) {
					door[i].leftX = door[i].x;
					door[i].leftY = door[i].y;
					door[i].rightX = door[i].x + door[i].width;
					door[i].rightY = door[i].y + door[i].height;
				}
				if (door[i].angle == 90) {
					door[i].leftX = door[i].x - door[i].height;
					door[i].leftY = door[i].y;
					door[i].rightX = door[i].x;
					door[i].rightY = door[i].y + door[i].width;
				}
				if (door[i].angle == 180) {
					door[i].leftX = door[i].x - door[i].width;
					door[i].leftY = door[i].y - door[i].height;
					door[i].rightX = door[i].x;
					door[i].rightY = door[i].y;
				}
				if (door[i].angle == 270) {
					door[i].leftX = door[i].x;
					door[i].leftY = door[i].y - door[i].width;
					door[i].rightX = door[i].x + door[i].height;
					door[i].rightY = door[i].y;
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Переход на другой этаж
			for (int i = 0; i < 5; i++) {
				if (localPosition.x >= floorNumberX[i] && localPosition.x <= floorNumberX[i] + 48 && localPosition.y >= floorNumberY[i] && localPosition.y <= floorNumberY[i] + 48) {
					if (floorNumber != i) {
						ofstream save("SaveLoad/" + to_string(floorNumber + 1) + "t.txt");
						save << "Offices\n";
						for (int i = 1; i < officeCount; i++) {
							save << i << endl;
							for (int j = 0; j < 8; j++) {
								save << office[i].x[j] << " " << office[i].y[j] << endl;
							}
							save << "number " << office[i].number << endl;
						}

						save << "Printers\n";
						for (int i = 1; i < printerCount; i++) {
							save << i << " " << printer[i].x << " " << printer[i].y << " " << printer[i].infoX << " " << printer[i].infoY << " " << printer[i].image << endl;
						}
						save << "Stairs\n";
						for (int i = 1; i < stairsCount; i++) {
							save << i << " " << stairs[i].x << " " << stairs[i].y << " " << stairs[i].angle << endl;
						}
						save << "Doors\n";
						for (int i = 1; i < doorCount; i++) {
							save << i << " " << door[i].x << " " << door[i].y << " " << door[i].angle << endl;
						}
						save << "Floor\n";
						for (int i = 0; i < floorCount; i++) {
							save << i << " " << floorX[i] << " " << floorY[i] << endl;
						}
						save.close();
						std::cout << "save: " << floorNumber + 1 << endl;
						floorNumber = i;
						///
						/// Удаление
						/// 
						for (int i = 1; i < officeCount; i++) {
							office.erase(office.begin() + i);
						}
						for (int i = 1; i < printerCount; i++) {
							printer.erase(printer.begin() + i);
						}
						for (int i = 1; i < stairsCount; i++) {
							stairs.erase(stairs.begin() + i);
						}
						for (int i = 1; i < doorCount; i++) {
							door.erase(door.begin() + i);
						}
						int sss = floorX.size();
						for (int i = 0; i < floorCount; i++) {
							//floorX.pop_back();
							//floorY.pop_back();
							//floorX.erase(floorX.begin() + i);
							//floorY.erase(floorY.begin() + i);
						}
						//floor.erase(floor.begin());
						officeCount = printerCount = stairsCount = doorCount = 1;
						floorCount = 0;
						///
						/// Загрузка
						std::cout << "load: " << floorNumber + 1 << endl;
						ifstream load("SaveLoad/" + to_string(floorNumber + 1) + "t.txt");
						bool offices = false, printers1 = false, stairs1 = false, door1 = false, floor1 = false, numberFound = false;
						int num1 = 0;
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
									num1 = 0;
									officeCount++;
									numberFound = false;
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
											if (g[0] != 'n')office[officeCount - 1].x[num1] = toInt(g);
											else numberFound = true;
											secondSpace = true;
										}
										else {
											if (!numberFound)office[officeCount - 1].y[num1] = toInt(g);
											else {
												office[officeCount - 1].number = g;
												numberFound = false;
											}
											break;
										}
									}
									num1++;
								}
							}
							if (printers1) {
								string g = s;
								int num2;
								bool secondSpace = false, thirdSpace = false, fourthSpace = false, fivthSpace = false, sixthSpace = false, seventhSpace = false;

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
										/*for (int h = printer[printerCount - 1].name.length(); h >= 0 ; h--) {
											if (printer[printerCount - 1].image[h] <= 32) {
												printer[printerCount - 1].image.pop_back();
											}
										}*/
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
								string g = s;
								int num2;
								bool secondSpace = false, thirdSpace = false, fourthSpace = false;
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
										stairsCount = toInt(g);
										secondSpace = true;
									}
									else if (secondSpace && !thirdSpace && !fourthSpace) {
										stairs[stairsCount].x = toInt(g);
										secondSpace = false;
										thirdSpace = true;
									}
									else if (!secondSpace && thirdSpace && !fourthSpace) {
										stairs[stairsCount].y = toInt(g);
										thirdSpace = false;
										fourthSpace = true;
									}
									else if (!secondSpace && !thirdSpace && fourthSpace) {
										stairs[stairsCount].angle = toInt(g);
										thirdSpace = false;
										fourthSpace = true;
									}
								}
							}

							if (door1) {
								string g = s;
								bool secondSpace = false, thirdSpace = false, fourthSpace = false;

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
										door[doorCount].x = toInt(g);
										secondSpace = false;
										thirdSpace = true;
									}
									else if (!secondSpace && thirdSpace && !fourthSpace) {
										door[doorCount].y = toInt(g);
										thirdSpace = false;
										fourthSpace = true;
									}
									else if (!secondSpace && !thirdSpace && fourthSpace) {
										door[doorCount].angle = toInt(g);
										thirdSpace = false;
										fourthSpace = true;
									}
								}
							}
							if (floor1) {
								string g = s;
								bool secondSpace = false;
								int len;
								for (int i = 0; i < s.length(); i++) {
									if (s[i] <= 32) {
										len = i + 1;
										break;
									}
								}
								g = s;
								for (int j = len; j < s.length(); j++) {
									int point = 0;
									for (int i = 0; i < s.length(); i++)
										g[i] = 0;
									for (int h = j;; h++, j++) {
										if (s[h] <= 32)break;
										g[point++] = s[h];
									}
									if (!secondSpace) {
										floorX[floorCount] = toInt(g);
										//cout << " = " << floorX[floorCount] << " " << g << " " << toInt(g) << endl;
										secondSpace = true;
									}
									else {
										floorY[floorCount] = toInt(g);
										floorCount++;
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
					else {
						floorNumber = i;
					}

				}
			}
		}
	A:


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// OFFICE ROTATE
		/// 
		if (Mouse::isButtonPressed(Mouse::Right)) {
			sf::Texture texture;
			texture.create(1920, 1080);
			texture.update(window);
			Image img = texture.copyToImage();
			thread t([img]() {
				img.saveToFile("images/test.png");
				});
			t.detach();
			if (texture.copyToImage().saveToFile("images/aaa.png"))
			{
				std::cout << "Screenshot saved to images/" << std::endl;
			}
			else
			{
				// Error handling
			}
			take_screenshot(window, "images/aaa.jpg");

			if (activeStairs > 0 && foundStairs) {
				stairs[activeStairs].angle += 90;
				if (stairs[activeStairs].angle >= 360)stairs[activeStairs].angle %= 360;

				for (int i = 1; i < stairsCount; i++) {
					if (stairs[i].angle == 0) {
						stairs[i].leftX = stairs[i].x;
						stairs[i].leftY = stairs[i].y;
						stairs[i].rightX = stairs[i].x + stairs[i].width;
						stairs[i].rightY = stairs[i].y + stairs[i].height;
					}
					if (stairs[i].angle == 90) {
						stairs[i].leftX = stairs[i].x - stairs[i].height;
						stairs[i].leftY = stairs[i].y;
						stairs[i].rightX = stairs[i].x;
						stairs[i].rightY = stairs[i].y + stairs[i].width;
					}
					if (stairs[i].angle == 180) {
						stairs[i].leftX = stairs[i].x - stairs[i].width;
						stairs[i].leftY = stairs[i].y - stairs[i].height;
						stairs[i].rightX = stairs[i].x;
						stairs[i].rightY = stairs[i].y;
					}
					if (stairs[i].angle == 270) {
						stairs[i].leftX = stairs[i].x;
						stairs[i].leftY = stairs[i].y - stairs[i].width;
						stairs[i].rightX = stairs[i].x + stairs[i].height;
						stairs[i].rightY = stairs[i].y;
					}
				}
			}

			if (activeDoor > 0 && foundDoor) {
				door[activeDoor].angle += 90;
				if (door[activeDoor].angle >= 360)door[activeDoor].angle %= 360;
				for (int i = 1; i < doorCount; i++) {
					if (door[i].angle == 0) {
						door[i].leftX = door[i].x;
						door[i].leftY = door[i].y;
						door[i].rightX = door[i].x + door[i].width;
						door[i].rightY = door[i].y + door[i].height;
					}
					if (door[i].angle == 90) {
						door[i].leftX = door[i].x - door[i].height;
						door[i].leftY = door[i].y;
						door[i].rightX = door[i].x;
						door[i].rightY = door[i].y + door[i].width;
					}
					if (door[i].angle == 180) {
						door[i].leftX = door[i].x - door[i].width;
						door[i].leftY = door[i].y - door[i].height;
						door[i].rightX = door[i].x;
						door[i].rightY = door[i].y;
					}
					if (door[i].angle == 270) {
						door[i].leftX = door[i].x;
						door[i].leftY = door[i].y - door[i].width;
						door[i].rightX = door[i].x + door[i].height;
						door[i].rightY = door[i].y;
					}
				}
			}

			if (activeOffice > 0) {
				office[activeOffice].angle += 90;
				if (office[activeOffice].angle > 360)office[activeOffice].angle %= 360;
				for (int i = 1; i < 8; i++) {
					if (office[activeOffice].angle == 90) {
						if (i == 1) {
							office[activeOffice].y[i] = abs(office[activeOffice].y[0] + office[activeOffice].width);
							office[activeOffice].x[i] = office[activeOffice].x[0];
						}
						if (i == 2) {
							office[activeOffice].y[i] = office[activeOffice].y[0] + office[activeOffice].width;
							office[activeOffice].x[i] = office[activeOffice].x[0] - office[activeOffice].height;
						}
						if (i == 3) {
							office[activeOffice].y[3] = office[activeOffice].y[2] - 20;
							office[activeOffice].x[3] = office[activeOffice].x[2];

							office[activeOffice].y[4] = office[activeOffice].y[2] - 20;
							office[activeOffice].x[4] = office[activeOffice].x[2] + 20;

							office[activeOffice].y[5] = office[activeOffice].y[3] - 20;
							office[activeOffice].x[5] = office[activeOffice].x[2];

						}
						if (i == 6) {
							office[activeOffice].y[6] = office[activeOffice].y[0];
							office[activeOffice].x[6] = office[activeOffice].x[0] - office[activeOffice].height;

							office[activeOffice].x[7] = office[activeOffice].x[0];
							office[activeOffice].y[7] = office[activeOffice].y[0];
						}
						office[activeOffice].left[0] = office[activeOffice].x[6];
						office[activeOffice].left[1] = office[activeOffice].y[6];
						office[activeOffice].right[0] = office[activeOffice].x[1];
						office[activeOffice].right[1] = office[activeOffice].y[1];
					}

					if (office[activeOffice].angle == 180) {

						if (i == 1) {
							office[activeOffice].y[i] = office[activeOffice].y[0];
							office[activeOffice].x[i] = office[activeOffice].x[0] - office[activeOffice].width;
						}
						if (i == 2) {
							office[activeOffice].y[i] = office[activeOffice].y[0] - office[activeOffice].height;
							office[activeOffice].x[i] = office[activeOffice].x[1];
						}

						if (i == 3) {
							office[activeOffice].y[3] = office[activeOffice].y[2];
							office[activeOffice].x[3] = office[activeOffice].x[2] + 20;

							office[activeOffice].y[4] = office[activeOffice].y[2] + 20;
							office[activeOffice].x[4] = office[activeOffice].x[2] + 20;

							office[activeOffice].y[5] = office[activeOffice].y[3];
							office[activeOffice].x[5] = office[activeOffice].x[2] + 40;
						}

						if (i == 6) {
							office[activeOffice].y[6] = office[activeOffice].y[0] - office[activeOffice].height;
							office[activeOffice].x[6] = office[activeOffice].x[0];

							office[activeOffice].x[7] = office[activeOffice].x[0];
							office[activeOffice].y[7] = office[activeOffice].y[0];
						}
						office[activeOffice].left[0] = office[activeOffice].x[2];
						office[activeOffice].left[1] = office[activeOffice].y[2];
						office[activeOffice].right[0] = office[activeOffice].x[0];
						office[activeOffice].right[1] = office[activeOffice].y[0];
					}

					if (office[activeOffice].angle == 270) {
						if (i == 1) {
							office[activeOffice].y[i] = office[activeOffice].y[0] - office[activeOffice].width;
							office[activeOffice].x[i] = office[activeOffice].x[0];
						}
						if (i == 2) {
							office[activeOffice].y[i] = office[activeOffice].y[1];
							office[activeOffice].x[i] = office[activeOffice].x[0] + office[activeOffice].height;
						}
						if (i == 3) {
							office[activeOffice].y[3] = office[activeOffice].y[2] + 20;
							office[activeOffice].x[3] = office[activeOffice].x[2];

							office[activeOffice].y[4] = office[activeOffice].y[2] + 20;
							office[activeOffice].x[4] = office[activeOffice].x[2] - 20;

							office[activeOffice].y[5] = office[activeOffice].y[3] + 20;
							office[activeOffice].x[5] = office[activeOffice].x[2];

						}
						if (i == 6) {
							office[activeOffice].y[6] = office[activeOffice].y[0];
							office[activeOffice].x[6] = office[activeOffice].x[0] + office[activeOffice].height;

							office[activeOffice].x[7] = office[activeOffice].x[0];
							office[activeOffice].y[7] = office[activeOffice].y[0];
						}

						office[activeOffice].left[0] = office[activeOffice].x[1];
						office[activeOffice].left[1] = office[activeOffice].y[1];
						office[activeOffice].right[0] = office[activeOffice].x[6];
						office[activeOffice].right[1] = office[activeOffice].y[6];
					}

					if (office[activeOffice].angle == 360) {
						if (i == 1) {
							office[activeOffice].y[i] = office[activeOffice].y[0];
							office[activeOffice].x[i] = office[activeOffice].x[0] + office[activeOffice].width;
						}
						if (i == 2) {
							office[activeOffice].y[i] = office[activeOffice].y[0] + office[activeOffice].height;
							office[activeOffice].x[i] = office[activeOffice].x[0] + office[activeOffice].width;
						}

						if (i == 3) {
							office[activeOffice].y[3] = office[activeOffice].y[2];
							office[activeOffice].x[3] = office[activeOffice].x[2] - 20;

							office[activeOffice].y[4] = office[activeOffice].y[2] - 20;
							office[activeOffice].x[4] = office[activeOffice].x[2] - 20;

							office[activeOffice].y[5] = office[activeOffice].y[3];
							office[activeOffice].x[5] = office[activeOffice].x[2] - 40;
						}

						if (i == 6) {
							office[activeOffice].y[6] = office[activeOffice].y[0] + office[activeOffice].height;
							office[activeOffice].x[6] = office[activeOffice].x[0];

							office[activeOffice].x[7] = office[activeOffice].x[0];
							office[activeOffice].y[7] = office[activeOffice].y[0];
						}

						office[activeOffice].left[0] = office[activeOffice].x[0];
						office[activeOffice].left[1] = office[activeOffice].y[0];
						office[activeOffice].right[0] = office[activeOffice].x[2];
						office[activeOffice].right[1] = office[activeOffice].y[2];
					}
				}
			}
			sf::sleep(t1);

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//if (keyboardInput() == 2)cout << "OLALLLA\n";
		if (found && activeOffice >= 0) {

			if (office[activeOffice].angle == 0 || office[activeOffice].angle == 180 || office[activeOffice].angle == 360) {
				office[activeOffice].width = (office[activeOffice].right[0] - office[activeOffice].left[0]);
				office[activeOffice].height = (office[activeOffice].right[1] - office[activeOffice].left[1]);
			}

			else {
				office[activeOffice].height = (office[activeOffice].right[0] - office[activeOffice].left[0]);
				office[activeOffice].width = (office[activeOffice].right[1] - office[activeOffice].left[1]);
			}
			int opa = keyboardInput();
			if (opa > 2) {
				office[activeOffice].number = office[activeOffice].number + char(opa);
				deletion = 1;
				sf::sleep(timeForPrinting);
			}
			else if (opa == 0 && office[activeOffice].number.length() - deletion >= 0) {
				if (office[activeOffice].number.length() - deletion >= 0) {
					//office[activeOffice].number[office[activeOffice].number.length() - deletion] = 32;
					//deletion++;
					office[activeOffice].number.pop_back();
					sf::sleep(timeForPrinting);
				}
			}
			else if (opa == 2) {
				/*office[activeOffice].number = office[activeOffice].number + "\n";

				office[activeOffice].number += char(92);
				office[activeOffice].number += "n";*/
				office[activeOffice].number.append("\n");
				cout << office[activeOffice].number << endl;
				sf::sleep(timeForPrinting);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// Примагничивание офиса к офису
			if (magnetism) {
				for (int i = 1; i < officeCount; i++) {
					if (abs(office[activeOffice].x[0] - office[i].x[1]) <= 15 && abs(office[activeOffice].y[0] - office[i].y[1]) <= 15) {
						int xx = office[activeOffice].x[0] - office[i].x[1], yy = office[activeOffice].y[0] - office[i].y[1];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[1] - office[i].x[0]) <= 15 && abs(office[activeOffice].y[1] - office[i].y[0]) <= 15) {
						int xx = office[activeOffice].x[1] - office[i].x[0], yy = office[activeOffice].y[1] - office[i].y[0];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[2] - office[i].x[1]) <= 15 && abs(office[activeOffice].y[2] - office[i].y[1]) <= 15) {
						int xx = office[activeOffice].x[2] - office[i].x[1], yy = office[activeOffice].y[2] - office[i].y[1];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[1] - office[i].x[2]) <= 15 && abs(office[activeOffice].y[1] - office[i].y[2]) <= 15) {
						int xx = office[activeOffice].x[1] - office[i].x[2], yy = office[activeOffice].y[1] - office[i].y[2];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[6] - office[i].x[0]) <= 15 && abs(office[activeOffice].y[6] - office[i].y[0]) <= 15) {
						int xx = office[activeOffice].x[6] - office[i].x[0], yy = office[activeOffice].y[6] - office[i].y[0];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}

				}
				///
				/// Примагничивание к точкам этажа
				/// 
				for (int i = 0; i < floorCount; i++) {
					if (abs(office[activeOffice].x[0] - floorX[i]) <= 15 && abs(office[activeOffice].y[0] - floorY[i]) <= 15) {
						int xx = office[activeOffice].x[0] - floorX[i], yy = office[activeOffice].y[0] - floorY[i];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[1] - floorX[i]) <= 15 && abs(office[activeOffice].y[1] - floorY[i]) <= 15) {
						int xx = office[activeOffice].x[1] - floorX[i], yy = office[activeOffice].y[1] - floorY[i];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[2] - floorX[i]) <= 15 && abs(office[activeOffice].y[2] - floorY[i]) <= 15) {
						int xx = office[activeOffice].x[2] - floorX[i], yy = office[activeOffice].y[2] - floorY[i];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
					if (abs(office[activeOffice].x[6] - floorX[i]) <= 15 && abs(office[activeOffice].y[6] - floorY[i]) <= 15) {
						int xx = office[activeOffice].x[6] - floorX[i], yy = office[activeOffice].y[6] - floorY[i];
						for (int h = 0; h < 8; h++) {
							office[activeOffice].x[h] -= xx;
							office[activeOffice].y[h] -= yy;
						}
					}
				}

			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			/// Рамка выделения офиса
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);
			mode.setPosition(office[activeOffice].left[0] - 3, office[activeOffice].left[1] - 3);
			if (office[activeOffice].angle == 0 || office[activeOffice].angle == 180 || office[activeOffice].angle == 360)mode.setSize(Vector2f(office[activeOffice].width + 4, office[activeOffice].height + 5));
			else mode.setSize(Vector2f(office[activeOffice].height + 5, office[activeOffice].width + 4));
			window.draw(mode);

			mode.setOutlineThickness(0);
			/// Площадь
			if (activeOffice > 0) {
				textMode.setPosition(localPosition.x + 7, localPosition.y - 6);
				textMode.setString(to_string((office[activeOffice].height / 15) * (office[activeOffice].width / 15)));
				textMode.setCharacterSize(12);
				window.draw(textMode);
			}
			/// Работники
			ifstream file("people/name_people.txt");
			int hei = 200;
			if (file.is_open()) {
				bool b = false;
				while (file) {
					string s;
					getline(file, s);
					int now = activeOffice + 100;
					if (toInt(s) == now) {
						b = true;
					}
					else if (s[0] == '1' && (int(s[1]) - 48 != now / 10 || int(s[2]) - 48 != now % 10)) {
						b = 0;
					}
					if (b) {
						textMode.setCharacterSize(15);
						textMode.setFillColor(Color::Black);
						textMode.setString(s);
						textMode.setPosition(10, hei);
						window.draw(textMode);
						hei += 30;
					}
				}
			}
			else {
				textMode.setCharacterSize(15);
				textMode.setFillColor(Color::Black);
				textMode.setString("NOT");
				textMode.setPosition(10, hei);
				window.draw(textMode);
				hei += 30;
			}
			file.close();
		}

		if (foundPrinter && activePrinter >= 0) {
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);
			mode.setPosition(printer[activePrinter].x - 3, printer[activePrinter].y - 3);
			mode.setSize(Vector2f(34, 30));
			window.draw(mode);
		}

		if (foundPrInfo && printerInfo > 0) {
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);
			mode.setPosition(printer[printerInfo].infoX - 3, printer[printerInfo].infoY - 3);
			mode.setSize(Vector2f(50, 92));
			window.draw(mode);
		}

		if (floorPoint && !draw && activeFloor >= 0) {
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);
			mode.setPosition(floorX[activeFloor] - 3, floorY[activeFloor] - 3);
			mode.setSize(Vector2f(6, 6));
			window.draw(mode);
		}

		if (foundStairs && activeStairs >= 0) {
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);

			if (stairs[activeStairs].angle == 0)
				mode.setSize(Vector2f(stairs[activeStairs].width + 3, stairs[activeStairs].height + 3));
			if (stairs[activeStairs].angle == 90)
				mode.setSize(Vector2f(stairs[activeStairs].height + 3, stairs[activeStairs].width + 3));
			if (stairs[activeStairs].angle == 180)
				mode.setSize(Vector2f(stairs[activeStairs].width + 3, stairs[activeStairs].height + 3));
			if (stairs[activeStairs].angle == 270)
				mode.setSize(Vector2f(stairs[activeStairs].height + 3, stairs[activeStairs].width + 3));

			mode.setPosition(stairs[activeStairs].leftX - 3, stairs[activeStairs].leftY - 3);
			window.draw(mode);
		}

		if (foundDoor && activeDoor >= 0) {
			mode.setFillColor(Color::White);
			mode.setOutlineColor(Color::Yellow);
			mode.setOutlineThickness(2);

			if (door[activeDoor].angle == 0)
				mode.setSize(Vector2f(door[activeDoor].width + 3, door[activeDoor].height + 3));
			if (door[activeDoor].angle == 90)
				mode.setSize(Vector2f(door[activeDoor].height + 3, door[activeDoor].width + 3));
			if (door[activeDoor].angle == 180)
				mode.setSize(Vector2f(door[activeDoor].width + 3, door[activeDoor].height + 3));
			if (door[activeDoor].angle == 270)
				mode.setSize(Vector2f(door[activeDoor].height + 3, door[activeDoor].width + 3));


			mode.setPosition(door[activeDoor].leftX - 3, door[activeDoor].leftY - 3);
			window.draw(mode);
		}
		mouseX = localPosition.x;
		mouseY = localPosition.y;

		////////////////////////////
		/// Рисование квадратиков Магнетизма, Сохранения, Загрузки и Рисования этажа
		/// Рис. этаж
		mode.setOutlineThickness(0);
		mode.setPosition(20, 20);
		mode.setFillColor(Color::Black);
		mode.setSize(Vector2f(140, 50));
		mode.setOutlineColor(Color::Black);
		window.draw(mode);


		/// Магнетизм
		mode.setPosition(20, 100);
		if (magnetism) {
			mode.setFillColor(Color::Blue);
			mode.setOutlineColor(Color::Blue);
		}
		else {
			mode.setFillColor(Color::Red);
			mode.setOutlineColor(Color::Red);
		}
		mode.setSize(Vector2f(140, 50));
		window.draw(mode);

		for (int i = 1; i < optCount; i++) {
			if (x_opt[i] && y_opt[i]) {
				mode.setPosition(Vector2f(x_opt[i], y_opt[i]));
				mode.setFillColor(Color::Black);
				mode.setSize(Vector2f(10, 10));
				mode.setOutlineColor(Color::Blue);
				window.draw(mode);
			}
		}

		/// Сохранить
		mode.setPosition(200, 20);
		mode.setFillColor(Color::Black);
		mode.setSize(Vector2f(140, 50));
		mode.setOutlineColor(Color::Black);
		window.draw(mode);

		/// Загрузить
		mode.setPosition(200, 100);
		mode.setFillColor(Color::Black);
		mode.setSize(Vector2f(140, 50));
		mode.setOutlineColor(Color::Black);
		window.draw(mode);

		/// Место принтеров
		mode.setPosition(380, 20);
		mode.setFillColor(Color::Black);
		mode.setSize(Vector2f(140, 50));
		mode.setOutlineColor(Color::Black);
		window.draw(mode);

		/// Очистить
		mode.setPosition(380, 100);
		mode.setFillColor(Color::Black);
		mode.setSize(Vector2f(140, 50));
		mode.setOutlineColor(Color::Black);
		window.draw(mode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Рис. этаж");
		textMode.setPosition(35, 30);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Магнетизм");
		textMode.setPosition(30, 110);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Сохранить");
		textMode.setPosition(210, 30);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Загрузить");
		textMode.setPosition(215, 110);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Принтеры");
		textMode.setPosition(395, 30);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::White);
		textMode.setString(L"Очистить");
		textMode.setPosition(395, 110);
		window.draw(textMode);

		textMode.setCharacterSize(23);
		textMode.setFillColor(Color::Black);
		textMode.setString(L"Этаж:");
		textMode.setPosition(floorNumberX[0] - 70, floorNumberY[0] + 10);
		window.draw(textMode);

		for (int i = 0; i < 5; i++) {
			if (i == floorNumber) {
				mode.setPosition(floorNumberX[i] + 2, floorNumberY[i] + 2);
				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Black);
				mode.setOutlineThickness(2);
				mode.setSize(Vector2f(48, 46));
				textMode.setFillColor(Color::Black);
			}
			else {
				mode.setPosition(floorNumberX[i], floorNumberY[i]);
				mode.setOutlineThickness(0);
				mode.setFillColor(Color::Black);
				mode.setOutlineColor(Color::Black);
				textMode.setFillColor(Color::White);
				mode.setSize(Vector2f(50, 50));
			}
			window.draw(mode);

			textMode.setCharacterSize(32);
			textMode.setString(to_string(i + 1));
			textMode.setPosition(floorNumberX[i] + 15, floorNumberY[i] + 5);
			textMode.setStyle(Text::Bold);
			window.draw(textMode);
		}
		textMode.setStyle(0);
		mode.setOutlineThickness(0);
		///////////////////////////
		/// Характеристики принтера
		/// 
		if (foundPrinter && activePrinter > 0) {

			if (printerInfoEditingName) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 220);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() > 2) {
					printer[activePrinter].name = printer[activePrinter].name + char(keyboardInput());
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0 && printer[activePrinter].name.length() > 0) {
					if (printer[activePrinter].name.length() > 0) {
						printer[activePrinter].name.pop_back();
						sf::sleep(timeForPrinting);
					}
				}
				else if (keyboardInput() == 2) {
					printer[activePrinter].name.append("\n");
					sf::sleep(timeForPrinting);
				}
			}

			if (printerInfoEditingId) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 250);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() >= 48 && keyboardInput() <= 57) {
					printer[activePrinter].id = printer[activePrinter].id * 10 + int(keyboardInput() - 48);
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0) {
					printer[activePrinter].id /= 10;
					sf::sleep(timeForPrinting);
				}
			}

			if (printerInfoEditingColor) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 280);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() >= 48 && keyboardInput() <= 49) {
					printer[activePrinter].color = int(keyboardInput() - 48);
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0) {
					printer[activePrinter].color = 0;
					sf::sleep(timeForPrinting);
				}
			}
			if (printerInfoEditingExpPages) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 310);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() >= 48 && keyboardInput() <= 57) {
					printer[activePrinter].expectedPages = printer[activePrinter].expectedPages * 10 + int(keyboardInput() - 48);
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0) {
					printer[activePrinter].expectedPages /= 10;
					sf::sleep(timeForPrinting);
				}
			}
			if (printerInfoEditingPrintPages) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 340);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() >= 48 && keyboardInput() <= 57) {
					printer[activePrinter].printedPages = printer[activePrinter].printedPages * 10 + int(keyboardInput() - 48);
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0) {
					printer[activePrinter].printedPages /= 10;
					sf::sleep(timeForPrinting);
				}
			}
			if (printerInfoEditingImage) {

				mode.setFillColor(Color::White);
				mode.setOutlineColor(Color::Yellow);
				mode.setOutlineThickness(2);
				mode.setPosition(20, 370);
				mode.setSize(Vector2f(330, 30));
				window.draw(mode);

				if (keyboardInput() > 2) {
					printer[activePrinter].image = printer[activePrinter].image + char(keyboardInput());
					deletion = 1;
					sf::sleep(timeForPrinting);
				}
				else if (keyboardInput() == 0 && printer[activePrinter].image.length() > 0) {
					if (printer[activePrinter].image.length() > 0) {
						//deletion++;
						printer[activePrinter].image.pop_back();
						sf::sleep(timeForPrinting);
					}
				}
			}

			textMode.setCharacterSize(14);
			textMode.setFillColor(Color::Black);
			textMode.setString(L"Имя: ");
			textMode.setPosition(25, 225);
			window.draw(textMode);
			textMode.setString(L"ID: ");
			textMode.setPosition(25, 255);
			window.draw(textMode);
			textMode.setString(L"Цвет (0 - Ч/Б, 1 - Цветной): ");
			textMode.setPosition(25, 285);
			window.draw(textMode);
			textMode.setString(L"Кол-во страниц (план.): ");
			textMode.setPosition(25, 315);
			window.draw(textMode);
			textMode.setString(L"Кол-во страниц (факт.): ");
			textMode.setPosition(25, 345);
			window.draw(textMode);
			textMode.setString(L"Изображение: ");
			textMode.setPosition(25, 375);
			window.draw(textMode);


			textMode.setString(printer[activePrinter].name);
			textMode.setPosition(60, 225);
			window.draw(textMode);
			textMode.setString(to_string(printer[activePrinter].id));
			textMode.setPosition(55, 255);
			window.draw(textMode);
			textMode.setString(to_string(printer[activePrinter].color));
			textMode.setPosition(215, 285);
			window.draw(textMode);
			textMode.setString(to_string(printer[activePrinter].expectedPages));
			textMode.setPosition(215, 315);
			window.draw(textMode);
			textMode.setString(to_string(printer[activePrinter].printedPages));
			textMode.setPosition(215, 345);
			window.draw(textMode);
			textMode.setCharacterSize(12);
			textMode.setString(printer[activePrinter].image);
			textMode.setPosition(125, 376);
			window.draw(textMode);
			textMode.setCharacterSize(14);
		}

		///
		////////////////////////////////////////////////
		/// Нумерация офисов

		/*for(int j = 0; j < officeCount; j++)
			for (int i = 0; i < 8; i++) {
				textMode.setPosition(office[0].x[i], office[0].y[i]);
				textMode.setString(char(i + 48));
				window.draw(textMode);
			}*/
		for (int j = 1; j < officeCount; j++)
			for (int i = 0; i < 8; i++) {
				if (office[j].angle == 0 || office[j].angle == 360) {
					textMode.setPosition(office[j].left[0] + 20, office[j].left[1] + 20);
				}
				if (office[j].angle == 90) {
					textMode.setPosition(office[j].right[0] - 40, office[j].left[1] + 20);
				}
				if (office[j].angle == 180) {
					textMode.setPosition(office[j].left[0] + 20, office[j].right[1] - 40);
				}
				if (office[j].angle == 270) {
					textMode.setPosition(office[j].left[0] + 20, office[j].left[1] + 20);
				}
				for (int h = 0; h < office[j].number.length(); h++) {
					if (office[j].number[h] == 0)office[j].number.erase(office[j].number.begin() + h);
				}
				textMode.setCharacterSize(15);
				textMode.setFillColor(Color::Black);
				textMode.setString(office[j].number);
				window.draw(textMode);
			}
		//////////////////////////////////////////
		/// Рисовка офисов
		for (int i = 0; i < officeCount; i++) {
			for (int j = 0; j < 8; j++) {
				room[j].position = Vector2f(office[i].x[j], office[i].y[j]);
				room[j].color = Color::Black;
			}
			if (edit1)
				room[editing1].color = Color::Cyan;

			if (edit2)
				room[editing1].color = Color::Cyan;


			window.draw(room);
		}

		//////////////////////////////////////////
		/// Рисовка лестниц

		for (int i = 0; i < stairsCount; i++) {
			Texture stairsTex;
			stairsTex.loadFromFile("images/Stairs.png", IntRect(0, 0, 59, 210));
			Sprite stairsSprite;
			stairsSprite.setTexture(stairsTex);
			stairsSprite.setPosition(stairs[i].x, stairs[i].y);
			stairsSprite.setRotation(stairs[i].angle);
			window.draw(stairsSprite);
		}

		//////////////////////////////////////////
		/// Рисовка дверей

		for (int i = 0; i < doorCount; i++) {
			Texture doorTex;
			doorTex.loadFromFile("images/Door.png", IntRect(0, 0, 27, 56));
			Sprite doorSprite;
			doorSprite.setTexture(doorTex);
			doorSprite.setPosition(door[i].x, door[i].y);
			doorSprite.setRotation(door[i].angle);
			window.draw(doorSprite);
		}

		//////////////////////////////////////////
		/// Рисовка этажа
		for (int i = 0; i < floorCount; i++) {
			floorD[i].position = Vector2f(floorX[i], floorY[i]);
			floorD[i].color = Color::Black;
		}
		for (int i = floorCount; i < 300; i++) {
			floorD[i].position = Vector2f(floorX[floorCount - 1], floorY[floorCount - 1]);
			floorD[i].color = Color::Black;
		}
		window.draw(floorD);


		//sleep(t1 + t1);
		window.draw(room);


		CircleShape circle(300, 100);
		for (int i = 1; i < printerCount; i++) {
			circle.setPosition(printer[i].x - 300 + 29, printer[i].y - 300 + 25);
			circle.setFillColor(Color(50, 255, 50, 50));
			window.draw(circle);
		}
		

		designBar[0].position = Vector2f(340, 1100);
		designBar[0].color = Color::Cyan;
		designBar[1].position = Vector2f(340, 150);
		designBar[1].color = Color::Cyan;
		designBar[2].position = Vector2f(2000, 150);
		designBar[2].color = Color::Cyan;
		designBar[3].position = Vector2f(0, 150);
		designBar[3].color = Color::Cyan;
		window.draw(designBar);

		if (floorCount > 0 && draw) {
			Vertex tail[] =
			{
				Vertex(Vector2f(floorX[floorCount - 1], floorY[floorCount - 1])),
				Vertex(Vector2f(localPosition.x, localPosition.y))
			};
			tail[0].color = Color::Black;
			tail[1].color = Color::Black;
			int A = sqrt((floorX[floorCount - 1] - localPosition.x) * (floorX[floorCount - 1] - localPosition.x) + (floorY[floorCount - 1] - localPosition.y) * (floorY[floorCount - 1] - localPosition.y)) / 10;
			textMode.setCharacterSize(12);
			textMode.setString(to_string(int(A)));
			textMode.setFillColor(Color::Black);
			textMode.setPosition(localPosition.x + 7, localPosition.y - 6);
			
			window.draw(tail, 2, Lines);
			window.draw(textMode);
		}

		//////////////////////////////////////////
		for (int i = 0; i < printerCount; i++) {
			spritePrint.setPosition(printer[i].x, printer[i].y);
			window.draw(spritePrint);
			if (i > 0) {
				Texture printerInfo;
				printerInfo.loadFromFile(printer[i].image, IntRect(0, 0, 47, 89));
				Sprite printerInfoSprite;
				printerInfoSprite.setTexture(printerInfo);
				printerInfoSprite.setPosition(printer[i].infoX, printer[i].infoY);
				window.draw(printerInfoSprite);

				textMode.setString(printer[i].name);
				textMode.setCharacterSize(15);
				textMode.setFillColor(Color::Black);
				textMode.setPosition((printer[i].infoX - printer[i].name.length() * 2), printer[i].infoY + 95);
				window.draw(textMode);

				Vertex connect[] =
				{
					Vertex(Vector2f((printer[i].x + printer[i].x + 29) / 2, (printer[i].y + printer[i].y + 25) / 2)),
					Vertex(Vector2f((printer[i].infoX + printer[i].infoX + 47) / 2, (printer[i].infoY + printer[i].infoY + 89) / 2))
				};
				connect[0].color = Color::Black;
				connect[1].color = Color::Black;
				window.draw(connect, 2, Lines);
			}
		}

		window.display();
		window.clear();
	}

	return 0;
}