#include "GameOfLife.h"

GameOfLife::GameOfLife(int dimX, int dimY):
        width(dimX),
        height(dimY)
{
    GameOfLife::SetSize(dimX, dimY);
}

GameOfLife::~GameOfLife()
{
    delete[] currentState;
    delete[] outputState;
}

void GameOfLife::SetCellState(int x, int y, int state)
{
    if (x >= 0 && x < width && y >= 0 && y < height && (state == 0 || state == 1))
    {
        currentState[x + y * width] = state;
    }
}

void GameOfLife::SetSize(int dimX, int dimY)
{

    width = dimX;
    height = dimY;

    currentState = new int[width * height]{0};
    outputState = new int[width * height]{0};

}

int GameOfLife::GetWidth() const
{
    return width;
}

int GameOfLife::GetHeight() const
{
    return height;
}

int GameOfLife::convertCoordinate(int x, int y)
{
    /* Converts a 2D coordinate to a 1D coordinate
    *  Makes changing and accesing cells easier
    */
    return outputState[y * width + x];
}

void GameOfLife::NewGeneration()
{
    for(int i = 0; i < width * height; i++){
            outputState[i] = currentState[i];     // Save the current state of all cells.
        }

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){


                    // Since each cell can have a value of 0 or 1 we can just add their values to get the living neighbours.

                    int neighbours = convertCoordinate((x - 1) % width, (y - 1) % height) + convertCoordinate(x + 0, (y - 1) % height) + convertCoordinate((x + 1) % width, (y - 1) % height)
                                   + convertCoordinate((x - 1) % width, y + 0)          +                  0                       + convertCoordinate((x + 1) % width, y + 0)
                                   + convertCoordinate((x - 1) % width, (y + 1) % height) + convertCoordinate(x, (y + 1) % height)     + convertCoordinate((x + 1) % width, (y + 1) % height);

                    /*  Game of Life Rules
                     *  Living cells with less than 2 living neighbours die due to loneliness.
                     *  Living cells with 2 or 3 living neighbours stay alive.
                     *  Living cells with more than 3 living neighbours die due to starvation.
                     *  Dead cells with exactly 3 living neighbours will be reborn.
                     */

                if(convertCoordinate(x, y) == 1) // If cell is alive
                    currentState[x + y * width] = neighbours == 2 || neighbours == 3 ? 1 : 0;
                else
                    currentState[x + y * width] = neighbours == 3;
            }

        }

}


void GameOfLife::LoadFile(std::string filename)
{
    std::ifstream inFile;

    inFile.open(filename);

    if (!inFile) {
            std::cout << "Unable to open file '" << filename << "'. Using existing board." << std::endl;
            return;
    }
    int dimX;
    int dimY;

    /* First we will read the first and second line to be able to resize our arrays
     * according to the size given in the textfile.
     */

    inFile >> dimX;
    inFile >> dimY;

    SetSize(dimX, dimY);

    std::string lines;
    std::string line;

    while(inFile >> line){
        lines.append(line);
    }

    inFile.close();

    for (int i = 0; i < dimX * dimY; i++){
        if (lines[i] == '*') currentState[i] = 1;
        else currentState[i] = 0;
    }

    std::cout << "File has been loaded." << std::endl;


}

bool doesFileExist(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}

void GameOfLife::SaveToFile(std::string filename)
{
    std::fstream file;

       /* Placeholder so I don't have to type the whole path.
       *  The file will be saved to a predefined folder.
       */

        std::string filepath = "C:\\Users\\Cedric\\Documents\\PRG_Praktikum\\Aufgabe1_textDateien\\" + filename;


       if(!doesFileExist(filepath)){
           std::cout << "File does not exist... Creating one at " << filepath << std::endl;
           std::ofstream myFile;
           myFile.open(filepath);
           std::cout << "Writing to file... " << filepath << std::endl;
           myFile << width << std::endl; // First line
           myFile << height << std::endl; // Second line

           for(int i = 0; i < width * height; i++){
               // The characters here are just the ones used in the given example file.

               if (currentState[i] == 1){
                   myFile  << '*';
               }
               else{
                   myFile  << 'o';
               }
               if(i % width == height - 1) myFile << std::endl;

           }

           std::cout << "Succes. File exists at " << filepath << "." << std::endl;
           myFile.close();

       }else{

           // Give the option to override or cancel.
           std::cout << "File already exists. Do you want to override it? (0) no (1) yes." << std::endl;
           int answer;
           std::cout << "> ";
           std::cin >> answer;

           if (answer == 0){ // No editing.
               std::cout << "File not overriden. Returning." << std::endl;
               return;
           }

           // 'trunc' makes sure we do not just appen to the file but clear it first.
           file.open(filepath, std::ios::out | std::ios::trunc);

           std::cout << "Writing to file... " << filepath << std::endl;
           file << width << std::endl;
           file << height << std::endl;

           for(int i = 0; i < width * height; i++){
               if (currentState[i] == 1){
                   file  << '*';
               }
               else{
                   file  << 'o';
               }
               if(i % width == width - 1) file << std::endl;
           }
           file.close();
           std::cout << "Succes. File exists at " << filepath << std::endl;
        }

}



void GameOfLife::PrintMatrix()
{
    for (int i = 0; i < 2 * width; i++){
                std::cout << "_";
        }

        std::cout << std::endl;
        std::cout << "Current State:" << std::endl;

        for(int i = 0; i < width * height; i++){
            if (currentState[i] == 1){
                std::cout << std::to_string(currentState[i]) << " ";
            }
            else{
                std::cout << "0" << " ";
            }
            if(i % width == width - 1) std::cout << std::endl;
        }
        for (int i = 0; i < 2 * width; i++){
                std::cout << "_";
        }
        std::cout << std::endl;

}
