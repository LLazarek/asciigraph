/**************************************************/
/* Author: Lukas Lazarek                          */
/* Copyright (C) 2016 Lukas Lazarek               */
/* Please see LICENSE.txt for full license info   */
/**************************************************/

#ifndef ASCIIGRAPH_EXCEPT
#define ASCIIGRAPH_EXCEPT

class invalid_data : public std::runtime_error {
public:
  explicit invalid_data(const std::string &str) : std::runtime_error(str) {}
};

class file_not_found : public std::runtime_error {
public:
  explicit file_not_found(const std::string &str) : std::runtime_error(str) {}
};

#endif
