#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <iostream>
#include <fstream>

class GameOfLife
{
public:

    GameOfLife(int width, int height);

    void PrintMatrix();

    void SetCellState(int x, int y, int status);

    void SetSize(int dimX, int dimY);

    void NewGeneration();

    ~GameOfLife();

    void LoadFile(std::string);

    void SaveToFile(std::string);

    int convertCoordinate(int x, int y);

    int GetWidth() const;

    int GetHeight() const;

private:

    int width, height;

    int *currentState;
    int *outputState;





};

#endif // GAMEOFLIFE_H
