#pragma once

#include <iostream>
using namespace std;

#define DEBUG_LOG(msg) do { \
    cout << msg << "\t[" << __FUNCTION__ << ":" << __LINE__ << "]" << endl; \
} while (0)

#define WARN_LOG(msg) do { \
    cerr << msg << "\t[" << __FUNCTION__ << ":" << __LINE__ << "]" << endl; \
} while (0)

#define ERROR_LOG(msg) do { \
    cerr << msg << "\t[" << __FUNCTION__ << ":" << __LINE__ << "]" << endl; \
} while (0)
