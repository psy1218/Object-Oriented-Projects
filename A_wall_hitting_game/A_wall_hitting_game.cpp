#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <time.h> 
#include <string> 
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;

// 부모 클래스 
class In {
protected: //상속받는 클래스에서 쓰기 위해서 protected 사용
	Texture t;
	double x, y;
public:
	Sprite s; //public에 있는 이유는 한 클래스에서 다른 클래스의 객체를 사용하기 위해 존재, 예를 들어 부딪힘 확인 위해서(intersects)
	SoundBuffer b;
	Sound sound;
	void move(double dx, double dy) { //이동하는 함수
		x += dx;
		y += dy;
		s.setPosition(x, y);
	}
	void draw(RenderWindow& window) { //그리는 함수
		window.draw(s);
	}
};

// 상속받은 패들 클래스 
class Paddle :public In { 
private:
	bool die;
public:
	Paddle(double _x, double _y); //패들 생성자
	void gostop(double _x); // 패들 좌우 이동 함수(끝에 도착할시 return을 사용해서 밖 화면을 나가지 못하게 하기)
};

Paddle::Paddle(double _x, double _y) {
	t.loadFromFile("paddle.png");
	s.setTexture(t);
	s.setScale(0.1, 0.7);
	s.setPosition(_x, _y);
	die = false;
}

void Paddle::gostop(double _x) {
	//_x 와 0의 크기를 비교해야 화면 끝에 도착했을 때 반대방향으로 움직일 수 있음.
	if (s.getPosition().x <= 0 && _x < 0) {
		return;
	}
	if (s.getPosition().x >= 370 && _x > 0) {
		return;
	}
	s.move(_x, 0);
}

//상속 받은 ball 클래스
class Ball : public In {
private:
	bool die;
	double dx;
	double dy;
public:
	Ball(); //볼 생성자
	void update(Paddle& _paddle); //볼 업데이트 함수(벽과 패들에 닿으면 방향 바꾸기, 이동과 소리 플레이, 죽음 판단)
	bool not_ball_paddle_hit(); //볼과 패들에 닿지 않는 상황을 저장하는 함수 (죽음)
};

Ball::Ball() {
	x = 200;
	y = 300;
	t.loadFromFile("ball.png");
	s.setTexture(t);
	s.setPosition(x, y);
	die = false;
	dx = 5.0;
	dy = -5.0;
	if (!b.loadFromFile("ball_hit.wav")) {
		cout << "ball_hit.wav 존재 X\n";
	}
	sound.setBuffer(b);
}

void Ball::update(Paddle& _paddle) { //==> 게임 필수 조건: 사운드 효과 (볼이 벽과 패들에 닿으면 사운드)
	if (s.getPosition().x <= 0 || s.getPosition().x >= 390) {
		dx = -dx;
		sound.play();
	}
	if (s.getPosition().y <= 0) {
		dy = -dy;
		sound.play();
	}

	if (FloatRect(_paddle.s.getGlobalBounds()).intersects(s.getGlobalBounds()) == true) {
		dy = -dy;
		sound.play();
	}
	else if (s.getPosition().y >= 600) {
		die = true;
		s.setPosition(200, 0);
		x = 200;
		y = 0;
	}
	s.move(dx, dy);
}

bool Ball::not_ball_paddle_hit() {
	//그냥 die만 return 하면 계속 죽어있는 상태여서 목숨이 바로 0으로 줄어든다.
	bool temp = die;
	die = false;
	return temp;
}

//상속받은 폭탄 클래스
class Bomb : public In {
private:
	bool die;
	double under_x;
	double speed;
public:
	Bomb(); //폭탄 생성자

	shared_ptr<Sprite>bombs[5]; //------->프로그래밍 필수 조건: 스마트 포인터, 객체 배열
	//==> shared_ptr이 class이기에 bombs라는 객체를 만들었다.
	//==> 하나의 객체에 대해 여러 포인터가 가리킬 수 있는 shared_ptr 사용 ->  bombs이라는 객체로 배열[5]개를 만들어 
	//    하나씩 포인터가 각각의 객체를 가리켜 스마트 포인터 이용

	void under(Paddle& _paddle); //폭탄이 떨어지는 함수 (속도와 개수를 랜덤으로 설정, 최대 폭탄이 5개)
	bool paddle_bomb_hit(); //패들과 폭탄이 부딛힌 상황을 저장하는 함수
	void draw(RenderWindow& window); //폭탄 그리는 함수(여러 개의 폭탄이라 따로따로 그려줘야함.)=> 상속관련으로 overriding
};

Bomb::Bomb() {
	x = 210;
	y = 10;
	die = 0;
	speed = rand() % 10;
	t.loadFromFile("bomb.png");
	s.setTexture(t);
	s.setPosition(x, y);
	s.setScale(0.5, 0.5);
	for (int i = 0; i < 5; i++) {
		under_x = (rand() % 20) * 20;
		bombs[i] = make_shared<Sprite>(); //--->프로그래밍 필수 조건: 객체 동적 생성
		//==> make_shared 안에 new가 있기 때문에 동적 생성 가능
		*bombs[i] = s;
		bombs[i]->setPosition(under_x, 0);
	}
	if (!b.loadFromFile("crash.wav")) {
		cout << "crash.wav 존재 X\n";
	}
	sound.setBuffer(b);
}

void Bomb::under(Paddle& _paddle) {

	for (int i = 0; i < 5; i++) { //게임 필수 조건==>랜덤으로 n>1 동시 출현
		under_x = (rand() % 20) * 20;
		if (bombs[i]->getPosition().y >=600) {
			bombs[i]->setPosition(under_x, 0);
		}

		bombs[i]->move(0, speed);
		if (FloatRect(_paddle.s.getGlobalBounds()).intersects(bombs[i]->getGlobalBounds()) == true) { //==>게임 필수 조건: 닿으면 bar 폭발
			//패들의 경계선과 폭탄의 경계선이 닿을 때 ==> 공은 상관없음! 
			sound.play(); //==>게임 필수 조건:사운드 효과 1개 (패들과 폭탄이 닿으면!)
			bombs[i]->setPosition(under_x, 0); //폭탄이 떨어지면 위로 올라가서 다시 시작
			die = true;
		}
	}
}

bool Bomb::paddle_bomb_hit() {
	bool temp = die;
	die = false;
	return temp;
}

void Bomb::draw(RenderWindow &window) { 
	for (int i = 0; i < 5.; i++) {
		window.draw(*bombs[i]);
	}
}

//상속받은 목숨 클래스
class Heart : public In {
private:
	int heart;
	bool game;
	Font font;
	Text text1;
	string status;

	Text text2;
	string gameover;

public:
	Heart(); //목숨 생성자
	void death(Paddle &paddle); //죽음 판별 함수(목숨 3개에서 0이 되면 패들은 화면 밖으로 나가고 게임 오버)
	void draw(RenderWindow& window); //그리는 함수(게임오버 text가 끝나고 실행되야하기 때문에 따로 만들기) => 상속관련으로 overriding
	bool gameend(); //게임 중단 하는 함수
};

Heart::Heart() {
	heart = 3; //==> 게임 필수 조건: bar 생명 개수 3개 
	if (!font.loadFromFile("OpenSans-Bold.ttf")) {
		cout << "폰트 파일을 오픈할 수 없음!" << endl;
	}
	text1.setFont(font);
	text2.setFont(font);
	status = "heart:" + to_string(heart);
	text1.setString(status);
	text1.setCharacterSize(20);
	text1.setPosition(0, 0);
	game = false;
	if (!b.loadFromFile("gameover.mp3")) {
		cout << "gameover.mpe 존재 X\n";
	}
	sound.setBuffer(b);
}

void Heart::death(Paddle &paddle) {
	heart -= 1; //==> 하나씩 카운트하기
	status = "heart:" + to_string(heart);
	text1.setString(status);
	text1.setCharacterSize(20);
	text1.setPosition(0, 0);

	gameover = "Game over";
	text2.setString(gameover);
	text2.setCharacterSize(50);
	text2.setPosition(60, 250);
	if (heart == 0) { //==> 목숨이 끝날 때
		sound.play(); //=> 게임 필수 조건: 사운드 효과(게임 오버)
		paddle.s.setPosition(-11111, -1111); //=> 죽으면 bar 는 화면 밖으로!!
		game = true;
	}
}

void Heart::draw(RenderWindow& window) {
	window.draw(text1);
	if (heart == 0) {
		window.draw(text2);
	}
}

bool Heart::gameend() {
	return game;
}

//메인
int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(400, 600), "A_wall_hitting_game");
	window.setFramerateLimit(30);
	Texture Td;
	Sprite background;
	Td.loadFromFile("background.png");
	background.setTexture(Td);
	Paddle paddle(200, 590);
	Ball ball;
	Bomb bomb;
	Heart heart;

	while (window.isOpen()) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
		}

		if (heart.gameend()) { //게임 오버
			continue; //밑에 각 객체의 업데이트와 그리기를 멈추기 위해 continue 실행
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			paddle.gostop(-3.0); //왼쪽 키 누르면 -3만큼 이동
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			paddle.gostop(3.0); //오른쪾 키 누르면 3만큼 이동
		}

		ball.update(paddle); //패들과 볼의 부딪힘을 봐야하기 때문에 아규먼트에 패들을 넣음.

		bomb.under(paddle); //패들과 폭탄의 부딪힘을 봐야하기 때문에 아규먼트에 패들을 넣음.

		if (ball.not_ball_paddle_hit() || bomb.paddle_bomb_hit()) {
			heart.death(paddle); //죽으면 패들을 밖으로 내보내기 위해 아규먼트에 패들을 넣음.
		}

		window.clear();
		window.draw(background);
		paddle.draw(window);
		ball.draw(window);
		bomb.draw(window);
		heart.draw(window);
		window.display();
	}

	return 0;
}