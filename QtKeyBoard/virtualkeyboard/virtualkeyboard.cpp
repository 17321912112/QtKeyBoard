#include "virtualkeyboard.h"

class VirtualKeyBoard::CPrivate
{
public:
    CPrivate(VirtualKeyBoard* parent)
        : mParent(parent)
    {

    }

    VirtualKeyBoard* mParent;

};

VirtualKeyBoard::VirtualKeyBoard(QObject *parent)
    : QObject(parent)
    , md(new CPrivate(this))
{

}

LangeuageLayout *VirtualKeyBoard::GetInstance()
{
    if (!mKeyBoard)
    {
        mKeyBoard = new LangeuageLayout();
    }
    return mKeyBoard;
}
