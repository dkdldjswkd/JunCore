# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

 # C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test 
 "main.cpp"
)

target_include_directories(test 
	PRIVATE 
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(test 
	PRIVATE 
		${Boost_LIBRARIES}
)