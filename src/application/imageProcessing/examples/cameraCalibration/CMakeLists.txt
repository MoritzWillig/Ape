project(CameraCalibration CXX)

set(CAMERA_CALIBRATION_SOURCES
        cameraCalibration.cpp
        )

add_executable(CameraCalibration
        ${CAMERA_CALIBRATION_SOURCES}
        $<TARGET_OBJECTS:apeImageProcessing>
        )

include_directories(${APE_COMPONENTS_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries(CameraCalibration ${OGRE_LIBRARIES})
target_link_libraries(CameraCalibration ${OpenCV_LIBRARIES})
