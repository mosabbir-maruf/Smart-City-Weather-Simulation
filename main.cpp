#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float carX[5]={-1,-0.7,-0.3,0.2,0.6};
float cloud[4]={-0.9,-0.3,0.2,0.7};
float birdX=-1.0;
float personX=-0.8;

bool rain=false;
bool night=false;

struct Rain{
float x;
float y;
};

Rain drop[300];

float starX[100];
float starY[100];

void circle(float cx,float cy,float r){
glBegin(GL_POLYGON);
for(int i=0;i<360;i++){
float t=i*3.1416/180;
glVertex2f(cx+r*cos(t),cy+r*sin(t));
}

glEnd();
}
void gradientSky(){

glBegin(GL_QUADS);

// top sky
if(night)
glColor3f(0.02,0.02,0.15);
else
glColor3f(0.2,0.45,0.9);

glVertex2f(-1,1);
glVertex2f(1,1);


// bottom sky
if(night)
glColor3f(0.05,0.05,0.2);
else
glColor3f(0.75,0.85,1);

glVertex2f(1,-1);
glVertex2f(-1,-1);

glEnd();

}

void sun(){

glColor3f(1,0.9,0.2);
circle(0.8,0.75,0.07);

glColor3f(1,0.7,0);

for(int i=0;i<16;i++){
float a=i*22.5*3.1416/180;

glBegin(GL_LINES);
glVertex2f(0.8+0.08*cos(a),0.75+0.08*sin(a));
glVertex2f(0.8+0.15*cos(a),0.75+0.15*sin(a));
glEnd();
}
}

void moon(){
glColor3f(1,1,1);
circle(0.8,0.75,0.06);
}

void stars(){
glColor3f(1,1,1);
glBegin(GL_POINTS);
for(int i=0;i<100;i++)
glVertex2f(starX[i],starY[i]);
glEnd();
}

void cloudDraw(float x,float y){

// cloud color change during rain
if(rain)
glColor3f(0.45,0.45,0.45);   // dark rain cloud
else
glColor3f(0.85,0.85,0.85);

circle(x+0.03,y-0.02,0.05);
circle(x+0.09,y-0.02,0.05);
circle(x+0.15,y-0.02,0.05);


// main cloud
if(rain)
glColor3f(0.55,0.55,0.55);
else
glColor3f(1,1,1);
circle(x,y,0.05);
circle(x+0.05,y+0.02,0.06);
circle(x+0.1,y+0.03,0.06);
circle(x+0.15,y+0.01,0.05);


// top fluffy layer
circle(x+0.04,y+0.05,0.04);
circle(x+0.1,y+0.06,0.04);

}

void bird(float x,float y){

glColor3f(0,0,0);

// left wing (curved V shape)
glBegin(GL_LINE_STRIP);
glVertex2f(x,y);
glVertex2f(x+0.015,y+0.015);
glVertex2f(x+0.03,y);
glEnd();

// right wing
glBegin(GL_LINE_STRIP);
glVertex2f(x+0.03,y);
glVertex2f(x+0.045,y+0.015);
glVertex2f(x+0.06,y);
glEnd();

// body
glPointSize(3);
glBegin(GL_POINTS);
glVertex2f(x+0.03,y);
glEnd();

}

void grass(){

// grass behind buildings
glColor3f(0.35,0.75,0.35);

glBegin(GL_QUADS);
glVertex2f(-1,-0.6);
glVertex2f(1,-0.6);
glVertex2f(1,-0.3);
glVertex2f(-1,-0.3);
glEnd();

}

void road(){

glColor3f(0.4,0.4,0.4);

glBegin(GL_QUADS);
glVertex2f(-1,-0.6);
glVertex2f(1,-0.6);
glVertex2f(1,-0.9);
glVertex2f(-1,-0.9);
glEnd();

glColor3f(1,1,1);

for(float i=-0.9;i<0.9;i+=0.25){

glBegin(GL_QUADS);
glVertex2f(i,-0.75);
glVertex2f(i+0.12,-0.75);
glVertex2f(i+0.12,-0.78);
glVertex2f(i,-0.78);
glEnd();

}
}

void zebra(){

glColor3f(1,1,1);

// zebra crossing directly in front of traffic light

for(float y=-0.62;y>-0.88;y-=0.07){

glBegin(GL_QUADS);

glVertex2f(0.50,y);
glVertex2f(0.70,y);
glVertex2f(0.70,y-0.025);
glVertex2f(0.50,y-0.025);

glEnd();

}

}

void building(float x,float h){

// building color
if(x < -0.8)
glColor3f(0.6,0.75,0.95);

else if(x < -0.6)
glColor3f(0.85,0.7,0.9);

else if(x < 0.3)
glColor3f(0.7,0.85,0.7);

else
glColor3f(0.95,0.75,0.6);


// building body
glBegin(GL_QUADS);
glVertex2f(x,-0.45);
glVertex2f(x+0.2,-0.45);
glVertex2f(x+0.2,h);
glVertex2f(x,h);
glEnd();


// window color day/night
if(night)
glColor3f(1.0,0.9,0.4);
else
glColor3f(0.25,0.45,0.75);

for(float y=-0.40;y<h;y+=0.1){
for(float i=x+0.03;i<x+0.17;i+=0.06){

glBegin(GL_QUADS);

glVertex2f(i,y);
glVertex2f(i+0.035,y);
glVertex2f(i+0.035,y+0.05);
glVertex2f(i,y+0.05);

glEnd();

}
}

}
void school(){

float s=-0.75;   // school left side position


// main center building
glColor3f(0.90,0.40,0.35);

glBegin(GL_QUADS);
glVertex2f(-0.05+s,-0.45);
glVertex2f(0.23+s,-0.45);
glVertex2f(0.23+s,0.28);
glVertex2f(-0.05+s,0.28);
glEnd();


// left wing
glColor3f(0.92,0.55,0.45);

glBegin(GL_QUADS);
glVertex2f(-0.28+s,-0.45);
glVertex2f(-0.05+s,-0.45);
glVertex2f(-0.05+s,0.18);
glVertex2f(-0.28+s,0.18);
glEnd();


// right wing
glBegin(GL_QUADS);
glVertex2f(0.23+s,-0.45);
glVertex2f(0.46+s,-0.45);
glVertex2f(0.46+s,0.18);
glVertex2f(0.23+s,0.18);
glEnd();


// roof frame
glColor3f(0.85,0.85,0.85);

glBegin(GL_QUADS);
glVertex2f(-0.30+s,0.18);
glVertex2f(0.48+s,0.18);
glVertex2f(0.43+s,0.24);
glVertex2f(-0.25+s,0.24);
glEnd();


// entrance
glColor3f(0.95,0.95,0.95);

glBegin(GL_QUADS);
glVertex2f(0.03+s,-0.45);
glVertex2f(0.15+s,-0.45);
glVertex2f(0.15+s,0.10);
glVertex2f(0.03+s,0.10);
glEnd();


// entrance pillars
glColor3f(0.92,0.92,0.92);

for(float x=0.05;x<=0.13;x+=0.04){

glBegin(GL_QUADS);
glVertex2f(x+s,-0.45);
glVertex2f(x+s+0.015,-0.45);
glVertex2f(x+s+0.015,0.10);
glVertex2f(x+s,0.10);
glEnd();

}


// door
glColor3f(0.20,0.45,0.85);

glBegin(GL_QUADS);
glVertex2f(0.08+s,-0.45);
glVertex2f(0.11+s,-0.45);
glVertex2f(0.11+s,-0.20);
glVertex2f(0.08+s,-0.20);
glEnd();


// windows glass day / night
if(night)
glColor3f(1.0,0.9,0.4);   // night light
else
glColor3f(0.45,0.80,0.98); // day glass

for(float y=-0.02;y<=0.10;y+=0.12){
for(float x=-0.25;x<=0.36;x+=0.18){

if(x>-0.02 && x<0.20) continue;

glBegin(GL_QUADS);

glVertex2f(x+s,y);
glVertex2f(x+s+0.08,y);
glVertex2f(x+s+0.08,y+0.08);
glVertex2f(x+s,y+0.08);

glEnd();

}
}


// window frames
glColor3f(0.95,0.95,0.95);

for(float y=-0.02;y<=0.10;y+=0.12){
for(float x=-0.25;x<=0.36;x+=0.18){

if(x>-0.02 && x<0.20) continue;

glBegin(GL_LINE_LOOP);

glVertex2f(x+s,y);
glVertex2f(x+s+0.08,y);
glVertex2f(x+s+0.08,y+0.08);
glVertex2f(x+s,y+0.08);

glEnd();

}
}


// SCHOOL board (centered properly)
glColor3f(0.75,0.10,0.10);

glBegin(GL_QUADS);
glVertex2f(0.02+s,0.24);
glVertex2f(0.16+s,0.24);
glVertex2f(0.16+s,0.30);
glVertex2f(0.02+s,0.30);
glEnd();


// SCHOOL text centered
glColor3f(1,1,1);

glRasterPos2f(0.04+s,0.262);

char text[]="SCHOOL";

for(int i=0;i<6;i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
}

void bank(){

float s=-0.05;   // position between school and police station


// main building (cherry color)
glColor3f(0.72,0.12,0.22);

glBegin(GL_QUADS);
glVertex2f(-0.08+s,-0.45);
glVertex2f(0.20+s,-0.45);
glVertex2f(0.20+s,0.30);
glVertex2f(-0.08+s,0.30);
glEnd();


// top triangle roof
glColor3f(0.55,0.05,0.15);

glBegin(GL_TRIANGLES);
glVertex2f(-0.10+s,0.30);
glVertex2f(0.22+s,0.30);
glVertex2f(0.06+s,0.40);
glEnd();


// roof line
glColor3f(0.60,0.08,0.18);

glBegin(GL_QUADS);
glVertex2f(-0.10+s,0.28);
glVertex2f(0.22+s,0.28);
glVertex2f(0.20+s,0.30);
glVertex2f(-0.08+s,0.30);
glEnd();


// entrance base
glColor3f(0.90,0.75,0.80);

glBegin(GL_QUADS);
glVertex2f(0.00+s,-0.45);
glVertex2f(0.12+s,-0.45);
glVertex2f(0.12+s,0.10);
glVertex2f(0.00+s,0.10);
glEnd();


// pillars
glColor3f(0.95,0.85,0.88);

for(float x=0.01;x<=0.10;x+=0.03){

glBegin(GL_QUADS);
glVertex2f(x+s,-0.45);
glVertex2f(x+s+0.015,-0.45);
glVertex2f(x+s+0.015,0.10);
glVertex2f(x+s,0.10);
glEnd();

}


// door
glColor3f(0.35,0.05,0.10);

glBegin(GL_QUADS);
glVertex2f(0.045+s,-0.45);
glVertex2f(0.075+s,-0.45);
glVertex2f(0.075+s,-0.20);
glVertex2f(0.045+s,-0.20);
glEnd();


// signboard
glColor3f(0.45,0.02,0.10);

glBegin(GL_QUADS);
glVertex2f(-0.02+s,0.22);
glVertex2f(0.14+s,0.22);
glVertex2f(0.14+s,0.27);
glVertex2f(-0.02+s,0.27);
glEnd();


// text
glColor3f(1,1,1);
glRasterPos2f(0.015+s,0.235);

char text[]="BANK";

for(int i=0;i<4;i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);

}

void policeStation(){

float s=0.55;   // right side position


// main building
glColor3f(0.20,0.30,0.45);

glBegin(GL_QUADS);
glVertex2f(-0.05+s,-0.45);
glVertex2f(0.23+s,-0.45);
glVertex2f(0.23+s,0.28);
glVertex2f(-0.05+s,0.28);
glEnd();


// side wings
glColor3f(0.15,0.25,0.40);

glBegin(GL_QUADS);
glVertex2f(-0.25+s,-0.45);
glVertex2f(-0.05+s,-0.45);
glVertex2f(-0.05+s,0.15);
glVertex2f(-0.25+s,0.15);
glEnd();

glBegin(GL_QUADS);
glVertex2f(0.23+s,-0.45);
glVertex2f(0.43+s,-0.45);
glVertex2f(0.43+s,0.15);
glVertex2f(0.23+s,0.15);
glEnd();


// roof
glColor3f(0.10,0.15,0.25);

glBegin(GL_QUADS);
glVertex2f(-0.27+s,0.15);
glVertex2f(0.45+s,0.15);
glVertex2f(0.40+s,0.22);
glVertex2f(-0.22+s,0.22);
glEnd();


// police tower
glColor3f(0.25,0.35,0.55);

glBegin(GL_QUADS);
glVertex2f(0.06+s,0.28);
glVertex2f(0.12+s,0.28);
glVertex2f(0.12+s,0.45);
glVertex2f(0.06+s,0.45);
glEnd();


// tower light
glColor3f(1,0,0);
circle(0.09+s,0.47,0.015);


// entrance
glColor3f(0.85,0.85,0.9);

glBegin(GL_QUADS);
glVertex2f(0.05+s,-0.45);
glVertex2f(0.13+s,-0.45);
glVertex2f(0.13+s,0.08);
glVertex2f(0.05+s,0.08);
glEnd();


// door
glColor3f(0.10,0.20,0.45);

glBegin(GL_QUADS);
glVertex2f(0.075+s,-0.45);
glVertex2f(0.105+s,-0.45);
glVertex2f(0.105+s,-0.18);
glVertex2f(0.075+s,-0.18);
glEnd();


// windows (aligned properly)
if(night)
glColor3f(1.0,0.9,0.4);
else
glColor3f(0.35,0.70,0.95);

for(float y=-0.05;y<=0.07;y+=0.12){
for(float x=-0.23;x<=0.35;x+=0.18){

if(x>-0.01 && x<0.19) continue;

glBegin(GL_QUADS);
glVertex2f(x+s,y);
glVertex2f(x+s+0.07,y);
glVertex2f(x+s+0.07,y+0.07);
glVertex2f(x+s,y+0.07);
glEnd();

}
}


// window frames
glColor3f(1,1,1);

for(float y=-0.05;y<=0.07;y+=0.12){
for(float x=-0.23;x<=0.35;x+=0.18){

if(x>-0.01 && x<0.19) continue;

glBegin(GL_LINE_LOOP);
glVertex2f(x+s,y);
glVertex2f(x+s+0.07,y);
glVertex2f(x+s+0.07,y+0.07);
glVertex2f(x+s,y+0.07);
glEnd();

}
}


// POLICE signboard
glColor3f(0.05,0.10,0.35);

glBegin(GL_QUADS);
glVertex2f(0.02+s,0.24);
glVertex2f(0.17+s,0.24);
glVertex2f(0.17+s,0.29);
glVertex2f(0.02+s,0.29);
glEnd();


// text
glColor3f(1,1,1);
glRasterPos2f(0.04+s,0.255);

char text[]="POLICE";

for(int i=0;i<6;i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);

}

void tree(float x){

// trunk
glColor3f(0.35,0.18,0.05);

glBegin(GL_QUADS);
glVertex2f(x,-0.60);
glVertex2f(x+0.03,-0.60);
glVertex2f(x+0.03,-0.48);
glVertex2f(x,-0.48);
glEnd();


// leaves bottom
glColor3f(0.1,0.6,0.2);
circle(x+0.015,-0.45,0.06);

// leaves middle
glColor3f(0.1,0.7,0.2);
circle(x+0.015,-0.40,0.05);

// leaves top
glColor3f(0.1,0.8,0.2);
circle(x+0.015,-0.35,0.04);

}


void car(float x,float r,float g,float b){

// car body
glColor3f(r,g,b);

glBegin(GL_POLYGON);
glVertex2f(x,-0.72);
glVertex2f(x+0.15,-0.72);
glVertex2f(x+0.17,-0.69);
glVertex2f(x+0.14,-0.66);
glVertex2f(x+0.05,-0.66);
glVertex2f(x+0.02,-0.69);
glEnd();


// windshield
glColor3f(0.7,0.9,1);

glBegin(GL_QUADS);
glVertex2f(x+0.06,-0.66);
glVertex2f(x+0.12,-0.66);
glVertex2f(x+0.11,-0.64);
glVertex2f(x+0.07,-0.64);
glEnd();


// front light
glColor3f(1,1,0.6);
circle(x+0.165,-0.69,0.01);


// back light
glColor3f(1,0.2,0.2);
circle(x+0.01,-0.69,0.01);


// wheels
glColor3f(0,0,0);
circle(x+0.04,-0.73,0.02);
circle(x+0.13,-0.73,0.02);


// wheel rim
glColor3f(0.7,0.7,0.7);
circle(x+0.04,-0.73,0.01);
circle(x+0.13,-0.73,0.01);

}
void policeCar(float x){

// car body
glColor3f(0.05,0.05,0.10);

glBegin(GL_POLYGON);
glVertex2f(x,-0.58);
glVertex2f(x+0.16,-0.58);
glVertex2f(x+0.18,-0.55);
glVertex2f(x+0.15,-0.52);
glVertex2f(x+0.05,-0.52);
glVertex2f(x+0.02,-0.55);
glEnd();


// white door part
glColor3f(1,1,1);

glBegin(GL_QUADS);
glVertex2f(x+0.05,-0.57);
glVertex2f(x+0.13,-0.57);
glVertex2f(x+0.13,-0.53);
glVertex2f(x+0.05,-0.53);
glEnd();


// windshield
glColor3f(0.6,0.85,1);

glBegin(GL_QUADS);
glVertex2f(x+0.06,-0.52);
glVertex2f(x+0.12,-0.52);
glVertex2f(x+0.11,-0.50);
glVertex2f(x+0.07,-0.50);
glEnd();


// police light bar
glColor3f(1,0,0);
circle(x+0.08,-0.49,0.008);

glColor3f(0,0,1);
circle(x+0.10,-0.49,0.008);


// front light
glColor3f(1,1,0.7);
circle(x+0.175,-0.55,0.01);


// back light
glColor3f(1,0.2,0.2);
circle(x+0.01,-0.55,0.01);


// wheels
glColor3f(0,0,0);
circle(x+0.05,-0.60,0.02);
circle(x+0.13,-0.60,0.02);


// wheel rim
glColor3f(0.7,0.7,0.7);
circle(x+0.05,-0.60,0.01);
circle(x+0.13,-0.60,0.01);


// POLICE text
glColor3f(0,0,0);
glRasterPos2f(x+0.065,-0.555);

char text[]="POLICE";

for(int i=0;i<6;i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,text[i]);

}

void schoolBus(float x){

// bus body
glColor3f(1.0,0.8,0.0);

glBegin(GL_QUADS);
glVertex2f(x,-0.60);
glVertex2f(x+0.32,-0.60);
glVertex2f(x+0.32,-0.50);
glVertex2f(x,-0.50);
glEnd();


// front slope
glBegin(GL_POLYGON);
glVertex2f(x+0.32,-0.60);
glVertex2f(x+0.37,-0.57);
glVertex2f(x+0.37,-0.53);
glVertex2f(x+0.32,-0.50);
glEnd();


// windows
glColor3f(0.2,0.3,0.5);

for(float i=0.03;i<0.30;i+=0.06){

glBegin(GL_QUADS);
glVertex2f(x+i,-0.51);
glVertex2f(x+i+0.04,-0.51);
glVertex2f(x+i+0.04,-0.55);
glVertex2f(x+i,-0.55);
glEnd();

}


// door
glColor3f(0.15,0.15,0.15);

glBegin(GL_QUADS);
glVertex2f(x+0.27,-0.60);
glVertex2f(x+0.31,-0.60);
glVertex2f(x+0.31,-0.50);
glVertex2f(x+0.27,-0.50);
glEnd();


// wheels
glColor3f(0,0,0);
circle(x+0.08,-0.62,0.022);
circle(x+0.28,-0.62,0.022);


// wheel rim
glColor3f(0.8,0.8,0.8);
circle(x+0.08,-0.62,0.010);
circle(x+0.28,-0.62,0.010);


// headlight
glColor3f(1,1,0.7);
circle(x+0.36,-0.56,0.01);


// SCHOOL BUS text
glColor3f(0,0,0);
glRasterPos2f(x+0.10,-0.585);

char text[]="SCHOOL BUS";

for(int i=0;i<10;i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text[i]);

}




void trafficLight(){

glColor3f(0.2,0.2,0.2);

glBegin(GL_QUADS);
glVertex2f(0.6,-0.6);
glVertex2f(0.62,-0.6);
glVertex2f(0.62,-0.4);
glVertex2f(0.6,-0.4);
glEnd();

glColor3f(1,0,0);
circle(0.61,-0.42,0.015);

glColor3f(1,1,0);
circle(0.61,-0.47,0.015);

glColor3f(0,1,0);
circle(0.61,-0.52,0.015);
}

void streetLight(float x){

// pole
glColor3f(0.3,0.3,0.3);

glBegin(GL_QUADS);
glVertex2f(x,-0.6);
glVertex2f(x+0.02,-0.6);
glVertex2f(x+0.02,-0.30);
glVertex2f(x,-0.30);
glEnd();


// arm
glBegin(GL_QUADS);
glVertex2f(x+0.02,-0.32);
glVertex2f(x+0.08,-0.32);
glVertex2f(x+0.08,-0.30);
glVertex2f(x+0.02,-0.30);
glEnd();


// light bulb
if(night)
glColor3f(1.0,0.95,0.6);   // glowing light
else
glColor3f(0.6,0.6,0.6);    // off light

circle(x+0.08,-0.33,0.015);

}

void rainDraw(){

glColor3f(0.7,0.7,1);

glBegin(GL_LINES);

for(int i=0;i<300;i++){

glVertex2f(drop[i].x,drop[i].y);
glVertex2f(drop[i].x+0.02,drop[i].y-0.06);

}

glEnd();
}

void rainSplash(){

glColor3f(0.8,0.8,1.0);

for(int i=0;i<80;i++){

float x=(rand()%200-100)/100.0;
float y=-0.88 + (rand()%10)/100.0;

glBegin(GL_LINES);

glVertex2f(x,y);
glVertex2f(x-0.01,y+0.01);

glVertex2f(x,y);
glVertex2f(x+0.01,y+0.01);

glEnd();

}

}

void display(){

glClear(GL_COLOR_BUFFER_BIT);
gradientSky();
grass();
road();
zebra();
school();
bank();
policeStation();
policeCar(0.75);
schoolBus(-0.95);


tree(-0.78);  // building 1 এর পাশে
tree(-0.53);  // building 2 এর পাশে
tree(0.05);   // hospital এর পাশে
tree(0.32);   // building 4 এর পাশে
tree(0.58);   // building 5 এর পাশে

streetLight(-0.85);
streetLight(-0.40);
streetLight(0.10);
streetLight(0.45);



trafficLight();

if(night){
moon();
stars();
}
else
sun();
if(!night){

bird(birdX,0.80);
bird(birdX+0.18,0.76);
bird(birdX+0.36,0.82);
bird(birdX+0.55,0.78);
}

for(int i=0;i<4;i++)
cloudDraw(cloud[i],0.8-i*0.05);

for(int i=0;i<5;i++)
car(carX[i],0.2*i,0.4,1-0.2*i);



if(rain){
rainDraw();
rainSplash();
}

glutSwapBuffers();
}

void update(int v){

for(int i=0;i<5;i++){

carX[i]+=0.0007+(i*0.0003);

if(carX[i]>1)
carX[i]=-1;

}



for(int i=0;i<4;i++){

cloud[i]+=0.001;

if(cloud[i]>1)
cloud[i]=-1;

}

birdX+=0.002;

if(birdX>1.2)
birdX=-1.2;

if(rain){

for(int i=0;i<300;i++){

drop[i].y-=0.03;
drop[i].x+=0.01;

if(drop[i].y<-0.6){

drop[i].y=1;
drop[i].x=(rand()%200-100)/100.0;

}

}

}

glutPostRedisplay();
glutTimerFunc(16,update,0);

}

void keyboard(unsigned char key,int x,int y){

if(key=='r') rain=true;
if(key=='s') rain=false;
if(key=='n') night=true;
if(key=='d') night=false;

}

void init(){

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(-1,1,-1,1);

for(int i=0;i<100;i++){
starX[i]=(rand()%200-100)/100.0;
starY[i]=0.45 + (rand()%55)/100.0;   // stars higher in sky
}

for(int i=0;i<300;i++){
drop[i].x=(rand()%200-100)/100.0;
drop[i].y=(rand()%200)/100.0;
}

srand(time(0));
}

int main(int argc,char** argv){

glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

glutInitWindowSize(1000,700);

glutCreateWindow("Final Smart City Weather Simulation");

init();

glutDisplayFunc(display);
glutKeyboardFunc(keyboard);
glutTimerFunc(16,update,0);

glutMainLoop();
}
