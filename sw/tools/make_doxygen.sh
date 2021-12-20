#!/bin/sh

mkdir -p ../doxygen
doxygen ../doc/doxyfile 2>../doxygen/warnings.txt
