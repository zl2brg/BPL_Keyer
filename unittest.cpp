#include "unittest.h"
#include <QTest>



UnitTest::UnitTest(QObject *parent) : QObject(parent)
{
 qDebug() << "Unit Test";

}


bool UnitTest::myCondition()
    {
        return true;
    }


void UnitTest::initTestCase()
{
      qDebug("Called before everything else.");
}

void UnitTest::myFirstTest()
   {
       QVERIFY(true); // check that a condition is satisfied
       QCOMPARE(1, 1); // compare two values
   }

   void UnitTest::mySecondTest()
   {
       QVERIFY(myCondition());
       QVERIFY(1 != 2);
   }
