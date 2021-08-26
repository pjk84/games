#ifndef GAME
#define GAME
#include<map>
#include<list>
#include<vector>
#include<string>

namespace TheGame{


    class Block{
    public:
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
        std::vector<int> blockColors;
        // std::vector<std::vector<int>> _blocks;
        int _timeout = 50;
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
        bool _hasActiveBlock = false;
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
        bool checkCollision(std::vector<int>);
        void drawBlocks();
        void printBoard();
        void handleKeyboardInput();
        void test();
    };
};


#endif