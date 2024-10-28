# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

 # C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# server_core 라이브러리 정의
add_library(server_core 
 "Packets/ByteBuffer.cpp"
 "Packets/PacketBuffer.cpp"
)

# PUBLIC : server_core를 사용하는 프로젝트에도 포함된다.
target_include_directories(server_core 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${Boost_INCLUDE_DIRS}
		common
)

# PUBLIC : server_core에서 링크한 라이브러리들이 server_core를 링크한 프로젝트에도 링크된다.
target_link_libraries(server_core 
	PUBLIC 
		${Boost_LIBRARIES}
		common
)