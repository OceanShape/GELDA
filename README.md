<p align="center">
  <img width="12%" src="https://user-images.githubusercontent.com/42532724/127039280-f16ae498-a1a6-4c1f-a3f3-6083fdace241.png"/>
</p>
<h1 align="center">GELDA</h1>
<h3 align="center">Game Engine Learning Do Alone</h3>

- **GELDA** 는 **실시간으로 플레이 가능한 플랫포머 맵 에디터** 입니다.
- **GELDA** 는 아래의 소프트웨어 및 프로젝트에서 영감을 얻어 제작하였습니다.
  - [Megaman maker](https://megamanmaker.com/)
  - [Super mario maker 2](https://supermariomaker.nintendo.com/)
  - [홍정모의 게임 만들기 연습문제 패키지](https://github.com/jmhong-simulation/GameDevPracticePackage)


## ⚙ 개발 환경
- Windows10(64bit)
- Visual Studio 2019(64bit)


## 📚 외부 라이브러리
- OpenGL/GLEW/GLFW3: 렌더링 루프 구성에 사용
- stb_image.h: 리소스 로딩에 사용([링크](https://github.com/nothings/stb/blob/master/stb_image.h))
- FMOD(예정): 효과음/배경음악에 사용([링크](https://www.fmod.com/))


## 사용 방법
- **실행**
  1. 링크에서 5개의 폴더 및 파일 다운로드 후, 같은 디렉토리에 저장
  2. GELDA.exe 실행

  - 모드 종류
    - 게임 모드: 일반적인 플랫포머 장르 게임과 같이 중력이 작용하는 공간에서 플레이어블 오브젝트를 조작할 수 있습니다.
    - 에디터 모드: 테스트/맵 에딧을 위한 모드로, 오브젝트의 위치를 편집할 수 있습니다.
      - 현재 맵 편집 기능은 미완성입니다.

  - 조작
    - 좌/우: 화살표
    - 점프: F 키
    - 게임-에디터 모드 변경: G 키

- **개발**
  1. Vcpkg 에서 OpenGL/GLEW/GLFW3 설치
  2. Visual Studio 2019에서 GELDA.sln 실행

## 프로젝트 구성
- GELDA 프로젝트는 네 개의 폴더로 구성되어 있습니다.

1. **api**
   - 외부 라이브러리 저장(현재 stb_image.h)

2. **data**
   - **gameobject.dat**: 맵상에 배치할 오브젝트의 정보가 저장되어 있습니다.
     - 헤더(32byte): 'GELDA GAMEOBJECT FILE'을 포함한 빈 문자열
     - 맵 데이터(256byte): 화면 기준 왼쪽 아래부터 오른쪽 위까지의 오브젝트 데이터 저장
        - 오브젝트 데이터는 16진수 에디터로 편집 가능합니다([notepad++](https://notepad-plus-plus.org/downloads/) 권장).
   - **resource.csv**: 각 오브젝트에 대한 리소스가 디렉토리 묶음으로 구성되어 있습니다.
     - 디렉토리는 resource 폴더를 기준으로 합니다.
     - 리소스 묶음은 '#'으로 구별하며, 파일이 끝날 때까지 읽어들입니다.
        - resource.csv 리소스 묶음의 순서와 gameobject.dat 의 맵 데이터의 ASCII 값은 동일합니다.
        - 예를 들어, 세 번째 리소스 묶음은 맵 데이터의 0x03 값과 일치합니다.
        - 단, 첫 번째 리소스 묶음은 메인 오브젝트의 리소스가 되며, 맵 데이터의 0x01 값과 일치합니다.

3. **engine**
   - GELDA 모든 클래스의 선언 및 정의가 저장되어 있습니다.
     - Game: 게임의 핵심 요소를 담당
     - Object/MoveableObject: 게임에서 상호작용하는 모든 객체를 정의
     - MessageQueue: Game 인스턴스에서 이벤트를 처리할 수 있도록, 게임상의 이벤트를 메시지로 저장
     - ObjectType: 오브젝트의 종류를 정의
     - CollisionType: 충돌 이벤트를 분류
     - CollisionMessage: MessageQueue 에 전달할 메시지의 규격을 정의
     - Status: MoveableObject 인스턴스의 상태를 분류하기 위해 정의

4. **resource**
    - 오브젝트를 위한 리소스가 저장되어 있습니다.


## 🛠 추가할 기능
- 애니메이션
- 사운드(각종 효과음 및 배경음악)
- 마우스 입력(맵 편집 기능에 활용)
- 맵 오브젝트 편집(추가/수정/삭제/저장)
- 오브젝트 종류 및 상호 작용


## 🐞 버그
- 좌우 충돌의 판정 너비가 넓어, 한 칸 너비의 구멍에 빠지지 않음
- 착지하면서 좌우에 있는 블록 오브젝트에 충돌할 경우, 간혹 충돌 처리를 무시하고 오브젝트를 통과
