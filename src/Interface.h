//
// Created by Jiarong on 2023/8/22.
//

#ifndef GAMETOWER_INTERFACE_H
#define GAMETOWER_INTERFACE_H

#include "Word.h"
#include <time.h>
//#include "Monster.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <random>
//#include "utils\\rand.h"

using std::cout, std::cin, std::endl, std::ifstream, std::string;



namespace PosControl {

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    // 窗口长宽
    struct Size {
        int width;
        int height;
    } size = {1020, 860};

    void centerWindow() {
        HWND hwnd = GetForegroundWindow();
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, size.width, size.height, 1);
        MoveWindow(hwnd, (screen_width - size.width) / 2, (screen_height - size.height) / 2, size.width, size.height,
                   1);
    }

// 回到坐标位置，坐标需要给定
    void setPos(int x, int y) {
        COORD coord{static_cast<SHORT>(y), static_cast<SHORT>(x)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);  //回到给定的坐标位置进行重新输出
    }


// 获取当前标准输出流位置
    void getPos(int &x, int &y) {
        CONSOLE_SCREEN_BUFFER_INFO b;           // 包含控制台屏幕缓冲区的信息
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &b);    //  获取标准输出句柄
        y = b.dwCursorPosition.X;
        x = b.dwCursorPosition.Y;
    }


    void HideCursor() {
        CONSOLE_CURSOR_INFO cursor;
        cursor.bVisible = FALSE;
        cursor.dwSize = sizeof(cursor);
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorInfo(handle, &cursor);
    }
}

void setDPI() {
    system("SetDpi.exe value > currDPI.txt");
    system("SetDpi.exe 150");
}

struct Menu {
    string name;
};

int randInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void welcomePage() {

    PosControl::centerWindow();
    PosControl::HideCursor();
    cout << "按下 \033[31m[Tab键] \033[0m跳过本段..." << endl << endl;
    ifstream welcomeFile("./Assets/.welcomePage");
    char welcome;
    bool flagTab = false;
    while (welcomeFile.get(welcome)) {
        if(kbhit()) {
            char c;
            c = getch();
            if (c == '\t') {
                flagTab = true;
            }
        }
        if (!flagTab) Sleep(50);
        cout << welcome;
    }
    welcomeFile.close();

    cout << endl << endl;
    for (int i = 0; i < 103; i++) {
        Sleep(5);
        cout << "=";
    }
    cout << endl << endl;

    int x, y;
    PosControl::getPos(x, y);
    int spaceLength = std::max((PosControl::size.width / 10 - 88) / 2 + 2, 0);
    string space(spaceLength, ' ');
    while (1) {
        PosControl::setPos(x, y);
        ifstream logoFile("./Assets/.logo");
        string logo;
        while (getline(logoFile, logo)) {
            int color = randInt(1, 15);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            Sleep(50);
            cout << space << logo << endl;
        }
        logoFile.close();

        cout << endl << "\033[0m按下 \033[31m[Enter键] \033[0m继续..." << endl;

        if(kbhit()){
            char c;
            c = getch();
            if (c == '\r' || c == '\n') {
                PosControl::setPos(x+7, 0);
                break;
            }
        }
    }
    cout << "=======================================================================================================" << endl;
    cout << endl << "    \033[0m请使用 \033[31m[Ctrl] \033[0m+ \033[31m滚轮 \033[0m调整控制台字体大小，确保以上分割线显示在同一行。" << endl;
    cout << endl << "\033[0m按下 \033[31m[Enter键] \033[0m继续..." << endl;
    while (1) {
        if(kbhit()){
            char c;
            c = getch();
            if (c == '\r' || c == '\n') {
                PosControl::setPos(x+8, 0);
                cout << "                                                                                        " << endl;
                cout << "                                                                                        " << endl;
                cout << "                                                                                        " << endl;
                cout << "                                                                                        " << endl;
                PosControl::setPos(x+9, 0);
                break;
            }
        }
    }
}

int switcher(Menu menu[], int length) {

    int x, y;

    cout << "使用 \033[31m[W] [S] \033[0m或 \033[31m[↑] [↓] \033[0m选择，按下 \033[31m[Enter键] \033[0m确认。" << endl << endl;

    PosControl::getPos(x, y);
    PosControl::setPos(x,0);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    for (int i = 0; i < length; i++) {
        cout << "    > " << menu[i].name << endl;
    }
    cout << endl;
    cout << "=======================================================================================================" << endl;

    char c;
    int choice = 0;
    int prevChoice = 0;

    while (1) {

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        PosControl::setPos(x + prevChoice, y);
        cout << "    > " << menu[prevChoice].name;

        PosControl::setPos(x + choice, y);
        cout << "    \33[43;37m> " << menu[choice].name << "\33[0m";
        c = getch();
        switch (c) {
            case 'w':
            case 'W':
            case 72: // 72是上箭头
                prevChoice = choice;
                choice = (choice + length - 1) % length;
                break;
            case 's':
            case 'S':
            case 80: // 80是下箭头
                prevChoice = choice;
                choice = (choice + 1) % length;
                break;
            case '\r':
            case '\n':
                return choice;
            default:
                prevChoice = choice;
                break;
        }
    }
}

void goodbye() {
    system("cls");
    ifstream creditsLogoFile("./Assets/.credit_logo");
    string logo;
    while (getline(creditsLogoFile, logo)) {
        cout << logo << endl;
        Sleep(randInt(20, 50));
    }
}

#endif //GAMETOWER_INTERFACE_H
