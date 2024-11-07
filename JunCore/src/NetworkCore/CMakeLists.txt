# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

 # C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# NetworkCore 라이브러리 정의
add_library(NetworkCore 
 "network/AsyncAcceptor.cpp"
 "network/Session.cpp"
 "network/NetworkThread.cpp" 
 "network/NetworkManager.cpp"
 "Packets/PacketBuffer.cpp"
 )

# PUBLIC : NetworkCore를 사용하는 프로젝트에도 포함된다.
target_include_directories(NetworkCore 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${Boost_INCLUDE_DIRS}
		common
)

# PUBLIC : NetworkCore에서 링크한 라이브러리들이 NetworkCore를 링크한 프로젝트에도 링크된다.
target_link_libraries(NetworkCore 
	PUBLIC 
		${Boost_LIBRARIES}
		common
)