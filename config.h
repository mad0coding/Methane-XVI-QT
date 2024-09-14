#ifndef CONFIG_H
#define CONFIG_H

//#include "widget.h"
#include "tools.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QVector>

struct config_key;

class config
{
public:
    config();
    
    void set_mode1_key(uint8_t key_i, uint8_t key_val);//设置模式1按键
    void set_mode2_key(uint8_t key_i, uint8_t key_val, uint8_t func);//设置模式2按键
    bool add_mode3_key(uint8_t key_i, uint8_t key_val, uint8_t shift);//增加模式3按键
    bool add_mode3_func(uint8_t key_i, uint8_t key_val, uint8_t func);//增加模式3快捷键
    bool add_mode3_pos(uint8_t key_i, uint16_t x, uint16_t y);//增加模式3光标移位
    bool add_mode3_delay(uint8_t key_i, uint16_t t);//增加模式3延时
    void set_mode4_key(uint8_t key_i, uint16_t x, uint16_t y);//设置模式4(5)按键
    void set_mode6_key(uint8_t key_i, uint8_t key_val, uint8_t func);//设置模式6按键及切换
    void set_mode7_key(uint8_t key_i, uint8_t key_val, uint8_t func);//设置模式7按键及配置
    void set_mode7_time(uint8_t key_i, uint16_t t);//设置模式7时间
    void set_rk_key(uint8_t key_i, uint8_t key_val);//设置摇杆按键
    void set_ec_key(uint8_t key_i, uint8_t key_val);//设置旋钮按键
    void del_mode3_key(uint8_t key_i);//删除一个模式3按键
    void clear_key(uint8_t key_i);//清除模式1~3按键
    void clear_rk_key(uint8_t key_i);//清除摇杆按键
    void clear_ec_key(uint8_t key_i);//清除旋钮按键
    
    bool write_cfg_data();//写入配置数组
    bool read_cfg_data();//读出配置数组
    
    bool write_mode3_data(uint8_t keyi);//写入key配置数组
    bool read_mode3_data(uint8_t keyi);//读出key配置数组
    
    void set_key_mode(uint8_t keyi, uint8_t mode);//设置按键模式
    uint8_t get_key_mode(uint8_t keyi);//获取按键模式
    
    
    
    uint8_t *cfg_data = NULL;//配置数组
    uint8_t *cfg_data_key = NULL;//key配置数组
    
    config_key *cfg_key = NULL;//按键配置
    
    uint8_t *rk_key = NULL;//摇杆按键
    uint8_t *ec_key = NULL;//旋钮按键
    
    //控件指针:
    QLabel* label_k[16];
    QLabel* label_r[10];
    QLabel* label_e[6];
    
    QPushButton* bt_k[16];
    QPushButton* bt_r[10];
    QPushButton* bt_e[6];
    
    QComboBox* cbox_k[16];
    QComboBox *cbox_key_turn;
    QComboBox *cbox_r_1,*cbox_r_key_1,*cbox_r_2,*cbox_r_key_2;
    QComboBox *cbox_e_1,*cbox_e_key_1,*cbox_e_2,*cbox_e_key_2;
    QComboBox *cbox_light,*cbox_cycle;
    QComboBox *cbox_pri,*cbox_pri_r,*cbox_pri_e;
    
    QSpinBox *sbox_v_1,*sbox_r_1,*sbox_v_2,*sbox_r_2;
    QSpinBox *sbox_light,*sbox_light_rgb;
    QSpinBox *sbox_w,*sbox_h;
    
    QCheckBox *ckbox_r_x_1,*ckbox_r_y_1,*ckbox_r_r_1;
    QCheckBox *ckbox_r_x_2,*ckbox_r_y_2,*ckbox_r_r_2;
    QCheckBox *ckbox_e_1,*ckbox_e_2;
    QCheckBox *ckbox_sys_rgb_rk,*ckbox_sys_rgb_pilot;
    
    QPlainTextEdit *pte_mode3Box;
};

struct config_key
{
    uint8_t mode = 0;//模式
    uint8_t key = 0;//键值
    uint8_t func = 0;//功能键
    QString str = "";//文本
    QVector<uint8_t> data = {};//数据列
    QVector<uint8_t> strNum = {};//文本分段
    QVector<uint8_t> dataNum = {};//数据列分段
    
    uint16_t x = 0, y = 0;
    uint16_t t = 0;
};


#endif // CONFIG_H



