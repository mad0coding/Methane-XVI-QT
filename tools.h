#ifndef TOOLS_H
#define TOOLS_H

//#include "widget.h"
#include <QCoreApplication>
#include <QTime>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QWidget>

#include <QDesktopWidget>
#include <QInputDialog>



#define m1_button    1
#define m2_shortcut  2
#define m3_group     3
#define m4_move      4
#define m5_click     5
#define m6_change    6
#define m7_fast      7
#define m8_light     8
#define m9_serial    9
#define m10_buzz     10

#define kv_vol_next     237
#define kv_vol_prev     238
#define kv_point		239
#define kv_wheel_up     240
#define kv_wheel_down   241
#define kv_vol_up       242
#define kv_vol_down     243
#define kv_vol_mute     244
#define kv_vol_stop     245
#define kv_mouse_l      246
#define kv_mouse_m      247
#define kv_mouse_r      248
#define kv_ctrl         249
#define kv_shift        250
#define kv_alt          251
#define kv_win          252
#define kv_shortcut     253
#define kv_delay		254


bool read_cfg_file(QString path, uint8_t *buf, uint16_t len);//读配置文件
bool write_cfg_file(QString path, uint8_t *buf, uint16_t len);//写配置文件

bool read_key_file(QString path, uint8_t *buf);//读key文件
bool write_key_file(QString path, uint8_t *buf);//写key文件

void mySleep(int ms);//毫秒延时

uint8_t key_to_USB(int key, int Vkey);//QT键值转USB键值
QString USB_to_str(uint8_t key, bool shift);//USB键值转按键名








#endif // TOOLS_H




