#ifndef GAME
#define GAME
#include<map>
#include<list>
#include<vector>
#include<string>
#include<array>

namespace TheGame{


    class Block{
    public:
        bool _slide = false;
        int _slideCounter = 0;
        char _shapeType;
        std::string _blockType;
        bool _isMoving = true;
        std::array<int, 2> _coords;
        int _color;
        Block(std::array<int, 2> coords, int color, char shapeType);
        void setCoords(int x, int y);
    };
    
    
    class Game {
    private:
        bool _quit = false;
        bool _gameOver = false;
        int _height, _width, _offsetX, _offsetY, _max_height, _header_height, _max_y;
        int c;
        bool _paused = false;
        int _score = 0;
        int _level = 1;
        bool _showHelper = false;
        std::vector<int> _fullRows;
        std::vector<int> blockColors;
        // std::vector<std::vector<int>> _blocks;
        int _timeout = 50;
        int _buffer = 1000;
        int _tick = 0;
        std::string _ch = " ";
        int _locX = 1;
        int _locY = 1;
        int _blockHeight = 2;
        int counter;
        int _key;
        std::list<int> keysPressed;
        std::vector<Block> _blocks;
        std::vector<Block> _activeBlocks;
        std::vector<Block> _assistorBlocks;
        std::string _t;
        int _test;
        int _center;
        std::string _instructions = 
                    "controls: \n\n" 
                    "- arrow keys: move block \n" 
                    + std::string("- SPACE: rotate \n") 
                    + std::string("- D: fast drop \n") 
                    + std::string("- A: toggle helper\n")
                    + std::string("- P: pauze game\n");


    public:
        Game();
        void rotate();
        void setupWindow();
        void setupColor();
        void startGame();
        void fastDrop();
        int getBlockColor();
        void drop();
        void checkRows();
        int getRandomNumber(int);
        void makeBlock();
        void moveBlock(std::vector<int>);
        bool checkCollision(std::vector<int>);
        bool checkCollisionRotation(std::vector<std::vector<int>>);
        void drawBlocks();
        void printBoard();
        void deleteRow();
        void shiftBlocks();
        void handleKeyboardInput();
        void setScore();
        void renderHelperBlock();
    };
};


#endif