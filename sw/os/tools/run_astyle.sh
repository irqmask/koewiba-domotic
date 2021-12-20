#!/bin/sh
astyle --options=astyle.cfg --recursive ../*.h
astyle --options=astyle.cfg --recursive ../*.c
astyle  --options=astyle.cfg --recursive ../*.cpp
