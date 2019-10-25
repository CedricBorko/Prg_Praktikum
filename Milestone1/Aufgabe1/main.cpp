#include <QCoreApplication>
#include <QFile>
#include <fstream>
#include <iostream>
#include <random>




int dimensionX = 30;
int dimensionY = 30;


int *outputData = new int[dimensionX * dimensionY]{0};
int *currentData = new int[dimensionX * dimensionY]{0};


void initBoard(int dimX, int dimY){
    for(int i = 0; i < dimX * dimY; i++){
        currentData[i] = rand() % 2;
    }
}

void printMatrix(int dimX, int dimY){

    for (int i = 0; i < 2 * dimX; i++){
            std::cout << "_";
    }

    std::cout << std::endl;
    std::cout << "Current State:" << std::endl;

    for(int i = 0; i < dimX * dimY; i++){
        if (currentData[i] == 1){
            std::cout << std::to_string(currentData[i]) << " ";
        }
        else{
            std::cout << "0" << " ";
        }
        if(i % dimX == dimX - 1) std::cout << std::endl;
    }
    for (int i = 0; i < 2 * dimX; i++){
            std::cout << "_";
    }
    std::cout << std::endl;

}




int convertCoordinate(int x, int y){
    /* Converts a 2D coordinate to a 1D coordinate
    *  Makes changing and accesing cells easier
    */
    return outputData[y * dimensionX + x];
}


void resizeArray(int width, int height){

    dimensionX = width;    // new width
    dimensionY = height;   // new height

    delete[] currentData;
    delete[] outputData;

    currentData = new int[dimensionX * dimensionY]{0};
    outputData = new int[dimensionX * dimensionY]{0};

}

void loadFile(std::string filename){

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

    resizeArray(dimX, dimY);

    std::string lines;
    std::string line;

    while(inFile >> line){
        lines.append(line);
    }

    inFile.close();

    for (int i = 0; i < dimX * dimY; i++){
        if (lines[i] == '*') currentData[i] = 1;
        else currentData[i] = 0;
    }

    std::cout << "File has been loaded." << std::endl;

}


bool doesFileExist(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}

void saveToFile(std::string filepath){

   std::fstream file;

   /* Placeholder so I don't have to type the whole path.
   *  The file will be saved to a predefined folder.
   */

    filepath = "C:\\Users\\Cedric\\Documents\\PRG_Praktikum\\Aufgabe1_textDateien\\" + filepath;


   if(!doesFileExist(filepath)){
       std::cout << "File does not exist... Creating one at " << filepath << std::endl;
       std::ofstream myFile;
       myFile.open(filepath);
       std::cout << "Writing to file... " << filepath << std::endl;
       myFile << dimensionX << std::endl; // First line
       myFile << dimensionY << std::endl; // Second line

       for(int i = 0; i < dimensionX * dimensionY; i++){
           // The characters here are just the ones used in the given example file.

           if (currentData[i] == 1){
               myFile  << '*';
           }
           else{
               myFile  << 'o';
           }
           if(i % dimensionX == dimensionX - 1) myFile << std::endl;

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
       file << dimensionX << std::endl;
       file << dimensionY << std::endl;

       for(int i = 0; i < dimensionX * dimensionY; i++){
           if (currentData[i] == 1){
               file  << '*';
           }
           else{
               file  << 'o';
           }
           if(i % dimensionX == dimensionX - 1) file << std::endl;
       }
       file.close();
       std::cout << "Succes. File exists at " << filepath << std::endl;
    }
}




void calcNewGeneration(int dimX, int dimY){

    for(int i = 0; i < dimX * dimY; i++){
        outputData[i] = currentData[i];     // Save the current state of all cells.
    }

    for(int y = 0; y < dimY; y++){
        for(int x = 0; x < dimX; x++){


                // Since each cell can have a value of 0 or 1 we can just add their values to get the living neighbours.

                int neighbours = convertCoordinate((x - 1) % dimX, (y - 1) % dimY) + convertCoordinate(x + 0, (y - 1) % dimY) + convertCoordinate((x + 1) % dimX, (y - 1) % dimY)
                               + convertCoordinate((x - 1) % dimX, y + 0)          +                  0                       + convertCoordinate((x + 1) % dimX, y + 0)
                               + convertCoordinate((x - 1) % dimX, (y + 1) % dimY) + convertCoordinate(x, (y + 1) % dimY)     + convertCoordinate((x + 1) % dimX, (y + 1) % dimY);

                /*  Game of Life Rules
                 *  Living cells with less than 2 living neighbours die due to loneliness.
                 *  Living cells with 2 or 3 living neighbours stay alive.
                 *  Living cells with more than 3 living neighbours die due to starvation.
                 *  Dead cells with exactly 3 living neighbours will be reborn.
                 */

            if(convertCoordinate(x, y) == 1) // If cell is alive
                currentData[x + y * dimensionX] = neighbours == 2 || neighbours == 3 ? 1 : 0;
            else
                currentData[x + y * dimensionX] = neighbours == 3;
        }

    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initBoard(dimensionX, dimensionY);
    printMatrix(dimensionX, dimensionY);

    do{
        std::cout << "0. Exit" << std::endl;
        std::cout << "1. Change cell" << std::endl;
        std::cout << "2. Evolve" << std::endl;
        std::cout << "3. Load file" << std::endl;
        std::cout << "4. Save file" << std::endl;


        std::cout << "> ";
        std::string filename = "";
        char userInput;
        int aliveCells = 0;
        std::cin >> userInput;

        switch (userInput) {
            case '0':
                return 0;
            case '1':
                std::cout << "Enter cell coordinates and if the cell should be (0) living or (1) dead like this: 'x y status'." << std::endl;
                std::cout << "x ranges from 0 to " << dimensionX - 1 << ", y ranges from 0 to " << dimensionY - 1 << std::endl;
                std::cout << "> ";

                int x, y, status;

                std::cin >> x >> y >> status;

                // Boundry check

                if (x >= 0 && x < dimensionX && y >= 0 && y < dimensionY && (status == 0 || status == 1)){
                    currentData[x + y * dimensionX] = status;
                }

                printMatrix(dimensionX, dimensionY);

                break;
            case '2':
                calcNewGeneration(dimensionX, dimensionY);
                printMatrix(dimensionX, dimensionY);
                for(int i = 0; i < dimensionX * dimensionY; i++){
                    if (currentData[i] == 1){
                        aliveCells++;
                    }
                }
                std::cout << "There are currently " << aliveCells << " cells alive.\n";

                break;
            case '3':
                std::cout << "Enter the full path to your text file (C:\\\\Users\\\\Example\\\\Desktop\\\\example.txt)." << std::endl;
                std::cout << "> ";

                std::cin >> filename;
                loadFile(filename);
                printMatrix(dimensionX, dimensionY);

                break;
            case '4':
                std::cout << "Enter the file name: 'example.txt'." << std::endl;
                std::cout << "> ";

                std::cin >> filename;
                saveToFile(filename);
                printMatrix(dimensionX, dimensionY);

                break;
            default:
                std::cout << "Invalid input" << std::endl;
                printMatrix(dimensionX, dimensionY);

                break;

        }


    }while (true);


    return a.exec();
   }
