#pragma once
#include <Object.h>

using namespace bangtal;
using namespace std;

class Board;
typedef std::shared_ptr<Board> BoardPtr;

class Board : public Object {
protected:
    Board(
        const string& _img,
        ScenePtr _scene,
        int _x, int _y, bool _shown,
        int _status
    );
    int status;
    int size = 0; // 물줄기의 길이
public:
    static BoardPtr create(
        const string& _img,
        ScenePtr _scene,
        int _x, int _y, bool _shown,
        int _status
    );

    virtual const int getStatus();
    virtual void setStatus(int _status);
    virtual const int getSize();
    virtual void setSize(int _size);

};