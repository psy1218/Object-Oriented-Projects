# Object-Oriented-Projects

# 🎮 C++ Game & Pathfinding Projects  

이 저장소에는 **C++**과 **SFML** 라이브러리를 활용하여 제작한 두 가지 프로젝트가 포함되어 있습니다.  

1. **A_wall_hitting_game.cpp** – 벽에 공을 튕기며 막대바로 받아내는 아케이드 게임  
2. **astar_pathfinding.cpp** – A* 알고리즘 기반 경로 탐색 시뮬레이터  

---

## 🟢 1. A_wall_hitting_game (벽 튕기기 게임)

### 📌 프로젝트 개요
- **언어/도구:** C++ / SFML (Graphics, Audio)  
- **설명:**  
  - 공이 벽에 튕기면서 내려오고, 플레이어는 **막대바(Paddle)** 를 움직여 공을 받아내야 합니다.  
  - 공을 받지 못하거나, 폭탄(Bomb)에 맞으면 **목숨(Heart)** 이 줄어듭니다.  
  - 목숨이 0이 되면 **Game Over** 메시지가 출력됩니다.  

### ⚙️ 주요 기능
- **Paddle 클래스:** 막대바 좌우 이동, 충돌 방지 로직  
- **Ball 클래스:** 공의 움직임, 벽/막대 충돌 처리, 사운드 효과  
- **Bomb 클래스:** 랜덤 낙하 폭탄 구현, 막대바 충돌 시 목숨 감소  
- **Heart 클래스:** 남은 목숨 관리 및 Game Over 처리  

<br>

## 🎮 실행 방법

### SFML 설치
먼저 [SFML](https://www.sfml-dev.org/download.php)을 설치해야 합니다.  

### 컴파일 & 실행
```bash
g++ astar_pathfinding.cpp -o astar -lsfml-graphics -lsfml-window -lsfml-system
./astar
```


## 🕹️ 사용법
- **좌클릭**: 목적지 지정  
- 펭귄 캐릭터가 장애물을 피해 **최단 경로**로 이동  
- **움직이는 벽**에 따라 경로가 실시간으로 변경  

---

## 🖼️ 결과 화면
- 맵에 다양한 색상으로 표현된 장애물 (ㄱ, ㄴ, ㄷ, ㄹ, ㅁ 등)  
- 파란색/노란색 셀로 탐색 과정 (**Visited, Path**) 시각화  
- 펭귄이 목적지까지 이동하는 **애니메이션** 확인 가능  

---

## 🚀 프로젝트 의의
- C++ **객체지향 프로그래밍(OOP)** 개념 (상속, 오버라이딩, 오버로딩) 실습  
- **SFML**을 활용한 게임/시각화 프로그래밍 경험  
- **알고리즘(A\*) + 실시간 그래픽스** 결합 → 실제 동작 확인 가능  
