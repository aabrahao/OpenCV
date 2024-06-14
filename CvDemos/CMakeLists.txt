cmake_minimum_required(VERSION 3.0)

# Create a project
project( Demos_v2 )

# Use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV packages
find_package( OpenCV REQUIRED )

# Define the executable helloCV
add_executable( helloCV helloCV.cpp )
target_link_libraries( helloCV ${OpenCV_LIBS} )

# Define the executable selectTreshold
add_executable( selectTreshold selectTreshold.cpp )
target_link_libraries( selectTreshold ${OpenCV_LIBS} )

# Define the executable traceBall
add_executable( traceBall traceBall.cpp )
target_link_libraries( traceBall ${OpenCV_LIBS} )

# Define the executable trackBall
add_executable( trackBall trackBall.cpp )
target_link_libraries( trackBall ${OpenCV_LIBS} )

# Define the executable findContours
add_executable( findContours findContours.cpp )
target_link_libraries( findContours ${OpenCV_LIBS} )

# Define the executable findContours
add_executable( assignmentTrackBalls assignmentTrackBalls.cpp )
target_link_libraries( assignmentTrackBalls ${OpenCV_LIBS} )
