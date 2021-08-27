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
        int _score = 0;
        int _level = 1;
        std::vector<int> _fullRows;
        std::vector<int> blockColors;
        // std::vector<std::vector<int>> _blocks;
        int _timeout = 50;
        int _buffer = 1000;
        int _tick = 0;
        std::string _ch = " ";
        // wchar_t _ch = U'🟨'; // figure out how to make this character work
        int _locX = 1;
        int _locY = 1;
        int _blockHeight = 2;
        int counter;
        int _key;
        std::list<int> keysPressed;
        bool _gameOver = false;
        std::vector<Block> _blocks;
        std::vector<Block> _activeBlocks;
        int _test = 0;
        std::string _testStr;
        int _center;

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
        int getRandomNumber(int range);
        void makeBlock();
        void moveBlock(std::vector<int> delta);
        bool checkCollisionSimple(std::vector<int>);
        bool checkCollisionRotation(std::vector<std::vector<int>>);
        void drawBlocks();
        void printBoard();
        void deleteRow();
        void shiftBlocks();
        void handleKeyboardInput();
        void setScore();
    };
};


#endif