#ifndef HEXVALIDATOR_H
#define HEXVALIDATOR_H

#include <QValidator>
#include <QDebug>

class HexValidator : public QValidator
{
public:
    HexValidator();
    QValidator::State validate(QString &input, int &pos) const;
};

class Base36Validator : public QValidator
{
public:
    Base36Validator();
    QValidator::State validate(QString &input, int &pos) const;
};


#endif // HEXVALIDATOR_H
