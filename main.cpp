#include "widget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget mywidget;
    mywidget.setWindowTitle("复合外设 Methane-XVI 配置程序 V2.2");//设置窗口名称
    mywidget.show();
    mywidget.setFocusPolicy(Qt::StrongFocus);
    
    setbuf(stdout, NULL);//设置printf缓冲为0,使立即输出
    
    if(argc == 2){//若从配置文件打开程序
        QString auto_fileName = QString::fromLocal8Bit(argv[1]);//获取配置文件路径
        //qDebug()<<auto_fileName<<endl;
        if(auto_fileName.length() >= 2){
            mywidget.fileNow = "";
            for(int i = 0; i < auto_fileName.length(); i++){
                if(auto_fileName[i] == '\\' && i + 1 < auto_fileName.length()){//斜杠转换
                    if(auto_fileName[i + 1] == '\\'){
                        mywidget.fileNow += '/';
                        i++;
                    }
                    else mywidget.fileNow += auto_fileName[i];
                }
                else mywidget.fileNow += auto_fileName[i];
            }
            mywidget.openCfgFile();//打开配置文件
        }
    }
    
    a.exec();
    
    return 0;
}
