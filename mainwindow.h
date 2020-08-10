#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include<QFile>

#define EPROM_SIZE 4096

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



public slots:
    void openFile(void);
    void saveFile(void);
    void update(void);
    void doUpdate(void);



private:
    void readFile(QString fileName);
    void displayBinData(QByteArray *data );
    void displayBinData();
    void setup();
    void insertBinaryData(QByteArray temp,unsigned char channel);

    QString encodeMorse(QString c);

    QString decodeByte(unsigned char byte, unsigned char bits);
    QByteArray encodeString(QString c);
    void processBinaryData();
    QString getChar(QString morse_char);
    QString fileName;
    Ui::MainWindow *ui;
    QByteArray epromData;

    QStringList morse_string;
    QStringList char_string;
    QStringList out_morse_list;
    QString morse_msg_ch1;
    QString morse_msg_ch2;
    unsigned char bits=0;
    QString tmp;
    QString morse_char;
    int space_count;
    bool endofchar;


};





#endif // MAINWINDOW_H
