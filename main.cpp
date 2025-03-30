#include <iostream>
#include <math.h>
#include <windows.h>

using namespace std;

const float theta_spacing = 0.07; // step size for the angle that runs around the tube of the torus
const float phi_spacing   = 0.02; // step size for the angle that rotates around the center of the torus

const int screen_width = 35;
const int screen_height = 25;

const float R1 = 1; // The radius of the tube
const float R2 = 2; // The distance from the center of the torus to the center of the tube
const float K2 = 10; // This is a constant that acts as a depth offset in your 3D to 2D projection. It ensures the donut is positioned in front of the viewer.

const float K1 = screen_width*K2*3/(8*(R1+R2)); // Scales the projected coordinates to fit your terminal’s dimensions.

const char pixels[] = {'.',',','-','~',':',';','=','!','*','#','$','@'};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorHome = {0, 0};

int main() 
{
    // These variables represent the rotation angles of the donut in 3D space. Incrementing them in the loop rotates the donut.
    float A = 4;
    float B = 2; 
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    system("cls");

    while(true) {
        float cosA = cos(A), sinA = sin(A);
        float cosB = cos(B), sinB = sin(B);

        char output[screen_height][screen_width];
        float z_buffer[screen_height][screen_width];

        for (int y = 0; y < screen_height; y++) {
            for (int x = 0; x < screen_width; x++) {
                output[y][x] = ' ';
                z_buffer[y][x] = 0;
            }
        }

        for(float theta = 0; theta < 2*M_PI; theta += theta_spacing) {
            for(float phi = 0; phi < 2*M_PI; phi += phi_spacing) {

                float cosTheta = cos(theta);
                float sinTheta = sin(theta);
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);
                float circleX = R2 + R1*cosTheta;
                float circleY = R1*sinTheta;

                float x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
                float y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;
                float z = cosA * circleX * sinPhi + circleY * sinA + K2;
                
                float ooz = 1/z;

                int xp = (int)(screen_width / 2 + K1 * ooz * x);
                int yp = (int)(screen_height / 2 - K1 * ooz * y);
                int idx = xp + yp * screen_width;

                
                float L = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - sinA * sinTheta + cosB*(cosA*sinTheta-cosTheta*sinA*sinPhi);
                if (xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
                    if (ooz > z_buffer[yp][xp]) {
                        z_buffer[yp][xp] = ooz;
                        int luminance_index = (int)(L * 8);
                        if (luminance_index < 0) luminance_index = 0;
                        if (luminance_index > 11) luminance_index = 11;
                        output[yp][xp] = ".,-~:;=!*#$@"[luminance_index];
                    }
                }  
            }
        }

        for(int j = 0; j < screen_height; j++) {
            for(int i = 0; i < screen_width; i++) {
                putchar(output[j][i]);
            }
            cout << "\n";
        }

        A += 0.2;
        B += 0.2;

        Sleep(50);
        SetConsoleCursorPosition(hConsole, cursorHome);
    }
    return 0;
}