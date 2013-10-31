set(BR_WITH_WG_PARTS_BASED_DETECTOR ON CACHE BOOL "Build with Willow Garage's Parts Based Detector")

if(${BR_WITH_WG_PARTS_BASED_DETECTOR})
  find_package(WGPartsBasedDetector REQUIRED)
  set(BR_THIRDPARTY_SRC ${BR_THIRDPARTY_SRC} plugins/wgpartsbaseddetector.cpp)
  set(BR_THIRDPARTY_LIBS ${BR_THIRDPARTY_LIBS} ${WGPARTSBASEDDETECTOR_LIBS})
endif()
