#include <iostream>
#include <math.h>

using namespace std;

const float theta_spacing = 0.07;
const float phi_spacing   = 0.02;

const int screen_width = 50;
const int screen_height = 50;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;

const float K1 = screen_width*K2*3/(8*(R1+R2));

const char pixels[] = {'.',',','-','~',':',';','=','!','*','#','$','@'};


int main() 
{
    float A = 0.8;
    float B = 0.5; 
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[screen_width * screen_height];
    float z_buffer[screen_width * screen_height];

    for (int i = 0; i < screen_width * screen_height; i++) {
        output[i] = ' ';
        z_buffer[i] = 0;
    }

    for(float theta = 0; theta < 2*M_PI; theta += theta_spacing) {
        for(float phi = 0; phi < 2*M_PI; phi += phi_spacing) {

            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            float cosPhi = cos(phi);
            float sinPhi = sin(phi);
            float circleX = R2 + R1*cosTheta;
            float circleY = R1*sinTheta;

            /* float x = circleX*(cos(screen_height)*cosPhi + sin(screen_width)*sin(screen_height)*sinPhi)- circleY*cos(screen_width)*sin(screen_height); 
            float y = circleX*(sin(screen_height)*cosPhi - sin(screen_width*cos(screen_height)*sinPhi) + circleY*cos(screen_width)*cos(screen_height));
            float z = K2 + cos(screen_width)*circleX*sinPhi + circleY*sin(screen_width); */

            float x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
            float y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * cosB;
            float z = cosA * circleX * sinPhi + circleY * sinA + K2;


            float ooz = 1/z;
            float L = cosTheta * cosPhi;
            if(L < 0) L = 0;

            int xp = (int)(screen_width / 2 + K1 * ooz * x);
            int yp = (int)(screen_height / 2 - K1 * ooz * y);
            int idx = xp + yp * screen_width;

            if (idx >= 0 && idx < screen_width * screen_height) {
                if (ooz > z_buffer[idx]) {
                    z_buffer[idx] = ooz;
                    int luminance_index = (int)(L * 11);
                    if(luminance_index < 0) luminance_index = 0;
                    if(luminance_index > 11) luminance_index = 11;
                    output[idx] = pixels[luminance_index];
                }
            }
        }
    }

    for(int j = 0; j < screen_height; j++) {
        for(int i = 0; i < screen_width; i++) {
            cout << output[i+j*screen_width];
        }
        cout << "\n";
    }
    return 0;
}