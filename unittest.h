#ifndef UNITTEST_H
#define UNITTEST_H

#include <QObject>

class UnitTest : public QObject
{
    Q_OBJECT
private:
    bool myCondition();

public:
    explicit UnitTest(QObject *parent = nullptr);
private slots:
    void initTestCase();
    void myFirstTest();
    void mySecondTest();
    void cleanupTestCase()
        {
            qDebug("Called after myFirstTest and mySecondTest.");
        }

signals:

};

#endif // UNITTEST_H
