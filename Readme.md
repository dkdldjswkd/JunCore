# JunCore Project

## 1. vcpkg 패키지 관리자 설치
1. **Windows cmd 실행**
2. `cd c:/`
3. `git clone https://github.com/microsoft/vcpkg.git`
4. `cd vcpkg`
5. `bootstrap-vcpkg.bat`
6. 환경 변수 **Path**에 `C:\vcpkg` 추가

## 2. Boost 설치
1. **Windows cmd 실행**
2. `vcpkg install boost:x64-windows`

## 3. Protobuf 설치
1. **Windows cmd 실행**
2. `vcpkg install protobuf:x64-windows-static`

## 4. CMake 설정
- **JunCore CMake 프로젝트의 `CMAKE_TOOLCHAIN_FILE`** 변수를 `"C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"`로 설정.
  - 기본적으로 **x64-debug** 구성에서는 `CMakePresets.json`을 통해 해당 파일이 설정됨.
  - x64-debug 외 구성에서 빌드할 경우, `CMAKE_TOOLCHAIN_FILE` 변수 추가 설정 필요.
  - **vcpkg** 경로가 `"c:/vcpkg"`가 아닐 경우, vcpkg 설치 경로에 맞춰 `CMAKE_TOOLCHAIN_FILE`을 조정.