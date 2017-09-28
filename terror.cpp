
#include "terror.h"

TError *TError::_current = 0;

TError::TError(const QString &msg):
    _msg(msg)
{
}

void TError::SetCurrent(TError *value)
{
    if (_current)
        delete _current;

    _current = value;
}
