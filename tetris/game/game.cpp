

#define _XOPEN_SOURCE_EXTENDED 1
#include "Game.hpp"
#include<ncurses.h>
#include<iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include<time.h>
#include <wchar.h>
#include <locale.h>
#include <cmath>

using namespace TheGame;

/*
TODO: 

- show shape landing position on stack
- levels

*/

WINDOW *_win;
WINDOW *_textbox;
WINDOW *_header;

Game::Game(){
    setlocale(LC_ALL, "");
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
    _textbox = newwin(_height / 2, _width, _offsetY, _width + _offsetX + 2);
    _header = newwin(3, _width, 0, _offsetX);
    refresh();
}

void Game::setupColor(){
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_WHITE);
}

void Game::startGame(){
    while(!_gameOver){
        _tick += 1;
        if(_tick == _buffer/_timeout){
            _tick = 0;
            if(_paused){
                continue;
            }
            drop();  
        }
        if (_activeBlocks.size() == 0){
            makeBlock();
        }
        if(_fullRows.size() > 0){
            shiftBlocks();
        }
        printBoard();
        handleKeyboardInput();  
    }
}

void Game::shiftBlocks(){
    // slide blocks in full rows to the right
    bool d = false;
    int slideStepSize = 8;
    for(auto & b: _blocks){
        if(b._coords[0] != _fullRows.back()){
            continue;
        }
        if(b._slideCounter >= _width - 2){
            d = true;
        }
        if(b._slide){
            b._slideCounter += slideStepSize;
            b.setCoords(b._coords[1] + slideStepSize , b._coords[0]);
        }
    }
    if(d){
        deleteRow();
    }
}

void Game::checkRows(){
    //check if there are any closed rows.
    // return sorted vector
    std::map<int, int> blocksByRow;
    for(auto & b: _blocks){
        int row = b._coords[0];
        int col = b._coords[1];
        if(blocksByRow.count(row) == 0){
            blocksByRow[row] = 0;
        }
        blocksByRow[row] += 1;
        if(blocksByRow[row] == (_width - 2) / 2){
            _fullRows.push_back(row);
        }
    }
    for(auto &b : _blocks){
        // set slide flag for animation
        if(std::find(_fullRows.begin(), _fullRows.end(), b._coords[0]) != _fullRows.end()){
            b._slide = true;
        }
    }
    sort(_fullRows.begin(), _fullRows.end(), std::greater<int>());
}

void Game::deleteRow(){
    // delete single row and collapse rows above it.
    std::vector<Block> remainingBlocks;
    int row = _fullRows.back();
    for(auto & b: _blocks){
        if( b._coords[0] == row){
            continue;
        }
        if (b._coords[0] < row){
            b._coords[0] += 1;
        }
        remainingBlocks.push_back(b);
    }
    _fullRows.pop_back();
    _blocks = remainingBlocks;
    setScore();
}

void Game::setScore(){
    _score += 1;
    if(_score % 5 == 0){
        if(_buffer > 50){
            _buffer -= 50;
            _level += 1;
        }
    }
}

int Game::getRandomNumber(int range){
    int r = rand() % range + 1;
    return r;
}

int Game::getBlockColor(){
    if(_blocks.size() == 0){
        return getRandomNumber(5);
    }
    int* colorId = &_blocks.back()._color;
    int color = *colorId;
    while(*colorId - color == 0){
        color = getRandomNumber(5);
    }
    return color;
}

   
void Game::makeBlock(){
    for(auto const & b: _activeBlocks){
        _blocks.push_back(b);
    }
    checkRows();
    _activeBlocks.clear();
    int colorId = getBlockColor();
    int blockType = getRandomNumber(7);
    std::vector<std::vector<int>> blockCoords;
    auto pushBlock = [&] (char shapeType){
        for (int i = 0; i < blockCoords.size(); i ++){
            Block block(blockCoords[i], colorId, shapeType);
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
            blockCoords = {{2, _center -2}, {1, _center-2},  {2, _center}, {1, _center}};
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
    if(_showAssistor){
     projectAssistor();
    }
}

void Game::projectAssistor(){
  // find lowest colliding y
  int d = _height;
  int i = 0;
  for(int i = 0; i < _activeBlocks.size(); i++){
      int deltaY = _height - _activeBlocks[i]._coords[0];
      if(deltaY < d){
          d = deltaY;
      }
      for(auto const & bb: _blocks){
          if(bb._coords[1] == _activeBlocks[i]._coords[1]){
              deltaY = (bb._coords[0] - _activeBlocks[i]._coords[0]);
              if(deltaY < d ){
                  d = deltaY;
              }
          }
      }
  }
  for(int i = 0; i < _activeBlocks.size(); i++){
      mvwprintw(_win, _activeBlocks[i]._coords[0] + (d - 1), _activeBlocks[i]._coords[1], _ch.c_str());
      mvwprintw(_win, _activeBlocks[i]._coords[0] + (d - 1), _activeBlocks[i]._coords[1]+1, _ch.c_str());
  }
}

void Game::printBoard(){
    counter += 1;
    werase(_win);
    werase(_textbox);
    werase(_header);
    // c = getch();
    std::string s = "" ;
    std::string s_concat = "key:" + std::to_string(_locY) + " locx:" + std::to_string(_locX);
    // string s_concat = to_string(c);
    char const *p = _paused ? _t.c_str() : (" score:" + std::to_string(_score) + " " + std::string(" level:") + std::to_string(_level)).c_str();
    wborder(_win, 0, 0, 0 , '_', 0, 0, 0, 0);
    box(_header, 0, 0);
    mvwprintw(_header, 1, 1, p);
    char const *instructions = _instructions.c_str();
    mvwprintw(_textbox, 1, 0, instructions);
    drawBlocks();
    wattron(_win, COLOR_PAIR(6));
    wrefresh(_win);
    wrefresh(_textbox);
    wrefresh(_header);
}

void Game::handleKeyboardInput(){
    int key = wgetch(_win);
    if (key == -1){
        return;
    }
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
            rotate();
            break;
        case 100:
            // d. fast drop
            fastDrop();
            break;
        case 97:
            // toggle assistor
            _showAssistor = !_showAssistor;
            break;
        case 112:
            // toggle assistor
            if(_paused){
                _t = "";
                _paused = false;
                break;
            }
            _t = "- game paused - ";
            _paused = true;
            break;
    }
}

void Game::moveBlock(std::vector<int> delta){
    if(_paused){
        return;
    }
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
    if(_paused){
        return;
    }
    bool collided = false;
    while(!collided){
        for(auto & b: _activeBlocks){
            // check for collisions first
            if(checkCollision({1, 0})){
                collided = true;
                break;
            }
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
    std::vector<int> axisPos = _activeBlocks[1]._coords;
    std::vector<std::vector<int>> newPositions;
    for(Block & b: _activeBlocks){
        if(b._shapeType == 'O'){
            // O type has no rotation
            break;
        }
        int deltaY = b._coords[0] - axisPos[0];
        int deltaX = b._coords[1] - axisPos[1];
        // on axis
        if(deltaX == 0){
            newPositions.push_back({axisPos[1] + (deltaY * -2), axisPos[0]});
            continue;
        }
        if(deltaY == 0){
            newPositions.push_back({axisPos[1], int(axisPos[0] + std::ceil(deltaX / 2))});
            continue;
        }

        //off axis
        newPositions.push_back({axisPos[1] - (deltaY*2), int(axisPos[0] + (std::ceil(deltaX/2)))});
    }
    if (checkCollisionRotation(newPositions)){
        return;
    }
    for(int i = 0; i < newPositions.size(); i++){
        _activeBlocks[i].setCoords(newPositions[i][0], newPositions[i][1]);
    }
}

bool Game::checkCollisionRotation(std::vector<std::vector<int>> newPositions){
    for(auto & p: newPositions){
        // hits walls
        if(p[0] < 0 || p[0] > _width - 2){
            return true;
        }
        // hits bottom
        if(p[1] < 0 || p[1] > _height - 1){
            return true;
        }
        // hits stack
        for(auto & b: _blocks){
            if(b._coords[0] == p[1] && b._coords[1] == p[0]){
                return true;
            }
        }
    }
}

bool Game::checkCollision(std::vector<int> delta){
    // check for collisions on horizontal/vertical movement
        for (auto & b: _activeBlocks){
            // walls
            if( b._coords[1] + delta[1] <= 0 || b._coords[1] + delta[1] >= _width - 2){
                return true;
            }
            // bottom
            if(b._coords[0] + delta[0] >= _height){
                // block hits bottom of grid
                makeBlock();
                return true;
            }
            // stack
        for (auto & bb: _blocks){
            if(b._coords[0] + delta[0] == bb._coords[0] && b._coords[1] + delta[1] == bb._coords[1]){
                if(delta[1] == 0){
                    // block lands on stack.
                        makeBlock();
                    return true;
                }
                return true;
            }
        }
    }
    return false;
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