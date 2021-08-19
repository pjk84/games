
#include "Game.hpp"
#include<ncurses.h>
#include<iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include<time.h>

using namespace TheGame;

/*
TODO: 

1) shape rotation
2) stack collission
3) line removal
4) levels

*/

WINDOW *_win;

Game::Game(){
    srand(time(0));
    initscr();
    if (!has_colors()) return;
    start_color();
    setupColor();
    noecho();
    setupWindow();
    keypad(_win, TRUE);
    curs_set(0);
    cbreak();
    wtimeout(_win, _timeout);
    startGame();
    endwin(); 
}

void Game::setupWindow(){
    _max_height = 40;
    _header_height = 2;
    _max_y = getmaxy(stdscr) - 2;
    
    if (_max_y > _max_height){
        _max_y = _max_height;
    }
    _height = _max_y;
    _width = _height;
    _center = _width / 2;
    if (_width % 2 != 0){
        _width += 1;
    }
    _offsetX = 10;
    _offsetY = 2;

    _win = newwin(_height, _width, _offsetY, _offsetX);
    refresh();
}

void Game::test(){
    std::cout << 'test' << std::endl;
}

void Game::setupColor(){
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
}

void Game::startGame(){
    while(!_gameOver){
        _tick += 1;
        if(_tick == 1000/_timeout){
            _tick = 0;
            drop(); 
        }
        if (!_hasActiveBlock){
            makeBlock();
        }
        printBoard();
        handleKeyboardInput();  
    }
}

int Game::getRandomNumber(int range){
    int r = rand() % range + 1;
    _test = rand() % range + 1;
    return r;
}
   

void Game::makeBlock(){
    for(auto const & b: _activeBlocks){
        _blocks.push_back(b);
    }
    _activeBlocks.clear();
    _hasActiveBlock=true;
    int colorId = getRandomNumber(5);
    int blockType = getRandomNumber(7);
    std::vector<std::vector<int>> blockCoords;
    auto pushBlock = [&] (char shapeType){
        for (auto const& v: blockCoords){
            Block block(v, colorId, shapeType);
            _activeBlocks.push_back(block);
        }
    };
    switch(blockType){
        case 1:
        // I type
            blockCoords = {{1, _center -2}, {1, _center}, {1, _center+2}, {1, _center+4}};
            pushBlock('I');
            break;
        case 2:
        // z type
            blockCoords = {{1, _center -2}, {1, _center}, {2, _center}, {2, _center+2}};
            pushBlock('Z');
            break;
        case 3:
        // T type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center}};
            pushBlock('T');
            break;
        case 4:
        // O type
            blockCoords = {{2, _center -2}, {2, _center}, {1, _center-2}, {1, _center}};
            pushBlock('O');
            break;
        case 5:
        // s type
            blockCoords = {{2, _center -2}, {2, _center}, {1, _center+2}, {1, _center}};
            pushBlock('S');
            break;
        case 6:
        // J type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center-2}};
            pushBlock('J');
            break;
        case 7:
        // L type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center+2}};
            pushBlock('L');
            break;
        
    }
}

void Game::drawBlocks(){
    // iterate blocks vector. draw each block by coords with color
    char const *block = _ch.c_str();
    for(Block const& b: _blocks){
        wattron(_win, COLOR_PAIR(b._color));
        mvwprintw(_win, b._coords[0], b._coords[1], block);
        mvwprintw(_win, b._coords[0], b._coords[1]+1, block);
    }
    for(Block const& b: _activeBlocks){
        wattron(_win, COLOR_PAIR(b._color));
        mvwprintw(_win, b._coords[0], b._coords[1], block);
        mvwprintw(_win, b._coords[0], b._coords[1]+1, block);
    }
}

void Game::printBoard(){
    counter += 1;
    werase(_win);
    // c = getch();
    std::string s = "" ;
    std::string s_concat = "key:" + std::to_string(_locY) + " locx:" + std::to_string(_locX);
    // string s_concat = to_string(c);
    char const *p = std::to_string(_test).c_str();
    box(_win, 0, 0); 
    mvwprintw(_win, 1, 1, p);
    drawBlocks();
    wattron(_win, COLOR_PAIR(6));
    wrefresh(_win);
}

void Game::handleKeyboardInput(){
    int key = wgetch(_win);
    if (key == -1){
        return;
    }
    _key = key;
    switch(key){
        case 261:
            // move right
            moveBlock({0, 2});
            break;
        case 258:
            //move down
            moveBlock({1, 0});
            break;
        case 260:
            // move left
            moveBlock({0, -2});
            break;
        case 32:
            // space. rotate
            break;
            rotate();
        case 100:
            // d. fast drop
            fastDrop();
            break;
    }
}

void Game::moveBlock(std::vector<int> delta){
    if(checkCollision(delta)){
        return;
    }
    for (auto & b: _activeBlocks){
        int newX = b._coords[1] += delta[1];
        int newY = b._coords[0] += delta[0];
        b.setCoords(newX, newY);
    }
}

void Game::fastDrop(){
    bool collided = false;
    while(!collided){
        if(checkCollision({1, 0})){
            collided = true;
            break;
        }
        // if no collission found, drop 1 row
        for(auto & b: _activeBlocks){
            b.setCoords(b._coords[1], b._coords[0] + 1);
        }
    }
    return;
}

void Game::drop(){
    if(checkCollision({1, 0})){
         // hit bottom
        return;
    }
    for(auto & b: _activeBlocks){
        b.setCoords(b._coords[1], b._coords[0]+1);
    }
}

void Game::rotate(){
    for(Block const& b: _activeBlocks){
        if(b._shapeType == 'O'){
            // O type has no rotation
            break;
        }
        if(b._shapeType){
            break;
        }
    }
}

void checkRows(){
    // test for compeleted rows. delete if found.
    
}

bool Game::checkCollision(std::vector<int> delta){
    bool collides = false;
    for (auto & b: _activeBlocks){
        // check for horizontal collision
        if( b._coords[1] + delta[1] <= 0 || b._coords[1] + delta[1] >= _width - 2){
            return true;
        }
        // vertical
        if(b._coords[0] + delta[0] >= _height){
            // block hit bottom
            makeBlock();
            return true;
        }
       // stack
       for (auto & bb: _blocks){
           if(b._coords[0] + delta[0] == bb._coords[0] && b._coords[1] + delta[1] == bb._coords[1]){
               makeBlock();
               checkRows();
               return true;
           }
       }

    }
}

Block::Block(std::vector<int> coords, int color, char shapeType){
    _shapeType = shapeType;
    _coords = coords;
    _color = color;
};

void Block::setCoords(int x, int y){
    _coords[0] = y;
    _coords[1] = x;
}


int main() {
    TheGame::Game g;
    return 1;
}