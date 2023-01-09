# OSSIA-specific configuration
set(OSSIA_PCH "${SCORE_PCH}" CACHE INTERNAL "" FORCE)
set(OSSIA_STATIC "${SCORE_STATIC_PLUGINS}" CACHE INTERNAL "" FORCE)
set(OSSIA_CXX_STANDARD "20" CACHE INTERNAL "" FORCE)
set(OSSIA_CALLBACK_CONTAINER_MUTEX "std::mutex" CACHE INTERNAL "" FORCE)

if(NOT SCORE_INSTALL_HEADERS)
  set(OSSIA_NO_INSTALL ON CACHE INTERNAL "" FORCE)
endif()
set(OSSIA_MATH_EXPRESSION ON CACHE INTERNAL "" FORCE)

# FFT-relatd features
set(OSSIA_ENABLE_FFT ON CACHE INTERNAL "" FORCE)

if(NOT DEFINED OSSIA_ENABLE_FFTW AND NOT DEFINED OSSIA_ENABLE_KFR)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES ".*Clang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Emscripten")
    set(OSSIA_ENABLE_KFR ON CACHE INTERNAL "" FORCE)
    set(OSSIA_ENABLE_FFTW OFF CACHE INTERNAL "" FORCE)
  else()
    set(OSSIA_ENABLE_FFTW ON CACHE INTERNAL "" FORCE)
    set(OSSIA_ENABLE_KFR OFF CACHE INTERNAL "" FORCE)
  endif()
endif()

# If KFR is not specifically disabled, we enable it
if(NOT DEFINED OSSIA_ENABLE_KFR)
  set(OSSIA_ENABLE_KFR ON CACHE INTERNAL "" FORCE)
endif()

if(SCORE_DEPLOYMENT_BUILD)
  if(EMSCRIPTEN)
    set(ENABLE_DFT_MULTIARCH 0)
    set(ENABLE_DFT_MULTIARCH 0 CACHE "" INTERNAL)
  else()
    set(ENABLE_DFT_MULTIARCH 1)
    set(ENABLE_DFT_MULTIARCH 1 CACHE "" INTERNAL)
  endif()
endif()

if(OSSIA_ENABLE_KFR)
    if(NOT CPU_ARCH AND NOT KFR_ARCH)
        set(KFR_ARCH sse2)
        set(KFR_ARCH sse2 CACHE "" INTERNAL)
    endif()
endif()

set(OSSIA_PD OFF CACHE INTERNAL "" FORCE)
set(OSSIA_MAX OFF CACHE INTERNAL "" FORCE)
set(OSSIA_PYTHON OFF CACHE INTERNAL "" FORCE)
set(OSSIA_UNITY3D OFF CACHE INTERNAL "" FORCE)
set(OSSIA_JAVA OFF CACHE INTERNAL "" FORCE)
set(OSSIA_OSX_FAT_LIBRARIES OFF CACHE INTERNAL "" FORCE)
set(OSSIA_PYTHON OFF CACHE INTERNAL "" FORCE)
set(OSSIA_QT ON CACHE INTERNAL "" FORCE)
set(OSSIA_QT_REQUIRED "REQUIRED" CACHE INTERNAL "" FORCE)
set(OSSIA_QML_DEVICE OFF CACHE INTERNAL "" FORCE)
set(OSSIA_DISABLE_QT_PLUGIN ON CACHE INTERNAL "" FORCE)
set(OSSIA_HIDE_ALL_SYMBOLS ON CACHE INTERNAL "" FORCE)
set(OSSIA_NO_DLLIMPORT ON CACHE INTERNAL "" FORCE)

set(OSSIA_PROTOCOL_MIDI ON CACHE INTERNAL "" FORCE)
if(SCORE_DISABLE_PROTOCOLS)
  set(OSSIA_PROTOCOL_HTTP OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_WEBSOCKETS OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_SERIAL OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_ARTNET OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_JOYSTICK OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_WIIMOTE OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_MINUIT OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_OSCQUERY OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_LEAPMOTION OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_PARALLEL OFF CACHE INTERNAL "" FORCE)
  set(OSSIA_DNSSD OFF CACHE INTERNAL "" FORCE)
else()
  set(OSSIA_PROTOCOL_HTTP ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_WEBSOCKETS ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_SERIAL ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_ARTNET ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_JOYSTICK ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_WIIMOTE ON CACHE INTERNAL "" FORCE)
  set(OSSIA_PROTOCOL_LEAPMOTION ON CACHE INTERNAL "" FORCE)
endif()
set(RTMIDI17_NO_WINUWP ON CACHE INTERNAL "" FORCE)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  set(OSSIA_SUBMODULE_AUTOUPDATE OFF CACHE INTERNAL "" FORCE)
else()
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libossia/cmake/cmake-modules/.git")
    set(OSSIA_SUBMODULE_AUTOUPDATE ON CACHE INTERNAL "" FORCE)
  else()
    set(OSSIA_SUBMODULE_AUTOUPDATE OFF CACHE INTERNAL "" FORCE)
  endif()
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libossia")
