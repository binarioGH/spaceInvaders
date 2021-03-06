#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <list>
using namespace std;

void win_animation(void);
void death_animation(void);
void draw_line(void);
void gotoxy(int x, int y);
void hideCursor(void);
void cls(void);
int play(int nAliens);
int horde(void);
void draw_underline(int bx, int tx);
void clean_underline(void);

class Box{
private:
	void collition(void);
public: 
	int x, y;
	bool moving = true;
	Box(int _x, int _y){
		x = _x;
		y = _y;
	}
	~Box(void){
		gotoxy(x,y);
		printf("*");
		Sleep(100);
		gotoxy(x,y);
		printf(" ");
	}
	void move(void);
};
void Box::move(void){
	if(!moving){
		return;
	}
	gotoxy(x,y);printf(" ");
	y++;
	collition();
	gotoxy(x,y);printf("%c",220);
	return;
}
void Box::collition(void){
	if(y == 22){
		moving = false;
	}
	return;
}

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
	int bullets = 10;
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

int main(int nArgs, char* argv[]){
	cls();
	hideCursor(); 
	draw_line();
	int totalScore = 0;
	int des = 0;
	bool brk = false;//brk == break;
	char key;
	int helper =des;
	int difficulty = 0;
	
	while(!brk){
		gotoxy(8,11);printf("EASY   MODE");
	    gotoxy(26,11);printf("HORDE   MODE");
	    gotoxy(46,11);printf("HARD   MODE");
	    gotoxy(66,11);printf("EXIT GAME");
		gotoxy(0,0);printf("Total Score: %i   ", totalScore);
		if(kbhit()){
			key = getch();
			switch(key){
				case 100:des += 1;break;
				case  97:des -= 1;break;
				case 13:
				    if(difficulty == -1){
				    	brk=true;
				    }
				    else if(difficulty == -2){
				    	totalScore += horde();
				    }
				    else{
				    	totalScore += play(difficulty);
				    }
				;break;
			}
			if(des <= -1){
				des = 3;
			}
			else if(des >= 4){
				des = 0;
			}
		}
		switch(des){
			case 0:draw_underline(8,19);difficulty=16;break;
			case 1:draw_underline(26,38);difficulty=-2;break;
			case 2:draw_underline(46,57);difficulty=8;break;
			case 3:draw_underline(66,75);difficulty=-1;break;
		}
		if(des != helper){
			helper = des;
			clean_underline();
		}
	}
	return 0;
}


int play(int nAliens){
	cls();
	gotoxy(0,0);
	draw_line();
	Ship ms (40, 22, 0); //ms means my ship.
	list<Box*> boxes;
	list<Box*>::iterator bbit; //bullet box iterator;
	list<Bullet*> bullets;
	list<Bullet*>::iterator bit; //bullet iterator
	list<Wall*> walls;
	list<Wall*>::iterator wit; // wall iterator
	list<Alien*> aliens;
	list<Alien*>::iterator ait; // alien iterator
	int i = 0;
	int c = 0;
	int speed = 0;
	int boxDelay = nAliens * 10;
	int bcounter = 0;
	int boxXposition = rand()%81;
	switch(nAliens){
		case 4:speed=0;break;
		case 8:speed=2;break;
		case 16:speed=4;break;
		default:speed=3;break;
	}
	for(i=nAliens;i<80;i+=nAliens){
		aliens.push_back(new Alien(i,4,speed));
		c++;//c++, haha
	}
	walls.push_back(new Wall(20, 19));
	walls.push_back(new Wall(40, 19));
	walls.push_back(new Wall(60, 19));
	bool witdone = false;
	int score = 0;
	int totalScore = 50 * c;
	gotoxy(0,24);printf("Bullets: %i   ", ms.bullets);
	while(ms.alive && score != totalScore){
		ms.keyMove();
		if(ms.shooting){
			if(ms.bullets > 0){
			    bullets.push_back(new Bullet(ms.x, ms.y-1, -1,0,0));
			    ms.shooting = false;
			    ms.bullets -= 1;
			}
			gotoxy(0,24);printf("Bullets: %i   ", ms.bullets);
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
			if((*ait)->y == 19){
				ms.alive = false;
				break;
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
		for(bbit=boxes.begin();bbit!=boxes.end();bbit++){
			if((*bbit)->moving){
				(*bbit)->move();
			}
			
			if((*bbit)->x == ms.x && (*bbit)->y == ms.y){
				ms.bullets += 5;
				gotoxy(0,24);printf("Bullets: %i   ", ms.bullets);
				delete(*bbit);
				bbit = boxes.erase(bbit);
			}

		}
		if(bcounter == boxDelay){
			boxes.push_back(new Box(boxXposition, 4));
			boxXposition = rand()%81;
			bcounter = 0;
		}
		else{
			bcounter++;
		}
		gotoxy(0,1);printf("Score: %i   ", score);
		Sleep(100);
	}
	for(bit=bullets.begin();bit!=bullets.end();bit++){
		delete(*bit);
		bit = bullets.erase(bit);
	}
	for(wit=walls.begin();wit!=walls.end();wit++){
		delete(*wit);
		wit = walls.erase(wit);
	}
	for(ait=aliens.begin();ait!=aliens.end();ait++){
		delete(*ait);
		ait = aliens.erase(ait);
	}
	delete &ms;
	if(totalScore == score){
		win_animation();
		return score;
	}
	else{
		death_animation();
		return 0;
	}
}

int horde(void){
	cls();
	draw_line();
	Ship ms (40, 22, 0);
	int score=0;
	list<Alien*> aliens;
	list<Alien*>::iterator ait;
	list<Bullet*> bullets;
	list<Bullet*>::iterator bit;
	int top = 500;
	int HordeCounter = top;
	int i;
	int aliensSpeed = 5;
	int speedCounter = 10;
	int shootingCoolDown = 3;
	int shootingCounter = 3;
	while(ms.alive){
		shootingCounter += 1;
		HordeCounter += 1;
		gotoxy(0,0);printf("Score: %i", score);
		ms.keyMove();
		if(ms.shooting){
			if(shootingCoolDown <= shootingCounter){
				bullets.push_back(new Bullet(ms.x, ms.y-1, -1,0,0));
				shootingCounter = 0;
			}
			ms.shooting = false;
		}
		if(HordeCounter >= top){
			for(i=8;i<80;i+=8){
				aliens.push_back(new Alien(i, 4, aliensSpeed));
			}
			HordeCounter = 0;
			top -= 1;
			speedCounter -= 1;
			if(speedCounter <= 0){
				speedCounter = 10;
				if(aliensSpeed > 0){
					aliensSpeed -= 1;
				}
			}
		}
		
		for(ait=aliens.begin();ait!=aliens.end();ait++){
			(*ait)->move();
			if((*ait)->y == 20){
				ms.alive = false;
			}
			if((*ait)->x == ms.x){
				bullets.push_back(new Bullet((*ait)->x,(*ait)->y+1,1,0,0));
			}
			for(bit=bullets.begin();bit!=bullets.end();bit++){
				if((*bit)->x == (*ait)->x && (*bit)->y == (*ait)->y){
					delete(*bit);
					bit = bullets.erase(bit);
					delete(*ait);
					ait = aliens.erase(ait);
					score += 50;
					break;
				}
			}
		}
		for(bit=bullets.begin();bit!=bullets.end();bit++){
			(*bit)->move();
			if((*bit)->x == ms.x && (*bit)->y == ms.y){
				ms.alive = false;
			}
			if((*bit)->death){
				delete (*bit);
				bit = bullets.erase(bit);
			}
		}
		Sleep(100);
	}
	cls();
	return score;
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
void win_animation(void){
	gotoxy(37,12);printf("You Won");
	Sleep(2000);
	cls();
	return;
}
void death_animation(void){
	gotoxy(36,12);printf("You lost");
	Sleep(2000);
	cls();
	return;
}
void draw_underline(int bx,int tx){
	int draw;
	for(draw=bx;draw<tx;draw++){
		gotoxy(draw,12);printf("-");
	}
	return;
}

void clean_underline(void){
	int cleanerX = 0;
	for(cleanerX;cleanerX<80;cleanerX++){
		gotoxy(cleanerX, 12);printf(" ");
	}
	return;
}