add_executable(worldserver 
main.cpp
)

# worldserver에 해당 라이브러리들을 링크한다.
# PUBLIC : worldserver에서 링크한 라이브러리들이 worldserver를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(worldserver 
	PRIVATE 
		common
		shared
		game
)