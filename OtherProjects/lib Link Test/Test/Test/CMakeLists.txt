set(BOOST_DIR "D:/boost_1_76_0")
set(LIB_INCLUDE_DIR "C:/ProjectVic/vic_server_lib/include")
set(LIB_DIR "C:/ProjectVic/vic_server_lib/lib/Debug")

add_executable (Test
"main.cpp"
)

target_include_directories(Test 
    PUBLIC 
        ${BOOST_DIR}
        ${LIB_INCLUDE_DIR}
)

target_link_libraries(Test 
    PRIVATE 
        ${LIB_DIR}/libboost_date_time-vc142-mt-gd-x64-1_76.lib
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Test PROPERTY CXX_STANDARD 20)
endif()
