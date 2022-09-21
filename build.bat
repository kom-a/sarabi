@echo off

set root=%CD%
set src=mymoon/src
set out=bin
set int=bin-int

if not exist %out% mkdir %out%
if not exist %int% mkdir %int%

pushd "%src%"

nasm boot.asm -f bin -o %root%/%int%/boot.obj

popd