#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

HANDLE openSerial(const char* portName, DWORD baudRate) {
    HANDLE hSerial = CreateFileA(portName,
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port! Error code: " << GetLastError() << std::endl;
        return nullptr;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb);
    dcb.BaudRate = baudRate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    SetCommState(hSerial, &dcb);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    return hSerial;
}

void moveMouse(int dx, int dy) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}

void leftClick() {
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, input, sizeof(INPUT));
}

void rightClick() {
    INPUT input[2] = {};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(2, input, sizeof(INPUT));
}

int main() {
    HANDLE hSerial = openSerial("COM4", CBR_115200);
    if (!hSerial) return 1;

    char buffer[256];
    DWORD bytesRead;
    std::string serialBuffer;

    std::cout << "Reading Arduino data..." << std::endl;

    float avgZ = 0;
    const int N = 200;
    int count = 0;
    bool zActive = false;
    DWORD lastClickTime = 0;

    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer)-1, &bytesRead, nullptr) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            serialBuffer += buffer;

            size_t pos;
            while ((pos = serialBuffer.find('\n')) != std::string::npos) {
                std::string line = serialBuffer.substr(0, pos);
                serialBuffer.erase(0, pos + 1);

                std::istringstream ss(line);
                std::string token;
                float accelX = 0, accelY = 0, accelZ = 0;

                if (std::getline(ss, token, ',')) accelX = std::stof(token);
                if (std::getline(ss, token, ',')) accelY = std::stof(token);
                if (std::getline(ss, token, ',')) accelZ = std::stof(token);

                avgZ = (avgZ * count + accelZ) / (count + 1);
                count++;
                if (count >= N) { count = 0; avgZ = accelZ; }

                float diffZ = accelZ - avgZ;
                int dx = static_cast<int>(accelX * 4);
                int dy = static_cast<int>(-accelY * 4);

                DWORD now = GetTickCount();
                if (!zActive && diffZ >= 1.0f && (now - lastClickTime > 400)) {
                    leftClick();
                    lastClickTime = now;
                    zActive = true;
                } else if (!zActive && diffZ <= -1.0f && (now - lastClickTime > 400)) {
                    rightClick();
                    lastClickTime = now;
                    zActive = true;
                } else if (zActive && std::fabs(diffZ) < 0.2f) {
                    zActive = false;
                }

                moveMouse(dx, dy);
            }
        }
    }

    CloseHandle(hSerial);
    return 0;
}
