#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>

class Message
{
public:
    int cols;
    int rows;
    int len;
    QString sb;

    Message(int cols, int rows, int len, QString sb){
        this->cols = cols;
        this->rows = rows;
        this->len = len;
        this->sb = sb;
    }
};

#endif // MESSAGE_H
