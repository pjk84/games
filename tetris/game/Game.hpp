#ifndef GAME
#define GAME
#include<map>
#include<list>
#include<vector>
#include<string>

namespace TheGame{


    class Block{
    public:
        bool _slide = false;
        int _slideCounter = 0;
        char _shapeType;
        std::string _blockType;
        bool _isMoving = true;
        std::vector<int> _coords;
        int _color;
        Block(std::vector<int> coords, int color, char shapeType);
        void setCoords(int x, int y);
    };
    
    
    class Game {
    private:
        int _height, _width, _offsetX, _offsetY, _max_height, _header_height, _max_y;
        int c;
        bool _paused = false;
        int _score = 0;
        int _level = 1;
        bool _showAssistor = false;
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
        bool _gameOver = false;
        std::vector<Block> _blocks;
        std::vector<Block> _activeBlocks;
        std::vector<Block> _assistorBlocks;
        std::string _t;
        int _center;
        std::string _instructions = 
                    "- move blocks: arrow keys \n" 
                    + std::string("- rotate: space bar \n") 
                    + std::string("- toggle helper: a \n")
                    + std::string("- pauze game: p \n")
                    + std::string("- rotate: space bar \n");


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
        void projectAssistor();
    };
};


#endif