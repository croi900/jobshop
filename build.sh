#!/bin/bash

premake5 gmake2 && rm -rf obj && make -j32 && ./bin/Debug/Internship