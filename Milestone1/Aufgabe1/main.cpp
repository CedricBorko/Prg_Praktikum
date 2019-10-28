#include <QCoreApplication>
#include <GameOfLife.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GameOfLife *gol = new GameOfLife(30, 30);

    gol->PrintMatrix();

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
                    std::cout << "x ranges from 0 to " <<  gol->GetWidth() - 1 << ", y ranges from 0 to " << gol->GetHeight() - 1 << std::endl;
                    std::cout << "> ";

                    int x, y, status;

                    std::cin >> x >> y >> status;

                    // Boundry check

                    gol->SetCellState(x, y, status);

                    gol->PrintMatrix();

                    break;
                case '2':
                    gol->NewGeneration();
                    gol->PrintMatrix();
                    std::cout << "There are currently " << aliveCells << " cells alive.\n";

                    break;
                case '3':
                    std::cout << "Enter the full path to your text file (C:\\\\Users\\\\Example\\\\Desktop\\\\example.txt)." << std::endl;
                    std::cout << "> ";

                    std::cin >> filename;
                    gol->LoadFile(filename);
                    gol->PrintMatrix();

                    break;
                case '4':
                    std::cout << "Enter the file name: 'example.txt'." << std::endl;
                    std::cout << "> ";

                    std::cin >> filename;
                    gol->SaveToFile(filename);
                    gol->PrintMatrix();

                    break;
                default:
                    std::cout << "Invalid input" << std::endl;
                    gol->PrintMatrix();

                    break;

            }


        }while (true);

    return a.exec();
}
