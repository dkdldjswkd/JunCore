# JunCore Project

## 프로젝트 소개
JunCore는 C++ 게임 서버 프로그래밍을 위한 네트워크 코어 프로젝트로, 현재 미완성 단계입니다.

---

## 1. Windows 빌드

### 1.1 vcpkg 패키지 관리자 설치
1. Windows cmd 실행
2. 아래 명령어 입력
    ```cmd
    cd c:/
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    bootstrap-vcpkg.bat
    setx PATH "%PATH%;C:\vcpkg"
    ```
   > 위 명령어는 vcpkg를 설치하고 환경 변수`PATH`에 `C:\vcpkg`를 추가합니다.

### 1.2 프로젝트 Clone 및 의존성 설치
1. Windows cmd 실행
2. 아래 명령어 입력
    ```cmd
    cd c:/
    git clone https://github.com/dkdldjswkd/JunCore
    cd JunCore/JunCore
    vcpkg install
    ```

---

## 2. Docker 빌드
1. Windows cmd 실행
2. 아래 명령어 입력
    ```cmd
    cd c:/
    git clone https://github.com/dkdldjswkd/JunCore
    cd JunCore/Docker
    docker-compose up -d
    ```
3. Docker 컨테이너가 정상적으로 실행되었는지 확인
    ```cmd
    docker ps
    ```

---

## 그 외
- 윈도우 빌드 시 JunCore 프로젝트는 `CMAKE_TOOLCHAIN_FILE` 변수를 `"C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"`로 설정하고 있습니다.
- 만약, vcpkg 경로가 `c:/vcpkg`가 아닐 경우, vcpkg 설치 경로에 맞춰 `CMakePresets.json`의 `CMAKE_TOOLCHAIN_FILE`을 수정해야 합니다.

