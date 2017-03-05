FIND_PACKAGE(SWIG 3.0 REQUIRED)
INCLUDE(${SWIG_USE_FILE})

SET(CMAKE_SWIG_FLAGS "")

IF(BUILD_MATLAB_BINDINGS)
  IF(NOT OPENMA_BUILD_SHARED_LIBS)
    MESSAGE(FATAL_ERROR "OpenMA must be compiled as shared libraries to build the Matlab binding")
  ENDIF()
  
  FIND_PACKAGE(Matlab REQUIRED)
  IF(BUILD_UNIT_TESTS)
    FUNCTION(ADD_MATLAB_UNITTEST_DRIVER)
      # Parse the arguments
      SET(options )
      SET(oneValueArgs NAME)
      SET(multiValueArgs SOURCES INCLUDES)
      CMAKE_PARSE_ARGUMENTS(_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
      # Extra unparsed arguments?
      LIST(LENGTH _TEST_UNPARSED_ARGUMENTS _TEST_NUM_UNPARSED_ARGUMENTS)
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 0)
        IF (NOT _TEST_NAME)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 0 _TEST_NAME)
        ENDIF()
      ENDIF()
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 1)
        IF (NOT _TEST_SOURCES)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 1 _TEST_SOURCES)
        ENDIF()
      ENDIF()
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 2)
        IF (NOT _TEST_INCLUDES)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 2 _TEST_INCLUDES)
        ENDIF()
      ENDIF ()
      # Create the test and register it
      #  - Generate string for the include paths
      SET(_TEST_INCLUDE_PATHS "")
      LIST(LENGTH _TEST_INCLUDES _TEMP_NUM)
      MATH(EXPR _TEMP_NUM "${_TEMP_NUM}-1")
      FOREACH(_inc RANGE ${_TEMP_NUM})
        LIST(GET _TEST_INCLUDES ${_inc} _TEST_INCLUDE_PATH)
        IF(${_inc} GREATER 0)
          SET(_TEST_INCLUDE_PATHS "${_TEST_INCLUDE_PATHS}, ")
        ENDIF()
        SET(_TEST_INCLUDE_PATHS "${_TEST_INCLUDE_PATHS}'${_TEST_INCLUDE_PATH}'")
      ENDFOREACH()
      #  - Generate string for the source files
      SET(_TEST_SOURCE_FILES "")
      LIST(LENGTH _TEST_SOURCES _TEMP_NUM)
      MATH(EXPR _TEMP_NUM "${_TEMP_NUM}-1")
      FOREACH(_inc RANGE ${_TEMP_NUM})
        LIST(GET _TEST_SOURCES ${_inc} _TEST_SOURCE_FILE)
        IF(${_inc} GREATER 0)
          SET(_TEST_SOURCE_FILES "${_TEST_SOURCE_FILES}, ")
        ENDIF()
        SET(_TEST_SOURCE_FILES "${_TEST_SOURCE_FILES}'${_TEST_SOURCE_FILE}'")
      ENDFOREACH()
      CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/cmake/templates/MatlabTestDriver.m.in" "${EXECUTABLE_OUTPUT_PATH}/test_${_TEST_NAME}.m")
      ADD_TEST(NAME ${_TEST_NAME} WORKING_DIRECTORY ${OPENMA_EXECUTABLE_PATH} COMMAND ${MATLAB_EXECUTABLE} -nojvm -nosplash -nodesktop -wait -r "out = test_${_TEST_NAME}; if (out == 0) exit(0); else exit(1); end")
    ENDFUNCTION()
  ENDIF()
ENDIF()

IF(BUILD_PYTHON_BINDINGS)
  IF(NOT OPENMA_BUILD_SHARED_LIBS)
    MESSAGE(FATAL_ERROR "OpenMA must be compiled as shared libraries to build the Python binding")
  ENDIF()
  
  FIND_PACKAGE(PythonLibs REQUIRED)
  FIND_PACKAGE(NumPy REQUIRED)
  IF(BUILD_UNIT_TESTS)
    FUNCTION(ADD_PYTHON_UNITTEST_DRIVER)
      # Parse the arguments
      SET(options )
      SET(oneValueArgs NAME)
      SET(multiValueArgs SOURCES INCLUDES)
      CMAKE_PARSE_ARGUMENTS(_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
      # Extra unparsed arguments?
      LIST(LENGTH _TEST_UNPARSED_ARGUMENTS _TEST_NUM_UNPARSED_ARGUMENTS)
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 0)
        IF (NOT _TEST_NAME)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 0 _TEST_NAME)
        ENDIF()
      ENDIF()
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 1)
        IF (NOT _TEST_SOURCES)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 1 _TEST_SOURCES)
        ENDIF()
      ENDIF()
      IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 2)
        IF (NOT _TEST_INCLUDES)
          LIST(GET _TEST_UNPARSED_ARGUMENTS 2 _TEST_INCLUDES)
        ENDIF()
      ENDIF ()
      # Create the test and register it
      #  - Generate string for the include paths
      SET(_TEST_INCLUDE_PATHS "")
      LIST(LENGTH _TEST_INCLUDES _TEMP_NUM)
      MATH(EXPR _TEMP_NUM "${_TEMP_NUM}-1")
      FOREACH(_inc RANGE ${_TEMP_NUM})
        LIST(GET _TEST_INCLUDES ${_inc} _TEST_INCLUDE_PATH)
        IF(${_inc} GREATER 0)
          SET(_TEST_INCLUDE_PATHS "${_TEST_INCLUDE_PATHS}, ")
        ENDIF()
        SET(_TEST_INCLUDE_PATHS "${_TEST_INCLUDE_PATHS}'${_TEST_INCLUDE_PATH}'")
      ENDFOREACH()
      #  - Generate string for the source files
      SET(_TEST_SOURCE_NAMES "")
      LIST(LENGTH _TEST_SOURCES _TEMP_NUM)
      MATH(EXPR _TEMP_NUM "${_TEMP_NUM}-1")
      FOREACH(_inc RANGE ${_TEMP_NUM})
        LIST(GET _TEST_SOURCES ${_inc} _TEST_SOURCE_FILE)
        IF(${_inc} GREATER 0)
          SET(_TEST_SOURCE_NAMES "${_TEST_SOURCE_NAMES}, ")
        ENDIF()
        SET(_TEST_SOURCE_NAMES "${_TEST_SOURCE_NAMES}'${_TEST_SOURCE_FILE}'")
      ENDFOREACH()
      CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/cmake/templates/PythonTestDriver.py.in" "${EXECUTABLE_OUTPUT_PATH}/test_${_TEST_NAME}.py")
      ADD_TEST(NAME ${_TEST_NAME} WORKING_DIRECTORY ${OPENMA_EXECUTABLE_PATH} COMMAND ${PYTHON_EXECUTABLE} "test_${_TEST_NAME}.py")
    ENDFUNCTION()
  ENDIF()
ENDIF()