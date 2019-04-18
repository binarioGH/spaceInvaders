#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <list>
using namespace std;

void draw_line(void);
void gotoxy(int x, int y);
void hideCursor(void);
void cls(void);

class Bullet{
private:
	int xd, yd;
	void collition(void);
	int counter = 0;
public:
	int x,y;
	int delay;
	Bullet(int _x, int _y, int dy, int dx,int d){
		x = _x;
		y = _y;
		xd = dx;
		yd = dy;
		delay = d;
	}
	~Bullet(void){
		gotoxy(x,y);
		printf("%c", 32);
	}
	bool death = false;
	void move(void);
};
void Bullet::collition(void){
	if(y>=23 || y <= 3){
		death = true;
	}
	return;
}
void Bullet::move(void){
	gotoxy(x,y);printf(" ");
	if(delay == counter){
		x += xd;
	    y += yd;
	    counter = 0;
	}
	else{
		counter++;
	}
	collition();
	gotoxy(x,y);printf("*");
}
class Wall{
private:
	void draw(int character);
public:
	int x,y;
	Wall(int centerX, int centerY){
		x = centerX;
		y = centerY;
		draw(178);
	}
	~Wall(void){
		draw(167);
		Sleep(100);
		draw(32);
	}
	bool inHitBox(int X, int Y);
	bool death = false;
	int health = 3;
};
void Wall::draw(int character){
	int wx=x-2, wy=y;
	for(wy;wy<y+1;wy++){
		for(wx=x-2;wx<=x+2;wx++){
			gotoxy(wx,wy);printf("%c",character);
		}
	}
	return;
}

bool Wall::inHitBox(int X, int Y){
	if((X >= x-2 && X <= x+2) && (Y >= y && Y <= y+1)){
		health -= 1;
		if(health <= 0){
			death = true;
		}
		return true;
	}
	return false;
}

class Alien{
private:
	int counter = 0;
	int direction = 1;
public:
	int x,y;
	int delay;
	Alien(int _x, int _y, int d){
		x = _x;
		y = _y;
		delay = d;
	}
	~Alien(void){
		gotoxy(x,y);printf("*");
		Sleep(100);
		gotoxy(x,y);printf(" ");
	}
	void move(void);
	bool shooting = false;
	bool death = false;
};
void Alien::move(void){
	gotoxy(x,y);printf(" ");
	if(delay <= counter){
		if(x <= 1 || x >= 79){
			direction *= -1;
			y += 1;
		}
        x += direction;
        counter = 0;
	}
	else{
		counter++;
	}
	gotoxy(x,y);printf("%c", 31);
	return;
}

class Ship{
private:
	
	void collition(void);
public:
	int delay, count = 0;
	int x, y;
	Ship(int _x, int _y, int d){
		x = _x;
		y = _y;
		delay = d;
	}
	~Ship(void){}
	void keyMove(void);
	void move(int key);
	bool shooting = false;
	bool alive = true;
};

void Ship::collition(void){
	if(x >= 80){x--;}
	if(x<= 0){x++;}
	return;
}

void Ship::keyMove(void){
	char key = 'p';
	if(kbhit()){
		key = getch();
	}
	move(key);
	return;
}

void Ship::move(int key){
	gotoxy(x,y);printf(" ");
	if(delay <= count){
		switch(key){
			case 97:x--;break;
			case 100:x++;break;
			case 32:shooting=true;break;
		}
		count = 0;
    }
    else{
    	count++;
    }
	collition();
	gotoxy(x,y);printf("%c", 30);
	return;
} 

int main(){
	hideCursor(); 
	cls();
	gotoxy(0,0);
	draw_line();
	Ship ms (40, 22, 0); //ms means my ship.
	list<Bullet*> bullets;
	list<Bullet*>::iterator bit;
	list<Wall*> walls;
	list<Wall*>::iterator wit;
	list<Alien*> aliens;
	list<Alien*>::iterator ait;
	int i = 0;
	int y = 4;
	for(i=16;i<80;i+=16){
		aliens.push_back(new Alien(i,y,1));
		y += 1;
	}
	walls.push_back(new Wall(20, 19));
	walls.push_back(new Wall(40, 19));
	walls.push_back(new Wall(60, 19));
	bool witdone = false;
	int score = 0;
	while(ms.alive){
		ms.keyMove();
		if(ms.shooting){
			bullets.push_back(new Bullet(ms.x, ms.y-1, -1,0,0));
			ms.shooting = false;
		}
		for(ait=aliens.begin();ait!=aliens.end();ait++){
			(*ait)->move();
			if((*ait)->x == ms.x){
				bullets.push_back(new Bullet((*ait)->x,(*ait)->y+1,1,0,0));
			}
			for(bit=bullets.begin();bit!=bullets.end();bit++){
				if((*bit)->x == (*ait)->x && (*bit)->y == (*ait)->y){
					(*bit)->death = true;
					(*ait)->death = true;
					break;
				}
			}
			if((*ait)->y == 20){
				ms.alive = false;
			}
			if((*ait)->death){
				score += 50;	
				delete(*ait);
				ait = aliens.erase(ait);
			}
		}
		for(bit=bullets.begin();bit!=bullets.end();bit++){
			(*bit)->move();
			if((*bit)->x == ms.x && (*bit)->y == ms.y){
				ms.alive = false;
				break;
			}
			for(wit=walls.begin();wit!=walls.end();wit++){
				if(witdone){
					witdone=false;
					break;
				}
				if((*wit)->inHitBox((*bit)->x, (*bit)->y)){
					(*bit)->death = true;
					witdone = true;
				}
				if((*wit)->death){
					delete(*wit);
					wit = walls.erase(wit);
				}
			}
			if((*bit)->death){
				delete (*bit);
				bit = bullets.erase(bit);
			}
		}
		gotoxy(0,1);printf("Score: %i   ", score);
		Sleep(100);
	}
	return 0;
}

void gotoxy(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
	return;
}


void hideCursor(void){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 0;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
	return;
}

void draw_line(void){
	int x;
	for(x=0;x<80;x++){
		gotoxy(x,2);printf("%c",238);
	}
	return;
}

void cls(void){
	int x, y;
	gotoxy(0,0);
	for(y=0;y<=24;y++){
		for(x=0;x<=80;x++){
			printf(" ");
		}
	}
	gotoxy(0,0);
	return;
}