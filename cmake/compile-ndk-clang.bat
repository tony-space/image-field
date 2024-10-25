@echo off

REM AVAILABLE PLATFORMS arm64-v8a armeabi-v7a x86 x86_64
SET PLATFORM_ABI=arm64-v8a
SET PLATFORM_VERSION=27
SET BUILD_TYPE=Release

SET BUILD_PATH=build-android/%PLATFORM_ABI%/%BUILD_TYPE%

cmake ^
	-S . ^
	-B %BUILD_PATH% ^
	-G "Unix Makefiles" ^
	-DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
	-DCMAKE_CXX_FLAGS="" ^
	-DCMAKE_TOOLCHAIN_FILE=%NDK_LOCATION%/build/cmake/android.toolchain.cmake ^
	-DANDROID_PLATFORM=%PLATFORM_VERSION% ^
	-DANDROID_ABI=%PLATFORM_ABI% ^
	-DCMAKE_MAKE_PROGRAM=%NDK_LOCATION%/prebuilt/windows-x86_64/bin/make.exe ^
	-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH ^
	-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH ^
	-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH ^
	-DCMAKE_UNITY_BUILD=OFF ^
	-DIMF_USE_PRECOMPILED_HEADERS=ON ^
	-DIMF_CPU_BACKEND=ON ^
	-DIMF_OPENGL_BACKEND=ON ^
	-DIMF_SANDBOXES=ON

cmake --build %BUILD_PATH% --parallel %NUMBER_OF_PROCESSORS% --config %BUILD_TYPE%
