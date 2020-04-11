#pragma once
/**
 * @file common_headers.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief   common used headers
 * @version 0.1
 * @date 2020-04-11
 *
 * @copyright Copyright (c) 2020
 *
 */

#if (_MSC_VER >= 1920)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif
