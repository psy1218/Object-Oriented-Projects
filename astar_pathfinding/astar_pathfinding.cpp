#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <array>
#include <SFML/System/Vector2.hpp>
#include <fstream>
using namespace std;
// namespace를 이용한 상수, 변수 정의
namespace gbl {
	namespace PATHFINDING {
		constexpr unsigned short CHECKS_PER_FRAME = 33;
	}
	namespace SCREEN {
		constexpr unsigned char RESIZE = 1;
		constexpr unsigned short HEIGHT = 700;
		constexpr unsigned short WIDTH = 1280;
		constexpr chrono::microseconds FRAME_DURATION(16667 * 3);
	}
	namespace MAP {
		constexpr unsigned char CELL_SIZE = 16;
		constexpr unsigned char ORIGIN_X = 8;
		constexpr unsigned char ORIGIN_Y = 8;
		constexpr unsigned short COLUMNS = 78;
		constexpr unsigned short ROWS = 43;
		enum Cell {
			Empty,
			Invalid,
			Path,
			Visited,
			Wall1,   //ㄱ
			Wall2,   //ㄴ
			Wall3,   //ㄷ
			Wall4,   //ㄹ
			Wall5,   //ㅁ
			Wall6,   //row
			Wall7,   //column
			Wall8,   //column_2
		};
	}

	// 2차원 지도 각 셀 상태에 대한 배열
	// (x, y) 형태로 나타내가 위해서 columns를 먼저 지정
	template <typename value_type = MAP::Cell>
	using Map = std::array<std::array<value_type, MAP::ROWS>, MAP::COLUMNS>;
	// 2차원 좌표 타입
	template <typename value_type = unsigned short>
	using Position = std::pair<value_type, value_type>;
	Position<short> get_mouse_cell(const sf::RenderWindow& i_window) {
		float mouse_x = sf::Mouse::getPosition(i_window).x - MAP::ORIGIN_X * SCREEN::RESIZE;
		float mouse_y = sf::Mouse::getPosition(i_window).y - MAP::ORIGIN_Y * SCREEN::RESIZE;
		return Position<short>(
			floor(mouse_x / MAP::CELL_SIZE / SCREEN::RESIZE),
			floor(mouse_y / MAP::CELL_SIZE / SCREEN::RESIZE));
	}
}

//--------------------
//신규 클래스 정의
//-------------------
class Mouse {
public:
	Mouse() {}
	int mouse_cnt;
	Mouse& operator++() {
		mouse_cnt++;
		return *this;
	}
};

//--------------------
//신규 클래스 정의, 클래스 상속
//-------------------
class Animal {
public:
	Animal() {}
	sf::Sprite s;
	sf::Texture penguin_texture;
	//-----------------
	//Pure virutal funcion, function overriding
	//------------------
	virtual void animal(sf::Sprite& map_sprite) = 0;
};
class Penguin : public Animal {
public:
	Penguin() {
		penguin_texture.loadFromFile("penguin copy.png");
	}
	void animal(sf::Sprite& _sprite) {
		_sprite.setTexture(penguin_texture);
	}
};

//--------------------
//신규 클래스 정의, 클래스 상속
//-------------------
int obstacle_cnt = 0; //장애물 셀 세기 위한 전역 변수

class Obstacle {
public:
	Obstacle() {
	}
	//-----------------
	//Pure virutal funcion, function overriding
	//------------------
	virtual bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& data) = 0;
};

// ㄱ
class K :public Obstacle {
public:
	K() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 2 < 43
			&& _data[x][y] != 1
			&& _data[x + 1][y] != 1
			&& _data[x + 1][y + 1] != 1)
		{
			obstacle_cnt += 3;
			_map[x][y] = gbl::MAP::Cell::Wall1;
			_map[x + 1][y] = gbl::MAP::Cell::Wall1;
			_map[x + 1][y + 1] = gbl::MAP::Cell::Wall1;

			_data[x][y] = 1;
			_data[x + 1][y] = 1;
			_data[x + 1][y + 1] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

//ㄴ
class N :public Obstacle {
public:
	N() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 2 < 43
			&& _data[x][y] != 1
			&& _data[x][y + 1] != 1
			&& _data[x + 1][y + 1] != 1)
		{
			obstacle_cnt += 3;
			_map[x][y] = gbl::MAP::Cell::Wall2;
			_map[x][y + 1] = gbl::MAP::Cell::Wall2;
			_map[x + 1][y + 1] = gbl::MAP::Cell::Wall2;

			_data[x][y] = 1;
			_data[x][y + 1] = 1;
			_data[x + 1][y + 1] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

//ㄷ
class D :public Obstacle {
public:
	D() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 3 < 43
			&& _data[x][y] != 1
			&& _data[x + 1][y] != 1
			&& _data[x][y + 1] != 1
			&& _data[x][y + 2] != 1
			&& _data[x + 1][y + 2] != 1)
		{
			obstacle_cnt += 5;
			_map[x][y] = gbl::MAP::Cell::Wall3;
			_map[x + 1][y] = gbl::MAP::Cell::Wall3;
			_map[x][y + 1] = gbl::MAP::Cell::Wall3;
			_map[x][y + 2] = gbl::MAP::Cell::Wall3;
			_map[x + 1][y + 2] = gbl::MAP::Cell::Wall3;

			_data[x][y] = 1;
			_data[x + 1][y] = 1;
			_data[x][y + 1] = 1;
			_data[x][y + 2] = 1;
			_data[x + 1][y + 2] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

//ㄹ
class L :public Obstacle {
public:
	L() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 3 < 78 && y + 5 < 43
			&& _data[x][y] != 1
			&& _data[x + 1][y] != 1
			&& _data[x + 2][y] != 1
			&& _data[x + 2][y + 1] != 1
			&& _data[x + 2][y + 2] != 1
			&& _data[x + 1][y + 2] != 1
			&& _data[x][y + 2] != 1
			&& _data[x][y + 3] != 1
			&& _data[x][y + 4] != 1
			&& _data[x + 1][y + 4] != 1
			&& _data[x + 2][y + 4] != 1)
		{
			obstacle_cnt += 11;
			_map[x][y] = gbl::MAP::Cell::Wall4;
			_map[x + 1][y] = gbl::MAP::Cell::Wall4;
			_map[x + 2][y] = gbl::MAP::Cell::Wall4;
			_map[x + 2][y + 1] = gbl::MAP::Cell::Wall4;
			_map[x + 2][y + 2] = gbl::MAP::Cell::Wall4;
			_map[x + 1][y + 2] = gbl::MAP::Cell::Wall4;
			_map[x][y + 2] = gbl::MAP::Cell::Wall4;
			_map[x][y + 3] = gbl::MAP::Cell::Wall4;
			_map[x][y + 4] = gbl::MAP::Cell::Wall4;
			_map[x + 1][y + 4] = gbl::MAP::Cell::Wall4;
			_map[x + 2][y + 4] = gbl::MAP::Cell::Wall4;

			_data[x][y] = 1;
			_data[x + 1][y] = 1;
			_data[x + 2][y] = 1;
			_data[x + 2][y + 1] = 1;
			_data[x + 2][y + 2] = 1;
			_data[x + 1][y + 2] = 1;
			_data[x][y + 2] = 1;
			_data[x][y + 3] = 1;
			_data[x][y + 4] = 1;
			_data[x + 1][y + 4] = 1;
			_data[x + 2][y + 4] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

//ㅁ
class M :public Obstacle {
public:
	M() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 2 < 43
			&& _data[x][y] != 1
			&& _data[x + 1][y] != 1
			&& _data[x + 2][y] != 1
			&& _data[x][y + 1] != 1
			&& _data[x][y + 2] != 1
			&& _data[x + 1][y + 2] != 1
			&& _data[x + 2][y + 2] != 1
			&& _data[x + 2][y + 1] != 1)
		{
			obstacle_cnt += 8;
			_map[x][y] = gbl::MAP::Cell::Wall5;
			_map[x + 1][y] = gbl::MAP::Cell::Wall5;
			_map[x + 2][y] = gbl::MAP::Cell::Wall5;
			_map[x][y + 1] = gbl::MAP::Cell::Wall5;
			_map[x][y + 2] = gbl::MAP::Cell::Wall5;
			_map[x + 1][y + 2] = gbl::MAP::Cell::Wall5;
			_map[x + 2][y + 1] = gbl::MAP::Cell::Wall5;
			_map[x + 2][y + 2] = gbl::MAP::Cell::Wall5;

			_data[x][y] = 1;
			_data[x + 1][y] = 1;
			_data[x + 2][y] = 1;
			_data[x][y + 1] = 1;
			_data[x][y + 2] = 1;
			_data[x + 1][y + 2] = 1;
			_data[x + 2][y + 2] = 1;
			_data[x + 2][y + 1] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

// 가로 3칸 모양
class Row :public Obstacle {
public:
	Row() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 2 < 43
			&& _data[x][y] != 1
			&& _data[x + 1][y] != 1
			&& _data[x + 2][y] != 1)
		{
			obstacle_cnt += 3;
			_map[x][y] = gbl::MAP::Cell::Wall6;
			_map[x + 1][y] = gbl::MAP::Cell::Wall6;
			_map[x + 2][y] = gbl::MAP::Cell::Wall6;

			_data[x][y] = 1;
			_data[x + 1][y] = 1;
			_data[x + 2][y] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

// 세로 3칸 모양
class Column :public Obstacle {
public:
	Column() {}
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		if (x + 2 < 78 && y + 2 < 43
			&& _data[x][y] != 1
			&& _data[x][y + 1] != 1
			&& _data[x][y + 2] != 1)
		{
			obstacle_cnt += 3;
			_map[x][y] = gbl::MAP::Cell::Wall7;
			_map[x][y + 1] = gbl::MAP::Cell::Wall7;
			_map[x][y + 2] = gbl::MAP::Cell::Wall7;

			_data[x][y] = 1;
			_data[x][y + 1] = 1;
			_data[x][y + 2] = 1;
			return 1;
		}
		else {
			return 0;
		}
	}
};

// 세로 10칸(수직 1자형)
class Column_2 :public Obstacle {
public:
	Column_2() {}
	int k;
	vector<int> flag;
	vector<int>y_position;
	bool make_Obstacle(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {

		obstacle_cnt += 10;
		for (int i = 3; i < 77; i = i + 8)
		{
			y = rand() % 33;
			flag.push_back(0);
			y_position.push_back(y);
			_map[i][y] = gbl::MAP::Cell::Wall8;
			_map[i][y + 1] = gbl::MAP::Cell::Wall8;
			_map[i][y + 2] = gbl::MAP::Cell::Wall8;
			_map[i][y + 3] = gbl::MAP::Cell::Wall8;
			_map[i][y + 4] = gbl::MAP::Cell::Wall8;
			_map[i][y + 5] = gbl::MAP::Cell::Wall8;
			_map[i][y + 6] = gbl::MAP::Cell::Wall8;
			_map[i][y + 7] = gbl::MAP::Cell::Wall8;
			_map[i][y + 8] = gbl::MAP::Cell::Wall8;
			_map[i][y + 9] = gbl::MAP::Cell::Wall8;

			_data[i][y] = 1;
			_data[i][y + 1] = 1;
			_data[i][y + 2] = 1;
			_data[i][y + 3] = 1;
			_data[i][y + 4] = 1;
			_data[i][y + 5] = 1;
			_data[i][y + 6] = 1;
			_data[i][y + 7] = 1;
			_data[i][y + 8] = 1;
			_data[i][y + 9] = 1;
		}
		return 1;
	}
	void move(int x, int y, gbl::Map<>& _map, vector<vector<int>>& _data) {
		k = 0;
		for (int i = 3; i < 77; i = i + 8, k++) {
			if (flag[k] == 0) {
				if (y_position[k] + 10 == 43) {
					flag[k] = 1;
				}
				else {
					_map[i][y_position[k]] = gbl::MAP::Cell::Empty;
					_map[i][y_position[k] + 10] = gbl::MAP::Cell::Wall8;
					y_position[k] = y_position[k] + 1;
				}
			}
			else if (flag[k] == 1) {
				if (y_position[k] - 1 == -1) {
					flag[k] = 0;
				}
				else {
					_map[i][y_position[k] + 9] = gbl::MAP::Cell::Empty;
					_map[i][y_position[k] - 1] = gbl::MAP::Cell::Wall8;
					y_position[k] = y_position[k] - 1;
				}
			}
		}

	}
};


/*
랜덤위치 설정하는 함수
*/
pair<int, int> random_position() {
	int x = rand() % 78;
	int y = rand() % 43;
	return make_pair(x, y);
}

/*
화면에 셀을 그리는 함수
*/
void draw_map(const unsigned short _x,
	const unsigned short _y,
	const gbl::Position<>& _finish_position,
	const gbl::Position<>& _start_position,
	sf::RenderWindow& _window,
	sf::Sprite& _sprite,
	sf::Texture map_texture,
	Penguin penguin,
	const gbl::Map<>& _map)
{
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			_sprite.setPosition((float)gbl::MAP::ORIGIN_X + _x + a * gbl::MAP::CELL_SIZE,
				(float)gbl::MAP::ORIGIN_Y + _y + b * gbl::MAP::CELL_SIZE);

			_sprite.setTexture(map_texture);
			if (a == _start_position.first && b == _start_position.second)
			{
				_sprite.setColor(sf::Color(255, 255, 255));
				_window.draw(_sprite);
				penguin.animal(_sprite);
			}
			else if (a == _finish_position.first && b == _finish_position.second)
			{
				_sprite.setColor(sf::Color(255, 0, 0));
			}
			else
			{
				gbl::MAP::Cell cell_type = _map[a][b];
				switch (cell_type)
				{
				case gbl::MAP::Cell::Empty:
				{
					_sprite.setColor(sf::Color(36, 36, 85));
					break;
				}
				case gbl::MAP::Cell::Path:
				{
					_sprite.setColor(sf::Color(255, 219, 85));
					break;
				}
				case gbl::MAP::Cell::Visited:
				{
					_sprite.setColor(sf::Color(0, 36, 255));
					break;
				}
				case gbl::MAP::Cell::Wall1:
				{
					_sprite.setColor(sf::Color(235, 51, 36));
					break;
				}
				case gbl::MAP::Cell::Wall2:
				{
					_sprite.setColor(sf::Color(235, 130, 28));
					break;
				}
				case gbl::MAP::Cell::Wall3:
				{
					_sprite.setColor(sf::Color(235, 227, 31));
					break;
				}
				case gbl::MAP::Cell::Wall4:
				{
					_sprite.setColor(sf::Color(117, 235, 20));
					break;
				}
				case gbl::MAP::Cell::Wall5:
				{
					_sprite.setColor(sf::Color(255, 183, 235));
					break;
				case gbl::MAP::Cell::Wall6:
				{
					_sprite.setColor(sf::Color(38, 9, 235));
					break;
				}
				case gbl::MAP::Cell::Wall7:
				{
					_sprite.setColor(sf::Color(109, 149, 235));
					break;
				}
				case gbl::MAP::Cell::Wall8:
				{
					_sprite.setColor(sf::Color(235, 198, 141));
					break;
				}

				}
				}
			}
			_window.draw(_sprite);
		}
	}
}

/*Cell type을 반환
		좌표 바깥 위치의 cell에 대해서는 Cell::Invalid값을 반환
*/
gbl::MAP::Cell get_cell(const gbl::Position<short>& _cell, const gbl::Map<>& _map)
{
	if (0 <= _cell.first && 0 <= _cell.second && _cell.first < gbl::MAP::COLUMNS && _cell.second < gbl::MAP::ROWS)
	{
		return _map[_cell.first][_cell.second];
	}
	return gbl::MAP::Cell::Invalid;
}
/*
_cell에 대하여 인접 cell들의 좌표를 반환
*/
std::vector<gbl::Position<>> get_adjacent_cells(const gbl::Position<>& _cell, const gbl::Map<>& _map, int _around_wall_flag)
{
	std::vector<gbl::Position<>> adjacent_cells;
	std::vector<gbl::Position<>> diagonal_adjacent_cells;
	if (_around_wall_flag == 1) {
		for (auto a = -2; a < 3; a++)
		{
			for (auto b = -2; b < 3; b++)
			{
				if (0 == a && 0 == b
					|| -1 == a && -1 == b
					|| -1 == a && 0 == b
					|| -1 == a && 1 == b
					|| 0 == a && -1 == b
					|| 0 == a && 1 == b
					|| 1 == a && -1 == b
					|| 1 == a && 0 == b
					|| 1 == a && 1 == b)
				{
					continue;
				}
				gbl::MAP::Cell _type = get_cell(gbl::Position<short>(_cell.first + a, _cell.second + b), _map);
				// 벽이거나, 범위 밖이 아닌 경우 포함
				if (gbl::MAP::Cell::Invalid != _type && gbl::MAP::Cell::Wall1 != _type
					&& gbl::MAP::Cell::Wall2 != _type && gbl::MAP::Cell::Wall7 != _type
					&& gbl::MAP::Cell::Wall3 != _type && gbl::MAP::Cell::Wall8 != _type
					&& gbl::MAP::Cell::Wall4 != _type
					&& gbl::MAP::Cell::Wall5 != _type && gbl::MAP::Cell::Wall6 != _type)
				{
					adjacent_cells.push_back(gbl::Position<>(a + _cell.first, b + _cell.second));
				}
			}
		}
	}
	else {
		for (auto a = -1; a < 2; a++)
		{
			for (auto b = -1; b < 2; b++)
			{
				// 자기자신 제외
				if (0 == a && 0 == b)
				{
					continue;
				}

				gbl::MAP::Cell _type = get_cell(gbl::Position<short>(_cell.first + a, _cell.second + b), _map);
				// 벽이거나, 범위 밖이 아닌 경우 포함
				if (gbl::MAP::Cell::Invalid != _type && gbl::MAP::Cell::Wall1 != _type
					&& gbl::MAP::Cell::Wall2 != _type && gbl::MAP::Cell::Wall7 != _type
					&& gbl::MAP::Cell::Wall3 != _type
					&& gbl::MAP::Cell::Wall8 != _type && gbl::MAP::Cell::Wall4 != _type
					&& gbl::MAP::Cell::Wall5 != _type && gbl::MAP::Cell::Wall6 != _type)
				{
					adjacent_cells.push_back(gbl::Position<>(a + _cell.first, b + _cell.second));
				}
			}
		}
	}
	return adjacent_cells;
}

/*
heuristic score 계산
*/
float calculate_h_score(const gbl::Position<>& _cell_0, const gbl::Position<>& _cell_1)
{
	float distance_x = abs(_cell_1.first - _cell_0.first);
	float distance_y = abs(_cell_1.second - _cell_0.second);
	return std::max(distance_x, distance_y) + std::min(distance_x, distance_y) * (sqrt(2) - 1);
}


/*A *algorithm
 */
bool astar_search(unsigned short& _path_length,
	unsigned short& _total_checks,
	std::chrono::microseconds& _duration,
	std::map<gbl::Position<>,
	gbl::Position<>> &_previous_cell,
	std::vector<gbl::Position<>>& _path_vector,
	gbl::Map<float>& _f_scores,
	gbl::Map<float>& _g_scores,
	const gbl::Map<float>& _h_scores,
	gbl::Position<>& _finish_position,
	const gbl::Position<>& _start_position,
	gbl::Map<>& _map,
	std::vector<gbl::Position<>>& _path_cell_save,
	int _around_wall_flag)
{
	// 시간 측정
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	// 8번만 A* 반복을 수행하고, frame을 다시 그린다.
	for (unsigned short a = 0; a < gbl::PATHFINDING::CHECKS_PER_FRAME; a++)
	{
		// 알고리즘 종료
		if (1 == _path_vector.empty())
		{
			_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
			return true;
		}
		std::vector<gbl::Position<>>::iterator min_f_cell_iterator = _path_vector.begin();
		gbl::Position<> min_f_cell;
		// f score값이 가장 작은 cell을 찾아낸다.

		// ㅁ자 모양의 목적지일 때 추가 처리
		if (_around_wall_flag == 1) {
			// ㅁ자와 연결된 빈 셀 찾기
			vector<gbl::Position<>> connected_empty_cell = get_adjacent_cells(_finish_position, _map, _around_wall_flag);

			for (std::vector<gbl::Position<>>::iterator a = 1 + _path_vector.begin(); a != _path_vector.end(); a++)
			{
				if (_f_scores[a->first][a->second] < _f_scores[min_f_cell_iterator->first][min_f_cell_iterator->second])
				{
					min_f_cell_iterator = a;
				}
			}
			min_f_cell = *min_f_cell_iterator;

			_path_vector.erase(min_f_cell_iterator);
			_map[min_f_cell.first][min_f_cell.second] = gbl::MAP::Cell::Visited;

			for (int i = 0; i < connected_empty_cell.size(); i++) {
				if (min_f_cell == connected_empty_cell[i]) {
					gbl::Position<> path_cell = min_f_cell;
					do
					{
						_path_length++;
						_map[path_cell.first][path_cell.second] = gbl::MAP::Cell::Path;
						_path_cell_save.push_back(path_cell);
						path_cell = _previous_cell.at(path_cell);

					} while (_start_position != path_cell);
					_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
					return 1;
				}
			}
		}
		else {
			for (std::vector<gbl::Position<>>::iterator a = 1 + _path_vector.begin(); a != _path_vector.end(); a++)
			{
				if (_f_scores[a->first][a->second] < _f_scores[min_f_cell_iterator->first][min_f_cell_iterator->second])
				{
					min_f_cell_iterator = a;
				}
			}
			min_f_cell = *min_f_cell_iterator;

			// 길이 없는 경우
			if (FLT_MAX == _f_scores[min_f_cell.first][min_f_cell.second])
			{
				_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
				cout << 1 << "\n";
				return true;
			}
			_path_vector.erase(min_f_cell_iterator);
			_map[min_f_cell.first][min_f_cell.second] = gbl::MAP::Cell::Visited;
			_total_checks++;
			// 최종 목적지에 도달한 경우
			if (min_f_cell == _finish_position)
			{
				gbl::Position<> path_cell = min_f_cell;
				do
				{
					_path_length++;
					_map[path_cell.first][path_cell.second] = gbl::MAP::Cell::Path;
					_path_cell_save.push_back(path_cell);
					path_cell = _previous_cell.at(path_cell);

				} while (_start_position != path_cell);
				_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
				return 1;
			}
		}

		// 주변 셀들에 대해서 업데이트
		for (const gbl::Position<>& adjacent_cell : get_adjacent_cells(min_f_cell, _map, 0))
		{
			if (gbl::MAP::Cell::Visited != _map[adjacent_cell.first][adjacent_cell.second])
			{
				float g_score = _g_scores[min_f_cell.first][min_f_cell.second];
				if (abs(adjacent_cell.first - min_f_cell.first) == abs(adjacent_cell.second - min_f_cell.second))
				{ // 대각선 거리
					g_score += sqrt(2);
				}
				else
				{ // 상하좌우 거리
					g_score++;
				}
				// g score (실제 거리)가 더 짧아지는 경우에 대해서만,
				if (g_score < _g_scores[adjacent_cell.first][adjacent_cell.second])
				{
					_previous_cell[adjacent_cell] = min_f_cell;
					_f_scores[adjacent_cell.first][adjacent_cell.second] = g_score + _h_scores[adjacent_cell.first][adjacent_cell.second];
					_g_scores[adjacent_cell.first][adjacent_cell.second] = g_score;
					// 이미 추가되지
					if (_path_vector.end() == std::find(_path_vector.begin(), _path_vector.end(), adjacent_cell))
					{
						_path_vector.push_back(adjacent_cell);
					}
				}
			}
		}
	}
	_duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time);
	return 0;
}

/*
A* 알고리즘 초기화
*/
void astar_reset(bool& _finished,
	unsigned short& _path_length,
	unsigned short& _total_checks,
	std::chrono::microseconds& _duration,
	std::map<gbl::Position<>, gbl::Position<>>& _previous_cell,
	std::vector<gbl::Position<>>& _path_vector,
	gbl::Map<float>& _f_scores,
	gbl::Map<float>& _g_scores,
	gbl::Map<float>& _h_scores,
	const gbl::Position<>& _finish_position,
	const gbl::Position<>& _start_position,
	gbl::Map<>& _map)
{
	_finished = 0;
	_path_length = 0;
	_total_checks = 0;
	_duration = std::chrono::microseconds(0);
	_previous_cell.clear();
	_path_vector.clear();
	_path_vector.push_back(_start_position);
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			_h_scores[a][b] = calculate_h_score(gbl::Position<>(a, b),
				_finish_position);
			if (a == _start_position.first && b == _start_position.second)
			{
				_f_scores[a][b] = _h_scores[a][b];
				_g_scores[a][b] = 0;
			}
			else
			{
				//map a,b가 확인하고 엠티로 바꾸기
				if (_map[a][b] == gbl::MAP::Cell::Visited || _map[a][b] == gbl::MAP::Cell::Path) {
					_map[a][b] = gbl::MAP::Cell::Empty;
				}
				_f_scores[a][b] = FLT_MAX;
				_g_scores[a][b] = FLT_MAX;
			}
		}
	}
}

//--------------------
//function overloading , file input/output 
//-------------------
void file_overloading(ofstream& _os) {
	_os << "file 캐릭터정보: 펭귄 장애물정보 종류: ㄱ,ㄴ,ㄷ,ㄹ,ㅁ,세로3칸,가로3칸,세로10칸(움직이는장애물)\n";
	_os.close();
}

void file_overloading(ifstream& _is) {
	while (_is) {
		string str;
		_is >> str;
		cout << str << "\n";
	}
}

int main()
{
	//--------------------
	//Lambda expression , file input/output 
	//-------------------
	auto version = [](string version_name) {
		std::cout << "SFML Version:" << version_name << "\n";
	};

	version("2.6.0");

	ofstream os{ "obstacle.txt" };
	file_overloading(os);
	ifstream is{ "obstacle.txt" };
	file_overloading(is);


	srand(time(NULL));
	bool astar_finished = 0;
	unsigned short astar_path_length = 0;
	unsigned short astar_total_checks = 0;
	// Astar 수행시간 저장
	// == chrono::duration<double, micro>
	chrono::microseconds astar_duration(0);
	map<gbl::Position<>, gbl::Position<>> astar_previous_cell;
	vector<gbl::Position<>> astar_path_vector;
	gbl::Map<float> astar_h_scores = {};
	gbl::Map<float> astar_g_scores = {};
	gbl::Map<float> astar_f_scores = {};
	//--------- SFML 화면 그리기---------------
	bool mouse_pressed = 0;
	bool pause_search = 1;
	chrono::microseconds lag(0);
	chrono::steady_clock::time_point previous_time;
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(gbl::SCREEN::RESIZE * gbl::SCREEN::WIDTH,
		gbl::SCREEN::RESIZE * gbl::SCREEN::HEIGHT),
		"Pathfinding by A*", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));
	sf::Sprite sprite;
	sf::Texture map_texture;
	map_texture.loadFromFile("Map.png");
	//penguin_texture.loadFromFile("penguin copy.png");
	// 
// -----------------
//STL map
//-------------------
	gbl::Map<> map = {};
	gbl::Position<> finish_position(0, 0);
	gbl::Position<> start_position(0, 0);
	gbl::Position<short> mouse_cell_start;
	//최단경로 저장하기
	std::vector<gbl::Position<>> path_cell_save;
	gbl::Position<> path_position(0, 0);
	// 모든 셀을 empty상태로 설정
	for (array<gbl::MAP::Cell, gbl::MAP::ROWS>& column : map)
	{
		column.fill(gbl::MAP::Cell::Empty);
	}

	previous_time = chrono::steady_clock::now();
	int leftbutton = 0;  //좌클릭 후 연속으로 목적지가 바뀌는 걸 막기 위한 
	int leftflag = 0;    //캐릭터가 이동을 종료했는지 확인하는 flag

	//좌클릭 후 캐릭터가 목적지까지 도착하면 flag=0, 가는 도중은 1
	int around_wall_flag = 0;
	Penguin penguin;
	Mouse mouse;
	mouse.mouse_cnt = 0;
	K k;
	N n;
	D d;
	L l;
	M m;
	Row row;
	Column column;
	Column_2 column_2;
	pair<int, int>random_p;
	vector<vector<int>>data(78, vector <int>(43, 0));


	// ㄱ,ㄴ,ㄷ,ㄹ 장애물
	/*
	for (int i = 0; i < 2; i++)
	{
		while ((k.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
		while ((n.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
		while ((d.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
		while ((l.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
		while ((row.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
		while ((column.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
			random_p = random_position();
		}
	}

	*/

	// ㅁ 장애물
/*
random_p = random_position();
while ((m.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
	random_p = random_position();
}
*/

// column_2 장애물 (수직 10칸)
	while ((column_2.make_Obstacle(random_p.first, random_p.second, map, data)) == 0) {
		random_p = random_position();
	}

	// 셀 70% 채우기 위한 while (row, column)
	/*
	while (obstacle_cnt <= 2350) {
		random_p = random_position();
		row.make_Obstacle(random_p.first, random_p.second, map, data);
		random_p = random_position();
		column.make_Obstacle(random_p.first, random_p.second, map, data);
	}
	*/

	int count = 0;
	while (1 == window.isOpen())
	{
		chrono::microseconds delta_time =
			chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - previous_time);
		lag += delta_time;
		previous_time += delta_time;
		// 최소 frame 유지시간 == FRAME_DURATION
		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			// 화면을 다시 그릴 필요가 있는지 검사
			bool map_updated = 0;
			// 다음 번 연산시간 설정
			lag -= gbl::SCREEN::FRAME_DURATION;
			// 키보드 입력에 대한 검사
			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::MouseButtonPressed:
				{
					gbl::Position<short> mousePosition = gbl::get_mouse_cell(window);
					++mouse;
					cout << mouse.mouse_cnt << "\n";
					//if (leftflag == 0) {
					if (mousePosition.first % 8 != 3) {
						if (event.mouseButton.button == sf::Mouse::Left) {
							leftbutton = 1;
							leftflag = 1;
							around_wall_flag = 0;

							// 목적지가 사방으로 벽으로 존재하는지 확인하는 if 문
							if (data[mousePosition.first - 1][mousePosition.second - 1] == 1
								&& data[mousePosition.first][mousePosition.second - 1] == 1
								&& data[mousePosition.first - 1][mousePosition.second - 1] == 1
								&& data[mousePosition.first - 1][mousePosition.second] == 1
								&& data[mousePosition.first + 1][mousePosition.second] == 1
								&& data[mousePosition.first - 1][mousePosition.second + 1] == 1
								&& data[mousePosition.first][mousePosition.second + 1] == 1
								&& data[mousePosition.first + 1][mousePosition.second + 1] == 1
								)
							{
								around_wall_flag = 1;
							}

							break;
						}
					}
					//	}
				}
				}
			}

			if (leftbutton == 1) {
				leftbutton = 0;
				gbl::Position<short> mouse_cell = gbl::get_mouse_cell(window);
				if (mouse_cell.first < gbl::MAP::COLUMNS && mouse_cell.second < gbl::MAP::ROWS)
				{
					//--------------------
					//try - catch 예외 처리
					//-------------------

					bool me_ok = true;
					try {
						if (finish_position == static_cast<gbl::Position<>>(mouse_cell) || start_position ==
							static_cast<gbl::Position<>>(mouse_cell))
						{
							throw me_ok;
						}

						if (gbl::MAP::Cell::Wall1 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall2 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall3 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall4 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall5 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall6 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall7 != map[mouse_cell.first][mouse_cell.second]
							&& gbl::MAP::Cell::Wall8 != map[mouse_cell.first][mouse_cell.second]
							)
						{
							map_updated = 1;
							path_cell_save.clear();
							finish_position = mouse_cell;
						}

					}
					catch (bool _me_ok) {
						cout << "이미 위치에 있습니다, " << "\n";
					}
				}
			}

			if (1 == map_updated)
			{
				astar_reset(astar_finished,
					astar_path_length,
					astar_total_checks,
					astar_duration,
					astar_previous_cell,
					astar_path_vector,
					astar_f_scores,
					astar_g_scores,
					astar_h_scores,
					finish_position,
					start_position,
					map);
			}
			if (false == astar_finished)
			{
				astar_finished = astar_search(
					astar_path_length,
					astar_total_checks,
					astar_duration,
					astar_previous_cell,
					astar_path_vector,
					astar_f_scores,
					astar_g_scores,
					astar_h_scores,
					finish_position,
					start_position,
					map,
					path_cell_save,
					around_wall_flag);
			}
			else {
				if (!path_cell_save.empty()) {
					path_position = path_cell_save.back();
					if (map[path_position.first][path_position.second] == gbl::MAP::Cell::Path
						|| map[path_position.first][path_position.second] == gbl::MAP::Cell::Empty
						|| map[path_position.first][path_position.second] == gbl::MAP::Cell::Visited)
					{
						start_position = path_position;
						path_cell_save.pop_back();
					}
					if (path_cell_save.empty()) {
						leftflag = 0;
					}
				}
			}
			if (count % 2 == 0) {
				column_2.move(random_p.first, random_p.second, map, data);
				count = 0;
			}
			count++;

			window.clear();
			draw_map(0, 0,
				finish_position,
				start_position,
				window,
				sprite,
				map_texture,
				penguin,
				map);
			window.display();
		}

	}
	std::cout << 78 * 43 << "\n" << obstacle_cnt << "\n" << 78 * 43 * 0.7 << "\n" << obstacle_cnt * 0.7 << "\n";
	return 0;
}