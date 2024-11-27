#include "inputmethodmgr.h"

class InputMethodMgr::CPrivate
{
public:
    CPrivate(InputMethodMgr* mParent)
        :mParent(mParent)
    {

    }

    InputMethodMgr *mParent;
};


InputMethodMgr::InputMethodMgr(QObject *parent)
    : QObject(parent)
    , md(new CPrivate(this))
{

}
