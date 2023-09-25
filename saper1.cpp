﻿#include <iostream>
using namespace std;
//1 генерация поля
//2 вывод поля на экран
//3 ввод команды
//4 открытие клеток
//5 завершение игры



int field[100][100];
// -1 - mine
// 0-8 - cell
int field_p[100][100];
// -2 - closed
// -1 - flag
// 0-8 - opened
const int N = 5, M = 5, K = 5;

void print_field() { // функция вывода поля
	system("cls");
	cout << "  ";
	for (int i = 0; i < M; ++i) {
		cout << " " << i + 1;
		if (i + 1 < 10) cout << ' ';
	}
	cout << endl;
	for (int i = 0; i < N; i++) { // вывод поля для ИГРОКА
		cout << i + 1 << " ";
		if (i + 1 < 10) cout << ' ';
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] == -2) cout << "_  ";
			else if (field_p[i][j] == -1) cout << "f  ";
			else if (field_p[i][j] == -3) cout << "*  ";
			else cout << field_p[i][j] << "  ";
		}
		cout << endl;
	}
}

void dfs(int x, int y) {
	if (x < 0 || x >= N || y < 0 || y >= M || field[x][y] > -2) return;
	field_p[x][y] = field[x][y];
	if (field[x][y] > 0) return;
	for (int i2 = x - 1; i2 <= x + 1; ++i2) {
		for (int j2 = y - 1; j2 <= y + 1; ++j2) {
			if (i2 != x || j2 != y) dfs(i2, j2);
		}
	}
}

// true - не взорвались
bool open_cell(int x, int y) { // функция открытия клетки
	if (field[x][y] == -1) return false;
	if (field[x][y] > 0) {
		field_p[x][y] = field[x][y];
		return true;
	}
	dfs(x, y); 
}

bool is_win() {
	int opened = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] >= 0) opened++;
		}
	}
	return (N * M - K == opened);
}

void end_game(bool s_win = false) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (field_p[i][j] == -1)field_p[i][j] = -2;
			if (field[i][j] == -1) field_p[i][j] = -3;
		}
	}
	print_field();
	cout << "YOU " << (is_win ? "LOSE!" : "WIN!") << "\ndlya novoi igri vvedi lubuyu stroku!";
	string s;
	cin >> s;
}
int main()
{
	while (true) {
		srand(time(0)); // счетчик для ранодмных чисел (rand())
		for (int i = 0; i < N; i++) { // заполнение поля нулями
			for (int j = 0; j < M; j++) {
				field[i][j] = 0;
				field_p[i][j] = -2;
			}
		}
		for (int i = 0; i < K; ++i) { // генерация мин на поле
			while (true) {
				int x = rand() % N;
				int y = rand() % M;
				if (field[x][y] != -1) {
					field[x][y] = -1;
					break;
				}
			}
		}
		for (int i = 0; i < N; i++) { // обозначение кол-ва мин вокруг клетки
			for (int j = 0; j < M; j++) {
				if (field[i][j] != -1) {
					field[i][j] = 0;
					for (int i2 = i - 1; i2 <= i + 1; ++i2) {
						for (int j2 = j - 1; j2 <= j + 1; ++j2) {
							if (i2 >= 0 && i2 < N && j2 >= 0 && j2 <= M &&
								field[i2][j2] == -1) ++field[i][j];
						}
					}
				}
			}
		}
		while (true) {
			print_field();
			cout << "Enter comand:";
				/*
				\o x y - открыть клетку
				\f x y - установка флага
				\n - новая игра
				\q - выход из игры
				*/
			string comand;
			cin >> comand;
			if (comand == "\\o") {
				//ввод координат
				int x, y;
				cin >> x >> y;
				--x; --y;
				// откроем клетки
				if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] >= 0) continue;
				if (!open_cell(x, y)) {
					end_game();
					break;
				}
				if (is_win()) {
					end_game(true);
					break;

				}
			}
			else if (comand == "\\f") {
				//ввод координат
				int x, y;
				cin >> x >> y;
				--x; --y;
				if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] >= 0) continue;
				// поставим флаг
				if (field_p[x][y] == -1) field_p[x][y] = -2;
				else field_p[x][y] = -1;
			}
			else if (comand == "\\n") {
				break;
			}
			else if (comand == "\\q") {
				return 0;

			}
		} 
	}
}
