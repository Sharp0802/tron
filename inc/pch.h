#ifndef PCH_H
#define PCH_H

#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <utility>
#include <unordered_map>
#include <vector>

#if _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#endif

#endif //PCH_H
