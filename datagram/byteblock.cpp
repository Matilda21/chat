#include "message.h"

quint16 ByteBlock::get_key()
{
    quint16 key;
    QDataStream stream(&block, QIODevice::ReadOnly);
    stream >> key;
    return key;
}

bool ByteBlock::is_msg()
{
    return (get_key() != REC_MSG);
}
bool ByteBlock::is_rec()
{
    return get_key() == REC_MSG;
}

bool ByteBlock::is_signal()
{
    return get_key() == SIGNAL_MSG;
}

ByteBlock::ByteBlock(const QByteArray &_block)
{
    block = _block;
    QDataStream out(&block, QIODevice::ReadOnly);
    out >> key;
}

QByteArray ByteBlock::toByteArray() const
{
    return block;
}

void ByteBlock::resize(int size)
{
    block.resize(size);
}

char *ByteBlock::data()
{
    return block.data();
}

const char *ByteBlock::data() const
{
    return block.data();
}

int ByteBlock::size() const
{
    return  block.size();
}
