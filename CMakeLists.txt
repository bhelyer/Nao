cmake_minimum_required(VERSION 3.17)
project(Nao)

set(CMAKE_CXX_STANDARD 17)

add_executable(Nao main.cpp Nao/PlaceValue.cpp Nao/PlaceValue.h Nao/Point.cpp Nao/Point.h Nao/Board.cpp Nao/Board.h Nao/Game.cpp Nao/Game.h Nao/MinimaxAI.cpp Nao/MinimaxAI.h)