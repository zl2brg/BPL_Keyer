#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFile()));
    connect(ui->UpdateButton,SIGNAL(clicked()),this,SLOT(doUpdate()));

    setup();

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setup()
{
   morse_string << ".-"<< "-..."<< "-.-."<< "-.."<< "."<< "..-."<< "--." << "...."<< ".."<< ".---"<< "-.-"<< ".-.."<< "--" <<"-."<< "---" <<".--."<< "--.-"<< ".-."<< "..." <<"-"<< "..-"<< "...-"<< ".--"<< "-..-"<< "-.--"<< "--.."<< ".----"<< "..---"<< "...--"<< "....-"<< "....."<< "-...."<< "--..."<< "---.."<< "----." << "-----" << " ";
   char_string  <<"A"<< "B"<< "C"<< "D"<< "E"<< "F"<< "G"<< "H"<< "I"<< "J"<< "K"<< "L"<< "M"<< "N"<< "O"<< "P"<< "Q"<< "R"<< "S"<< "T"<< "U"<< "V"<< "W"<< "X"<< "Y"<< "Z"<< "1"<< "2"<< "3"<< "4"<< "5"<< "6"<< "7"<< "8"<< "9" << "0" << " ";
   readFile("/home/simon/keyer.bin");
}

void MainWindow::update(void){
    morse_msg_ch1="";
    displayBinData();
    processdData();
}

void MainWindow::doUpdate(void){
QByteArray temp;
temp = this->encodeString("ZL2BRG",0x03);
displayBinData(&temp);
binaryData = temp;
processdData();

}


QString MainWindow::getChar(QString morse_char){
    if (morse_string.contains(morse_char))
    {
        int index = morse_string.indexOf(morse_char);
        return char_string[index];
    }
    else qDebug() << "Unknown morse char" << morse_char;

        return "";
}

/* converts eprom byte stream to morse mark-space*/
QString MainWindow::get_unitlen(QString c){
    static QString result;
    static QString morse_msg;
    QString tmp_char;
    qDebug() << "decoding Morse packet" << c;
    if (c == "111")
    {
        tmp_char+="-";
    }
    else if (c =="1")
    {
        tmp_char+= ".";
    }
    return tmp_char;
}

QString  MainWindow::decodeByte(unsigned char byte, unsigned char bit)
{
    static QString tmp;
    static QString morse_char;
    static int space_count;
    QString morse_packet;
        qDebug() << "Decode" <<  byte << bit;
        if (byte & bit){
        tmp+="1";
        return 0;
        }

        /* got a 0 ==space so lets check what we have got */
        space_count++;
        if (tmp.length() > 0)
        {
            morse_char+=get_unitlen(tmp);
            tmp="";
            return 0;
        }
        else {
        if (space_count >= 3){
            tmp="";
            morse_packet+=getChar(morse_char);
            morse_packet+=" "; // insert space in output
            morse_char="";
            space_count=0;
        }
    }
        return morse_packet;
}


void MainWindow::processdData(){
    int x;
    morse_msg_ch1.clear();
    morse_msg_ch2.clear();
    for (x = 0; x < binaryData.size(); x++ ){
        morse_msg_ch1+=decodeByte(binaryData[x],0x01);
    }

    for (x = 0; x < binaryData.size(); x++ ){
        morse_msg_ch2+=decodeByte(binaryData[x],0x04);
    }

     ui->channel1_output->appendPlainText(morse_msg_ch1);
     ui->channel2_output->appendPlainText(morse_msg_ch2);

}

void MainWindow::openFile()
{

   qDebug() << "Load";
   fileName = QFileDialog::getOpenFileName(this,"Select Binary file",QDir::homePath());
   if (fileName.isEmpty()) return;
   readFile(fileName);
}




void MainWindow::readFile(QString fileName)
{

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        binaryData=file.readAll();
        file.close();
        update();

}

void MainWindow::displayBinData(QByteArray *data)
{
    int x;
    QString a;
    ui->binary_text->clear();
    for (x = 0; x < data->size(); x++ ){
     a += QString::asprintf("%02X ",(unsigned char) data->at(x));
    }
        ui->binary_text->appendPlainText(a);
}


void MainWindow::displayBinData()
{
    int x;
    QString a;
    ui->binary_text->clear();
    for (x = 0; x < binaryData.size(); x++ ){
     a += QString::asprintf("%02X ",(unsigned int)binaryData[x]);
    }
        ui->binary_text->appendPlainText(a);
}

/* Converts alphanumeric char to morse encoded string */
QByteArray MainWindow::encodeString(QString input, unsigned char bits){
QString morse_char,temp;
QString encoded_morse;
QByteArray output;

output.clear();

for (int len=0; len < input.length();len++)
{

    if (char_string.contains(input.at(len).toUpper()))
    {
        int index = char_string.indexOf(input.at(len).toUpper());
        morse_char = morse_string[index];
        qDebug() << "encode" << input.at(len).toUpper();
    }
    else{
        qDebug() << "Unknown char" << input.at(len);
    }

    encoded_morse = encodeMorse(morse_char,bits);
    qDebug() << "encoded Morse" << encoded_morse;
    for (int len= 0 ; len < encoded_morse.length();len++)
    {
        bool ok;
        temp = encoded_morse.at(len);
        unsigned char b = temp.toUInt(&ok,16);
        qDebug() << "b" << b << temp;
        output.append(b);

    }
    output.append(3,0);
  }

 return output;
}

/* converts morse encoded string to hex encoded string */
QString MainWindow::encodeMorse(QString c, unsigned char bits ){
    QString tmp_char="";
    int x = 0;
    qDebug() << "endecoding Morse packet" << c;
    while (x < c.length())
    {
        if (c.at(x) == "-")
        {
            tmp_char+="FFF0";
        }
        else if (c.at(x)=='.'){
            tmp_char+="F0";
        }
         x++;
       }
        return tmp_char;

}



void MainWindow::saveFile(void)
{


}
