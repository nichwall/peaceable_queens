#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

// POSIX compliant system call to get output on stdout
// Obtained from
// https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd);

#endif
