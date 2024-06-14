cmake_minimum_required(VERSION 3.0)
cmake_minimum_required( VERSION 3.6 )

# Create a project
project( Demos )

# Use C++11
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Find OpenCV packages
find_package( OpenCV REQUIRED )

# Find all .cpp files
file( GLOB files "*.cpp" )

message( STATUS "Defining executables... " )

foreach( file ${files} )

        # Removing directory and extension from the file name
        get_filename_component( name ${file} NAME )
        get_filename_component( name_we ${name} NAME_WE )

        # Output the executable name
        message( STATUS ${name_we} )

        # Define the executable compilation rule
        add_executable( ${name_we} ${name_we}.cpp )
        target_link_libraries( ${name_we} ${OpenCV_LIBS} )

endforeach()