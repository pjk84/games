
#include "Game.hpp"
#include<ncurses.h>
#include<iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include<time.h>

using namespace TheGame;


// Block::Block(int size, int color){

// }
WINDOW *_win;

Game::Game(){
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

void Game::drop(){
    for(auto & b: _blocks){
        b.setCoords(b._coords[0]+1, b._coords[1]);
    }

}

void Game::test(){
    std::cout << 'test' << std::endl;
}

void Game::setupColor(){
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
}

void Game::startGame(){
    while(!_gameOver){
        _tick += 1;
        if(_tick == 1000/_timeout){
            _tick = 0;
            // drop(); 
        }
        if (!_hasActiveBlock){
            makeBlock();
        }
        printBoard();
        handleKeyboardInput();  
    }
}

int Game::getRandomColor(){
    srand(time(NULL));
    int colorId = rand() % 5 + 2;
    return colorId;
}
   

void Game::makeBlock(){
    _activeBlocks.clear();
    _hasActiveBlock=true;
    int colorId = getRandomColor();
    char blockType = 2;
    std::vector<std::vector<int>> blockCoords;
    auto pushBlock = [&] (){
        for (auto const& v: blockCoords){
            Block block(v, colorId);
            _blocks.push_back(block);
            _activeBlocks.push_back(block);
        }
    };
    switch(blockType){
        case 0:
        // I type
            blockCoords = {{1, _center -2}, {1, _center}, {1, _center+2}, {1, _center+4}};
            pushBlock();
            break;
        case 1:
        // S type
            blockCoords = {{1, _center -2}, {1, _center}, {2, _center}, {2, _center+2}};
            pushBlock();
            break;
        case 2:
        // T type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center}};
            pushBlock();
            break;
        case 3:
        // O type
            blockCoords = {{2, _center -2}, {2, _center}, {1, _center-2}, {1, _center}};
            pushBlock();
            break;
        case 4:
        // Z type
            blockCoords = {{2, _center -2}, {2, _center}, {1, _center+2}, {1, _center}};
            pushBlock();
            break;
        case 5:
        // J type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center-2}};
            pushBlock();
            break;
        case 6:
        // L type
            blockCoords = {{2, _center -2}, {2, _center}, {2, _center+2}, {1, _center+2}};
            pushBlock();
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
    wattron(_win, COLOR_PAIR(1));
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
            if (_locX + 2 > _width - 2) return;
            _locX += 2;
            break;
        case 258:
            if (_locY + 1 + _blockHeight > _height){
                return;
            }
            // move down
            _locY += 1;
            break;
        case 260:
            if (_locX - 2 < 1) return;
            // move left
            _locX -= 2;
            break;
    }
}

Block::Block(std::vector<int> coords, int color){
    _coords = coords;
    _color = color;
};

void Block::setCoords(int x, int y){
    _coords[0] = x;
    _coords[1] = y;
}



int main() {
    TheGame::Game g;
    return 1;
}