#include "Board.h"

using namespace std;
using namespace bangtal;

BoardPtr Board::create(
    const string& _img,
    ScenePtr _scene,
    int _x, int _y, bool _shown,
    int _status
) {
    auto object = BoardPtr(new Board(_img, _scene, _x, _y, _shown, _status));
    Object::add(object);

    return object;
};

Board::Board(
    const string& _img,
    ScenePtr _scene,
    int _x, int _y, bool _shown,
    int _status
) : Object(_img, _scene, _x, _y, _shown), status(_status) {};

const int Board::getStatus() { return status; };
void Board::setStatus(int _status) { status = _status; };
const int Board::getSize() { return size; };
void Board::setSize(int _size) { size = _size; };
