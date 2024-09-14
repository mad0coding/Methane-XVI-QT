#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    cfgUnit = new config;//配置类
    passPointer();//传递指针
    
    serialPort = new QSerialPort(this);//创建串口对象
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(on_readyRead()));//连接串口信号
    
    hidApi = new HidApi();
    hidDev = new HidDevice();//HID设备
    
    ui->label_ctrl->setStyleSheet("color: gray");
    ui->label_shift->setStyleSheet("color: gray");
    ui->label_alt->setStyleSheet("color: gray");
    ui->label_win->setStyleSheet("color: gray");
    
    QString style_prefix = "QPushButton{background-color:rgb(220, 220, 220);";
    QString style_suffix = "QPushButton:hover{background-color:rgb(210, 210, 210);}"
                           "QPushButton:pressed{background-color:rgb(200, 200, 200);}";
    
    style_big_black = style_prefix + "color: black;border-radius: 20px;}" + style_suffix;
    style_big_gray = style_prefix + "color: gray;border-radius: 20px;}" + style_suffix;
    style_mid_black = style_prefix + "color: black;border-radius: 12px;}" + style_suffix;
    style_mid_gray = style_prefix + "color: gray;border-radius: 12px;}" + style_suffix;
    style_small_black = style_prefix + "color: black;border-radius: 8px;}" + style_suffix;
    style_small_gray = style_prefix + "color: gray;border-radius: 8px;}" + style_suffix;
    
    for(int i = 0; i < 16; i++){
        cfgUnit->cbox_k[i]->clear();
        //cfgUnit->cbox_k[i]->addItems(cbox_k_list);
        cfgUnit->cbox_k[i]->addItem("无",0);
        cfgUnit->cbox_k[i]->addItem("单键",1);
        cfgUnit->cbox_k[i]->addItem("快捷键",2);
        cfgUnit->cbox_k[i]->addItem("按键组",3);
        cfgUnit->cbox_k[i]->addItem("光标移位",4);
        cfgUnit->cbox_k[i]->addItem("光标点击",5);
        cfgUnit->cbox_k[i]->addItem("切换键",6);
        cfgUnit->cbox_k[i]->addItem("按键连点",7);
        cfgUnit->cbox_k[i]->addItem("灯控",8);
        cfgUnit->cbox_k[i]->addItem("串口",9);
        cfgUnit->cbox_k[i]->addItem("蜂鸣器",10);
        
        cfgUnit->bt_k[i]->setStyleSheet(style_big_black);//键盘按键样式
    }
    for(int i = 0; i < 10; i++) cfgUnit->bt_r[i]->setStyleSheet(style_big_black);//摇杆按键样式
    for(int i = 0; i < 6; i++) cfgUnit->bt_e[i]->setStyleSheet(style_big_black);//旋钮按键样式
    
    ui->Bt_write->setStyleSheet(style_big_black);
    ui->Bt_open->setStyleSheet(style_big_black);
    ui->Bt_save->setStyleSheet(style_big_black);
    ui->Bt_saveas->setStyleSheet(style_big_black);
    ui->Bt_get_adc->setStyleSheet(style_big_black);
    ui->Bt_key_filter->setStyleSheet(style_big_black);
    
    ui->key_clear->setStyleSheet(style_mid_black);
    ui->key_clear_e_1->setStyleSheet(style_mid_black);
    ui->key_clear_e_2->setStyleSheet(style_mid_black);
    ui->key_clear_r_1->setStyleSheet(style_mid_black);
    ui->key_clear_r_2->setStyleSheet(style_mid_black);
    
    ui->Bt_pos_get->setStyleSheet(style_mid_black);
    ui->Bt_pos_input->setStyleSheet(style_mid_black);
    ui->Bt_delay->setStyleSheet(style_mid_black);
    
    ui->Bt_open_key->setStyleSheet(style_mid_black);
    ui->Bt_write_key->setStyleSheet(style_mid_black);
    ui->Bt_save_key->setStyleSheet(style_mid_black);
    ui->Bt_saveas_key->setStyleSheet(style_mid_black);
    
    ui->mode3_input->setStyleSheet(style_mid_black);
    ui->mode3_shortcut->setStyleSheet(style_mid_black);
    ui->mode3_del->setStyleSheet(style_mid_black);
    ui->mode3_clear->setStyleSheet(style_mid_black);
    
    ui->Bt_hold->setStyleSheet(style_small_black);
    ui->Bt_empty->setStyleSheet(style_small_black);
    ui->Bt_vol_down->setStyleSheet(style_small_black);
    ui->Bt_vol_up->setStyleSheet(style_small_black);
    ui->Bt_vol_prev->setStyleSheet(style_small_black);
    ui->Bt_vol_next->setStyleSheet(style_small_black);
    ui->Bt_vol_mute->setStyleSheet(style_small_black);
    ui->Bt_vol_stop->setStyleSheet(style_small_black);
    ui->Bt_wheel_down->setStyleSheet(style_small_black);
    ui->Bt_wheel_up->setStyleSheet(style_small_black);
    ui->Bt_mouse_L->setStyleSheet(style_small_black);
    ui->Bt_mouse_M->setStyleSheet(style_small_black);
    ui->Bt_mouse_R->setStyleSheet(style_small_black);
    
//    QFont font = this->font();
//    printf("%d\n",font.pointSize());
//    font.setPointSize(7);//默认为9
//    this->setFont(font);
}

Widget::~Widget()
{
    delete cfgUnit;
    delete ui;
}

void Widget::key_handle(uint8_t keyValue, bool ifPress = true)//按键处理
{
    if(ifPress && state > 0){
        if(button_class == 1){//键盘
            if(cfgUnit->get_key_mode(button_choose - 1) == m1_button){//单键
                cfgUnit->set_mode1_key(button_choose - 1,keyValue);
                cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
                state = 0;
            }
            else if(cfgUnit->get_key_mode(button_choose - 1) == m2_shortcut){//快捷键
                if(!(keyValue >= 249 && keyValue <= 252) && func != 0){//若不为功能键且功能键存在
                    cfgUnit->set_mode2_key(button_choose - 1,keyValue,func);
                    cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
                    state = 0;
                }
            }
            else if(cfgUnit->get_key_mode(button_choose - 1) == m3_group){//按键组
                if(!(keyValue >= 249 && keyValue <= 252)){//若不为功能键
                    if(state == 2 && func != 0){
                        if(!cfgUnit->add_mode3_func(button_choose - 1,keyValue,func)){
                            QMessageBox::critical(this,"提示","此键可用空间不足");
                        }
                    }
                    else if(state == 3){
                        if(!cfgUnit->add_mode3_key(button_choose - 1,keyValue,func & 0x02)){
                            QMessageBox::critical(this,"提示","此键可用空间不足");
                        }
                    }
                }
            }
            else if(cfgUnit->get_key_mode(button_choose - 1) == m6_change){//配置切换
                int ansTP = QMessageBox::question(this,"切换配置","切换方式?","临时","永久",0,-1);
                int ansK = QMessageBox::question(this,"切换配置","切换整个键盘配置?","是","否",0,-1);
                int ansR = QMessageBox::question(this,"切换配置","切换摇杆配置?","是","否",0,-1);
                int ansE = QMessageBox::question(this,"切换配置","切换旋钮配置?","是","否",0,-1);
                uint8_t ansTO = (!ansK) | (!ansR << 1) | (!ansE << 2);
                if(ansTP == 0){//临时切换
                    cfgUnit->set_mode6_key(button_choose - 1,keyValue,ansTO | 0x08);
                }
                else{//永久切换
                    cfgUnit->set_mode6_key(button_choose - 1,0,ansTO);
                }
                cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
                state = 0;
            }
            else if(cfgUnit->get_key_mode(button_choose - 1) == m7_fast){//按键连点
                int ansAuto = QMessageBox::question(this,"按键连点","连点方式?","非自动","自动",0,-1);
                cfgUnit->set_mode7_key(button_choose - 1,keyValue,ansAuto);
                cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
                state = 0;
            }
        }
        else if(button_class == 2){//摇杆
            cfgUnit->set_rk_key(button_choose - 1,keyValue);
            cfgUnit->bt_r[button_choose - 1]->setStyleSheet(style_big_black);
            state = 0;
        }
        else if(button_class == 3){//旋钮
            cfgUnit->set_ec_key(button_choose - 1,keyValue);
            cfgUnit->bt_e[button_choose - 1]->setStyleSheet(style_big_black);
            state = 0;
        }
    }
}

void Widget::keyPressEvent(QKeyEvent *event)//按键按下
{
    if(event->isAutoRepeat() || ifSending){ return; }//若为自动重复触发或正在发送数据则返回
    int key1 = event->key();//读取第一种键值
    int key2 = event->nativeVirtualKey();//读取第二种键值
    
    uint8_t keyValue = key_to_USB(key1,key2);//映射到USB键值
    
    if(ifPos){//若正在位置获取
        ifPos = false;
        ui->Bt_pos_get->setStyleSheet(style_mid_black);
        setWindowOpacity(1.0);
        QPoint pt = QCursor::pos();//获取鼠标的绝对位置
        int mouseScreen = QApplication::desktop()->screenNumber(pt);//获取鼠标所在的屏幕
        QRect mouseScreenGeometry = QApplication::desktop()->screen(mouseScreen)->geometry();
        QPoint ptInScreen = pt - mouseScreenGeometry.topLeft();//鼠标在该屏幕中的位置
        ui->spinBox_x->setValue(ptInScreen.x());//填入数据框
        ui->spinBox_y->setValue(ptInScreen.y());
        return;
    }
    
    if(keyValue >= 249 && keyValue <= 252) func |= 0x01 << (keyValue - 249);
    else{
        ui->label_keyValue->setText("USB键值:" + QString::number(keyValue));
        ui->label_keyName->setText("按键:" + USB_to_str(keyValue,0));
        key_num++;
    }
    
    if(keyValue == 249) ui->label_ctrl->setStyleSheet("color: black");
    else if(keyValue == 250) ui->label_shift->setStyleSheet("color: black");
    else if(keyValue == 251) ui->label_alt->setStyleSheet("color: black");
    else if(keyValue == 252) ui->label_win->setStyleSheet("color: black");
    
    key_handle(keyValue);
    //printf("key:%d\n",keyValue);//打印键值
    //printf("funcP:%d",func);//打印当前功能键
}

void Widget::keyReleaseEvent(QKeyEvent *event)//按键抬起
{
    if(event->isAutoRepeat() || ifSending){ return; }//若为自动重复触发或正在发送数据则返回
    int key1 = event->key();//读取第一种键值
    int key2 = event->nativeVirtualKey();//读取第二种键值
    
    uint8_t keyValue = key_to_USB(key1,key2);//映射到USB键值
    
    if(keyValue >= 249 && keyValue <= 252){
        if(!ifHold) func &= ~(0x01 << (keyValue - 249));
    }
    else if(key_num > 0) key_num--;
    
    if(key_num == 0){
        ui->label_keyValue->setText("USB键值:None");
        ui->label_keyName->setText("按键:None");
    }
    
    if(!ifHold){
        if(keyValue == 249) ui->label_ctrl->setStyleSheet("color: grey");
        else if(keyValue == 250) ui->label_shift->setStyleSheet("color: grey");
        else if(keyValue == 251) ui->label_alt->setStyleSheet("color: grey");
        else if(keyValue == 252) ui->label_win->setStyleSheet("color: grey");
    }
    key_handle(keyValue,false);
    //printf("funcR:%d",func);//打印当前功能键
}

void Widget::on_readyRead()//串口接收数据
{
    QByteArray BAbuf = serialPort->readAll();//数据读入缓冲区
    int len = BAbuf.length();//获取数据长度
    char* buf = BAbuf.data();//提取数据
    if(len == 3){
        if(buf[1] == 'L' && buf[2] == 'E'){
            if(buf[0] == 'R') response = true;//响应连接
            if(buf[0] >= 0 && buf[0] <= 15) {//响应数据包
                count = buf[0];//计数
            }
        }
    }
}

void Widget::portSend(uint8_t *buf, uint16_t len)//串口发送数据
{
    QByteArray txBuf = "";
    for(uint16_t i = 0; i < len; i++){
        txBuf.append(buf[i]);
    }
    serialPort->write(txBuf);        
}

bool Widget::openPort(QString Name)//打开串口
{
    serialPort->setPortName(Name);//串口名
    serialPort->setBaudRate(QSerialPort::Baud115200);//波特率
    serialPort->setDataBits(QSerialPort::Data8);//数据位
    serialPort->setStopBits(QSerialPort::OneStop);//停止位
    serialPort->setParity(QSerialPort::NoParity);//校验位
    return serialPort->open(QIODevice::ReadWrite);//打开串口并返回是否成功
}

bool Widget::connectCDC()//以CDC连接设备
{
    serialPort->close();//关闭串口
    response = false;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {//搜索可用串口
        if(info.vendorIdentifier() != ui->spinBox_vid->value() 
                || info.productIdentifier() != ui->spinBox_pid->value()) continue;//vid,pid不正确则跳过
        if(openPort(info.portName())){//尝试打开串口
            if(ui->cBox_flash->currentIndex() == 0) serialPort->write("LE1");//发送连接指令1
            else serialPort->write("LE2");//发送连接指令2
            for(int i = 0; i < 100; i++){//等待设备响应
                if(response) return true;//有响应
                mySleep(1);//延时1ms
            }
            serialPort->close();//关闭串口(有此操作才能打开接下来的串口)
        }
    }
    return false;//无可用串口或无设备响应
}

bool Widget::writeCDC()//以CDC向设备写入数据
{
    ifSending = true;//正在发送
    int success = 0;
    if(connectCDC()){//若连接成功
        count = 255;
        for(int i = 0; i < 8; i++){//64*8=512字节
            portSend(cfgUnit->cfg_data + i * 64,64);//发送一包数据
            for(int j = 0; j < 100; j++){//等待设备响应
                if(count == i){//若正确响应
                    success++;
                    break;
                }
                mySleep(1);//延时1ms
            }
        }
    }
    serialPort->close();//关闭串口
    
    ifSending = false;//发送结束
    if(success == 8) return true;
    else return false;
}

uint32_t Widget::connectHID(uint8_t cmd, uint16_t para = 0)//以自定义HID连接设备
{
//    hidApi = new HidApi();
//    hidDev = new HidDevice();//HID设备
    HidDeviceList devList;//HID设备列表
    
    if(!hidApi->isInitialized()) return false;//HidApi未成功初始化
    
    devList = hidApi->scanDevices(ui->spinBox_vid->value(),ui->spinBox_pid->value());//按VID和PID扫描设备
    
    for(size_t i = 0; i < devList.size(); i++){//可打印扫描到的设备的信息
        if(devList[i].getUsagePage() == 0xFF00){//在devList中找到要通信的设备
            *hidDev = devList[i];//选定该设备
            if(!hidDev->isInitialized()) return false;//HID设备未初始化则退出
            if(!hidDev->open()) return false;//HID设备打开失败则退出
            
            std::string hidWriteStr;//创建写入缓存
            hidWriteStr.resize(65, 0);//重置为65字节
            if(cmd == 0){//连接命令
                hidWriteStr[0] = 0; hidWriteStr[1] = 'L'; hidWriteStr[2] = 'E';//填入连接指令
                hidWriteStr[3] = '1' + ui->cBox_flash->currentIndex();//填入存储位置
                
                while(hidDev->readAvailable()) hidDev->read(1);//若已有数据则先读出
                if(hidDev->write(hidWriteStr) != 65) return false;//若写入失败则退出
                
                std::string hidReadStr = hidDev->read(500);//等待读取响应数据最多500ms
                if(hidReadStr.length() == 65 && hidReadStr[1] == 'R' 
                        && hidReadStr[2] == 'L' && hidReadStr[3] == 'E'){//若正确响应
                    return true;
                }
            }
            else if(cmd == 1){//摇杆校正命令
                hidWriteStr[0] = 0;
                hidWriteStr[1] = 'R'; hidWriteStr[2] = 'K'; hidWriteStr[3] = 'C';//填入校正指令
                
                while(hidDev->readAvailable()) hidDev->read(1);//若已有数据则先读出
                if(hidDev->write(hidWriteStr) != 65) return false;//若写入失败则退出
                
                std::string hidReadStr = hidDev->read(500);//等待读取响应数据最多500ms
                hidDev->close();//关闭HID设备
                if(hidReadStr.length() == 65 && hidReadStr[1] == 'R' && hidReadStr[2] == 'K'){//若正确响应
                    uint16_t adcValue[2];
                    adcValue[0] = ((uint8_t)hidReadStr[3] << 8) | (uint8_t)hidReadStr[4];//获取ADC值
                    adcValue[1] = ((uint8_t)hidReadStr[5] << 8) | (uint8_t)hidReadStr[6];
                    if(adcValue[0] > 4095 || adcValue[1] > 4095) return 0;
                    uint32_t adcUint = (adcValue[0] << 16) | adcValue[1];//打包成32位数
                    return adcUint;
                }
            }
            else if(cmd == 2){//按键滤波设置命令
                hidWriteStr[0] = 0;
                hidWriteStr[1] = 'K'; hidWriteStr[2] = 'Y'; hidWriteStr[3] = 'F';//填入校正指令
                hidWriteStr[4] = (uint8_t)para;
                while(hidDev->readAvailable()) hidDev->read(1);//若已有数据则先读出
                if(hidDev->write(hidWriteStr) != 65) return false;//若写入失败则退出
                
                std::string hidReadStr = hidDev->read(500);//等待读取响应数据最多500ms
                hidDev->close();//关闭HID设备
                if(hidReadStr.length() == 65 && hidReadStr[1] == 'K' && hidReadStr[2] == 'Y'){//若正确响应
                    return 1;
                }
                else return 0;
            }
        }
    }
    return false;//无可用HID或无设备响应
}

bool Widget::writeHID()//以自定义HID向设备写入数据
{
    ifSending = true;//正在发送
    int success = 0;
    if(connectHID(0)){//若连接成功
        std::string hidWriteStr;//创建写入缓存
        hidWriteStr.resize(65, 0);//重置为65字节
        hidWriteStr[0] = 0;
        
        while(hidDev->readAvailable()) hidDev->read(0);//若已有数据则先读出
        
        for(int i = 0; i < 8; i++){//64*8=512字节
            mySleep(5);//强制延时5ms防止HID发送卡死
            for(int j = 0; j < 64; j++) hidWriteStr[j + 1] = *(cfgUnit->cfg_data + i * 64 + j);//把64字节复制到发送字符串
            
            if(hidDev->write(hidWriteStr) != 65) return false;//若写入失败则退出     
            
            for(int j = 0; j < 100; j++){//等待设备响应
                if(hidDev->readAvailable()){//若有响应数据
                    std::string hidReadStr = hidDev->read(50);//读取响应数据
                    if(hidReadStr.length() == 65 && hidReadStr[1] == i 
                            && hidReadStr[2] == 'L' && hidReadStr[3] == 'E'){//若正确响应
                        success++;
                        break;
                    }
                }
                mySleep(1);//延时1ms
            }
        }
    }
    hidDev->close();//关闭HID设备
    
    ifSending = false;//发送结束
    if(success == 8) return true;
    else return false;
}

void Widget::on_Bt_get_adc_clicked()//摇杆校正
{
    ifSending = true;//正在发送
    uint32_t adcUint = connectHID(1);//获取摇杆ADC值
    ifSending = false;//发送结束
    if(adcUint == 0){//若获取失败
        QMessageBox::critical(this,"摇杆校正","HID通信失败");
        return;
    }
    else{//若获取成功
        uint16_t adcValue[2];
        adcValue[0] = adcUint >> 16;
        adcValue[1] = adcUint & 0xFFFF;
        
        QString adcInfo = "已更新中位值为:" + QString::number(adcValue[0]) 
                                    + "," + QString::number(adcValue[1]);
        QMessageBox::information(this,"摇杆校正",adcInfo);
    }
}

void Widget::on_Bt_key_filter_clicked()//按键滤波设置
{
    bool ifSet = false;
    uint8_t paraSet = QInputDialog::getInt(this, "按键滤波设置", "滤波时间(1-250ms)",
                                            10, 1, 250, 1,//默认值,最小值,最大值,步进
                                            &ifSet, Qt::WindowCloseButtonHint);
    if(!ifSet) return;
    ifSending = true;//正在发送
    uint32_t ans = connectHID(2, paraSet);//按键滤波设置
    ifSending = false;//发送结束
    if(ans == 0){//若获取失败
        QMessageBox::critical(this,"按键滤波设置","HID通信失败");
    }
    else{//若获取成功
        QMessageBox::information(this,"按键滤波设置","滤波时间设置成功");
    }
}

void Widget::openCfgFile()//打开配置文件
{
    //qDebug()<<fileNow<<endl;
    if(!read_cfg_file(fileNow,cfgUnit->cfg_data,512)){
        QMessageBox::critical(this,"提示","无路径");
        return;
    }
    
    if(cfgUnit->read_cfg_data()){
        QFileInfo fileinfo = QFileInfo(fileNow);//文件信息
        ui->lineEdit_fileName->setText(fileinfo.fileName());//显示文件名
    }
    else QMessageBox::critical(this,"提示","配置读取失败");
}

void Widget::saveCfgFile()//保存配置文件
{
    QFileInfo fileinfo = QFileInfo(fileNow);//文件信息
    if(fileinfo.fileName().isEmpty()) return;
    if(fileinfo.suffix().isEmpty()){
        fileNow += ".lecfg";//若无后缀则添加
        fileinfo = QFileInfo(fileNow);
    }
    
    if(cfgUnit->write_cfg_data()){
        write_cfg_file(fileNow,cfgUnit->cfg_data,512);
        ui->lineEdit_fileName->setText(fileinfo.fileName());//显示文件名
    }
    else QMessageBox::critical(this,"提示","配置超长");
}

void Widget::openKeyFile()//打开key文件
{
    if(!read_cfg_file(fileNowKey,cfgUnit->cfg_data_key,257)){
        QMessageBox::critical(this,"提示","无路径");
    }
    QFileInfo fileinfo = QFileInfo(fileNowKey);//文件信息
    ui->lineEdit_fileName_key->setText(fileinfo.fileName());//显示文件名
    if(button_choose == 0 || cfgUnit->get_key_mode(button_choose - 1) != m3_group) return;
    //cfgUnit->cbox_k[button_choose - 1]->setCurrentIndex(m3_group);
    if(cfgUnit->read_mode3_data(button_choose - 1)){
        cfgUnit->pte_mode3Box->setPlainText(cfgUnit->cfg_key[button_choose - 1].str);
    }
    else QMessageBox::critical(this,"提示","按键配置失败");
    cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
    button_choose = 0;
}

void Widget::saveKeyFile()//保存key文件
{
    if(button_choose == 0 || cfgUnit->get_key_mode(button_choose - 1) != m3_group 
            || cfgUnit->cfg_key[button_choose - 1].data.isEmpty()){
        QMessageBox::information(this,"提示","需要选中按键组模式的非空按键");
        return;
    }
    QFileInfo fileinfo = QFileInfo(fileNowKey);//文件信息
    if(fileinfo.fileName().isEmpty()) return;
    if(fileinfo.suffix().isEmpty()){
        fileNowKey += ".lekey";//若无后缀则添加
        fileinfo = QFileInfo(fileNowKey);
    }
    
    if(cfgUnit->write_mode3_data(button_choose - 1)){
        write_cfg_file(fileNowKey,cfgUnit->cfg_data_key,257);
        ui->lineEdit_fileName_key->setText(fileinfo.fileName());//显示文件名
    }
    else QMessageBox::critical(this,"提示","配置储存失败");
    button_choose = 0;
}

void Widget::on_Bt_write_clicked()//写入设备按钮
{
    ui->Bt_write->setStyleSheet(style_big_gray);
    ui->Bt_write->setEnabled(false);//暂时禁用按钮
    if(cfgUnit->write_cfg_data()){
        if(ui->cBox_hid_cdc->currentIndex() == 0){//HID通信
            if(writeHID()){
                //QMessageBox::information(this,"提示","写入成功");
            }
            else QMessageBox::critical(this,"HID通信","写入失败");
        }
        else if(ui->cBox_hid_cdc->currentIndex() == 1){//CDC通信
            if(writeCDC()){
                //QMessageBox::information(this,"提示","写入成功");
            }
            else QMessageBox::critical(this,"CDC通信","写入失败");
        }
    }
    ui->Bt_write->setEnabled(true);//恢复启用按钮
    ui->Bt_write->setStyleSheet(style_big_black);
}

void Widget::on_Bt_open_clicked()//打开文件按钮
{
    QString fileNew = QFileDialog::getOpenFileName(this,QStringLiteral("打开配置文件"),
                                                   "./configFile",QStringLiteral("配置文件(*lecfg)"));
    if(fileNew.isEmpty()) return;
    fileNow = fileNew;
    openCfgFile();
}

void Widget::on_Bt_save_clicked()//保存文件按钮
{
    if(fileNow.isEmpty()){
        fileNow = QFileDialog::getSaveFileName(this,QStringLiteral("保存配置文件"),
                                               "./configFile",QStringLiteral("配置文件(*lecfg)"));
    }
    saveCfgFile();//保存配置文件
}

void Widget::on_Bt_saveas_clicked()//另存为文件按钮
{
    QString fileNew = QFileDialog::getSaveFileName(this,QStringLiteral("另存为配置文件"),
                                                   "./configFile",QStringLiteral("配置文件(*lecfg)"));
    if(fileNew.isEmpty()) return;
    fileNow = fileNew;
    saveCfgFile();//保存配置文件
}

void Widget::on_Bt_write_key_clicked()//写入key按钮
{
    if(button_choose == 0){
        QMessageBox::information(this,"提示","请先选中按键");
        return;
    }
    openKeyFile();//打开key文件
}

void Widget::on_Bt_open_key_clicked()//打开key文件按钮
{
    QString path = "./keyFile";
    QDir dir(path);
    if(!dir.exists()) path = "../keyFile";
    fileNowKey = QFileDialog::getOpenFileName(this,QStringLiteral("打开按键配置文件"),
                                              path,QStringLiteral("按键配置文件(*lekey)"));
    openKeyFile();//打开key文件
}

void Widget::on_Bt_save_key_clicked()//保存key文件按钮
{
    QString path = "./keyFile";
    QDir dir(path);
    if(!dir.exists()) path = "../keyFile";
    if(fileNowKey.isEmpty()){
        fileNowKey = QFileDialog::getSaveFileName(this,QStringLiteral("保存按键配置文件"),
                                                  path,QStringLiteral("按键配置文件(*lekey)"));
    }
    saveKeyFile();//保存key文件
}

void Widget::on_Bt_saveas_key_clicked()//另存为key文件按钮
{
    QString path = "./keyFile";
    QDir dir(path);
    if(!dir.exists()) path = "../keyFile";
    fileNowKey = QFileDialog::getSaveFileName(this,QStringLiteral("另存为按键配置文件"),
                                              path,QStringLiteral("按键配置文件(*lekey)"));
    saveKeyFile();//保存key文件
}

void Widget::passPointer()//传递指针
{
    cfgUnit->label_k[0] = ui->lb_key_1;//按键标签
    cfgUnit->label_k[1] = ui->lb_key_2;
    cfgUnit->label_k[2] = ui->lb_key_3;
    cfgUnit->label_k[3] = ui->lb_key_4;
    cfgUnit->label_k[4] = ui->lb_key_5;
    cfgUnit->label_k[5] = ui->lb_key_6;
    cfgUnit->label_k[6] = ui->lb_key_7;
    cfgUnit->label_k[7] = ui->lb_key_8;
    cfgUnit->label_k[8] = ui->lb_key_9;
    cfgUnit->label_k[9] = ui->lb_key_10;
    cfgUnit->label_k[10] = ui->lb_key_11;
    cfgUnit->label_k[11] = ui->lb_key_12;
    cfgUnit->label_k[12] = ui->lb_key_13;
    cfgUnit->label_k[13] = ui->lb_key_14;
    cfgUnit->label_k[14] = ui->lb_key_15;
    cfgUnit->label_k[15] = ui->lb_key_16;
    
    cfgUnit->label_r[0] = ui->lb_r_1;//摇杆按键标签
    cfgUnit->label_r[1] = ui->lb_r_1_0;
    cfgUnit->label_r[2] = ui->lb_r_1_1;
    cfgUnit->label_r[3] = ui->lb_r_1_2;
    cfgUnit->label_r[4] = ui->lb_r_1_3;
    cfgUnit->label_r[5] = ui->lb_r_2;
    cfgUnit->label_r[6] = ui->lb_r_2_0;
    cfgUnit->label_r[7] = ui->lb_r_2_1;
    cfgUnit->label_r[8] = ui->lb_r_2_2;
    cfgUnit->label_r[9] = ui->lb_r_2_3;
    
    cfgUnit->label_e[0] = ui->lb_e_1;//旋钮按键标签
    cfgUnit->label_e[1] = ui->lb_e_1_0;
    cfgUnit->label_e[2] = ui->lb_e_1_1;
    cfgUnit->label_e[3] = ui->lb_e_2;
    cfgUnit->label_e[4] = ui->lb_e_2_0;
    cfgUnit->label_e[5] = ui->lb_e_2_1;
    
    cfgUnit->bt_k[0] = ui->key1;//按键按钮
    cfgUnit->bt_k[1] = ui->key2;
    cfgUnit->bt_k[2] = ui->key3;
    cfgUnit->bt_k[3] = ui->key4;
    cfgUnit->bt_k[4] = ui->key5;
    cfgUnit->bt_k[5] = ui->key6;
    cfgUnit->bt_k[6] = ui->key7;
    cfgUnit->bt_k[7] = ui->key8;
    cfgUnit->bt_k[8] = ui->key9;
    cfgUnit->bt_k[9] = ui->key10;
    cfgUnit->bt_k[10] = ui->key11;
    cfgUnit->bt_k[11] = ui->key12;
    cfgUnit->bt_k[12] = ui->key13;
    cfgUnit->bt_k[13] = ui->key14;
    cfgUnit->bt_k[14] = ui->key15;
    cfgUnit->bt_k[15] = ui->key16;
    
    cfgUnit->bt_r[0] = ui->key_r_1;//摇杆按键按钮
    cfgUnit->bt_r[1] = ui->key_r_1_0;
    cfgUnit->bt_r[2] = ui->key_r_1_1;
    cfgUnit->bt_r[3] = ui->key_r_1_2;
    cfgUnit->bt_r[4] = ui->key_r_1_3;
    cfgUnit->bt_r[5] = ui->key_r_2;
    cfgUnit->bt_r[6] = ui->key_r_2_0;
    cfgUnit->bt_r[7] = ui->key_r_2_1;
    cfgUnit->bt_r[8] = ui->key_r_2_2;
    cfgUnit->bt_r[9] = ui->key_r_2_3;
    
    cfgUnit->bt_e[0] = ui->key_e_1;//旋钮按键按钮
    cfgUnit->bt_e[1] = ui->key_e_1_0;
    cfgUnit->bt_e[2] = ui->key_e_1_1;
    cfgUnit->bt_e[3] = ui->key_e_2;
    cfgUnit->bt_e[4] = ui->key_e_2_0;
    cfgUnit->bt_e[5] = ui->key_e_2_1;
    
    cfgUnit->cbox_key_turn = ui->cBox_key_turn;//方向
    
    cfgUnit->cbox_k[0] = ui->cBox_k_1;//按键模式下拉框
    cfgUnit->cbox_k[1] = ui->cBox_k_2;
    cfgUnit->cbox_k[2] = ui->cBox_k_3;
    cfgUnit->cbox_k[3] = ui->cBox_k_4;
    cfgUnit->cbox_k[4] = ui->cBox_k_5;
    cfgUnit->cbox_k[5] = ui->cBox_k_6;
    cfgUnit->cbox_k[6] = ui->cBox_k_7;
    cfgUnit->cbox_k[7] = ui->cBox_k_8;
    cfgUnit->cbox_k[8] = ui->cBox_k_9;
    cfgUnit->cbox_k[9] = ui->cBox_k_10;
    cfgUnit->cbox_k[10] = ui->cBox_k_11;
    cfgUnit->cbox_k[11] = ui->cBox_k_12;
    cfgUnit->cbox_k[12] = ui->cBox_k_13;
    cfgUnit->cbox_k[13] = ui->cBox_k_14;
    cfgUnit->cbox_k[14] = ui->cBox_k_15;
    cfgUnit->cbox_k[15] = ui->cBox_k_16;
    
    cfgUnit->cbox_r_1 = ui->cBox_r_1;//摇杆模式
    cfgUnit->cbox_r_key_1 = ui->cBox_r_key_1;//摇杆按键模式
    cfgUnit->cbox_r_2 = ui->cBox_r_2;
    cfgUnit->cbox_r_key_2 = ui->cBox_r_key_2;
    
    cfgUnit->sbox_v_1 = ui->spinBox_v_1;//摇杆速度
    cfgUnit->sbox_r_1 = ui->spinBox_r_1;//摇杆死区
    cfgUnit->sbox_v_2 = ui->spinBox_v_2;
    cfgUnit->sbox_r_2 = ui->spinBox_r_2;
    
    cfgUnit->cbox_e_1 = ui->cBox_e_1;//旋钮模式
    cfgUnit->cbox_e_key_1 = ui->cBox_e_key_1;//旋钮按键模式
    cfgUnit->cbox_e_2 = ui->cBox_e_2;
    cfgUnit->cbox_e_key_2 = ui->cBox_e_key_2;
    
    cfgUnit->cbox_light = ui->cBox_light;//灯光模式
    cfgUnit->cbox_cycle = ui->cBox_cycle;//循环周期
    cfgUnit->cbox_pri = ui->cBox_pri;//优先级
    cfgUnit->cbox_pri_r = ui->cBox_pri_r;//摇杆优先级
    cfgUnit->cbox_pri_e = ui->cBox_pri_e;//旋钮优先级
    
    cfgUnit->sbox_light = ui->spinBox_light;//灯亮度
    cfgUnit->sbox_light_rgb = ui->spinBox_light_rgb;//RGB亮度
    cfgUnit->sbox_w = ui->spinBox_w;//屏幕宽度
    cfgUnit->sbox_h = ui->spinBox_h;//屏幕高度
    
    cfgUnit->ckbox_r_x_1 = ui->cB_r_x_1;//摇杆反向
    cfgUnit->ckbox_r_y_1 = ui->cB_r_y_1;
    cfgUnit->ckbox_r_r_1 = ui->cB_r_r_1;//摇杆右旋90度
    cfgUnit->ckbox_r_x_2 = ui->cB_r_x_2;
    cfgUnit->ckbox_r_y_2 = ui->cB_r_y_2;
    cfgUnit->ckbox_r_r_2 = ui->cB_r_r_2;
    cfgUnit->ckbox_e_1 = ui->cB_e_1;//旋钮反向
    cfgUnit->ckbox_e_2 = ui->cB_e_2;
    
    cfgUnit->ckbox_sys_rgb_rk = ui->cB_sys_rgb_rk;//RGB摇杆映射
    cfgUnit->ckbox_sys_rgb_pilot = ui->cB_sys_rgb_pilot;//RGB灯光指示
    
    cfgUnit->pte_mode3Box = ui->mode3Box;//按键数据组显示框
}


void Widget::key_click_handle(uint8_t button_cs)//16按钮按下处理
{
    if(button_choose > 0 && button_class == 1){
        cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
        if(button_cs == button_choose && (state || cfgUnit->get_key_mode(button_choose - 1) == m3_group)){
            ui->lb_key_cs->setText("当前选择:None");
            button_class = button_choose = 0;
            ui->mode3Box->clear();
            ui->mode3_input->setText("输入");
            return;
        }
    }
    state = button_class = 1;
    button_choose = button_cs;
    QString key_cs_str = "当前选择:KEY " + QString::number(button_cs);
    ui->lb_key_cs->setText(key_cs_str);
    if(cfgUnit->get_key_mode(button_cs - 1) == m3_group){//若为按键组模式
        ui->mode3Box->setPlainText(cfgUnit->cfg_key[button_cs - 1].str);
        state = 0;
    }
    else if(cfgUnit->get_key_mode(button_cs - 1) == m7_fast){//若为连点模式
        ui->mode3Box->setPlainText(cfgUnit->cfg_key[button_cs - 1].str);
    }
    else ui->mode3Box->clear();
    cfgUnit->bt_k[button_cs - 1]->setStyleSheet(style_big_gray);
    ui->mode3_input->setText("输入");
}
void Widget::on_key1_clicked(){ key_click_handle(1);    }
void Widget::on_key2_clicked(){ key_click_handle(2);    }
void Widget::on_key3_clicked(){ key_click_handle(3);    }
void Widget::on_key4_clicked(){ key_click_handle(4);    }
void Widget::on_key5_clicked(){ key_click_handle(5);    }
void Widget::on_key6_clicked(){ key_click_handle(6);    }
void Widget::on_key7_clicked(){ key_click_handle(7);    }
void Widget::on_key8_clicked(){ key_click_handle(8);    }
void Widget::on_key9_clicked(){ key_click_handle(9);    }
void Widget::on_key10_clicked(){ key_click_handle(10);    }
void Widget::on_key11_clicked(){ key_click_handle(11);    }
void Widget::on_key12_clicked(){ key_click_handle(12);    }
void Widget::on_key13_clicked(){ key_click_handle(13);    }
void Widget::on_key14_clicked(){ key_click_handle(14);    }
void Widget::on_key15_clicked(){ key_click_handle(15);    }
void Widget::on_key16_clicked(){ key_click_handle(16);    }

void Widget::key_r_click_handle(uint8_t button_cs)//摇杆页10按钮按下处理
{
    if(button_choose > 0 && button_class == 2){
        cfgUnit->bt_r[button_choose - 1]->setStyleSheet(style_big_black);
        if(state && button_cs == button_choose){
            button_class = button_choose = 0;
            return;
        }
    }
    state = 1;
    button_class = 2;
    button_choose = button_cs;
    cfgUnit->bt_r[button_cs - 1]->setStyleSheet(style_big_gray);
}
void Widget::on_key_r_1_clicked(){  key_r_click_handle(1);   }//摇杆1按键
void Widget::on_key_r_1_0_clicked(){  key_r_click_handle(2);   }//摇杆1上
void Widget::on_key_r_1_1_clicked(){  key_r_click_handle(3);   }//摇杆1下
void Widget::on_key_r_1_2_clicked(){  key_r_click_handle(4);   }//摇杆1左
void Widget::on_key_r_1_3_clicked(){  key_r_click_handle(5);   }//摇杆1右
void Widget::on_key_r_2_clicked(){  key_r_click_handle(6);   }//摇杆2按键
void Widget::on_key_r_2_0_clicked(){  key_r_click_handle(7);   }//摇杆2上
void Widget::on_key_r_2_1_clicked(){  key_r_click_handle(8);   }//摇杆2下
void Widget::on_key_r_2_2_clicked(){  key_r_click_handle(9);   }//摇杆2左
void Widget::on_key_r_2_3_clicked(){  key_r_click_handle(10);   }//摇杆2右

void Widget::key_e_click_handle(uint8_t button_cs)//旋钮页6按钮按下处理
{
    if(button_choose > 0 && button_class == 3){
        cfgUnit->bt_e[button_choose - 1]->setStyleSheet(style_big_black);
        if(state && button_cs == button_choose){
            button_class = button_choose = 0;
            return;
        }
    }
    state = 1;
    button_class = 3;
    button_choose = button_cs;
    cfgUnit->bt_e[button_cs - 1]->setStyleSheet(style_big_gray);
}
void Widget::on_key_e_1_clicked(){  key_e_click_handle(1);   }//旋钮1按键
void Widget::on_key_e_1_0_clicked(){  key_e_click_handle(2);   }//旋钮1逆时针
void Widget::on_key_e_1_1_clicked(){  key_e_click_handle(3);   }//旋钮1顺时针
void Widget::on_key_e_2_clicked(){  key_e_click_handle(4);   }//旋钮2按键
void Widget::on_key_e_2_0_clicked(){  key_e_click_handle(5);   }//旋钮2逆时针
void Widget::on_key_e_2_1_clicked(){  key_e_click_handle(6);   }//旋钮2顺时针

void Widget::on_Bt_hold_clicked()//按键保持
{
    if(ifHold){//若之前在按键保持
        ifHold = false;
        func = 0;//清空功能键
        key_num = 0;//清零按键计数
        ui->label_ctrl->setStyleSheet("color: gray");
        ui->label_shift->setStyleSheet("color: gray");
        ui->label_alt->setStyleSheet("color: gray");
        ui->label_win->setStyleSheet("color: gray");
        ui->Bt_hold->setText("按键保持");
    }
    else{
        ifHold = true;
        ui->Bt_hold->setText("取消保持");
    }
}

void Widget::on_mode3_input_clicked()//按键组输入按钮
{
    if(ui->mode3_input->text() == "输入"){
        ui->mode3_input->setText("停止");
        ui->mode3_shortcut->setText("快捷键");
        state = 3;
    }
    else{
        ui->mode3_input->setText("输入");
        state = 0;
    }
}

void Widget::on_mode3_shortcut_clicked()//按键组快捷键按钮
{
    if(state == 3){
        ui->mode3_shortcut->setText("单键");
        state = 2;
    }
    else if(state == 2){
        ui->mode3_shortcut->setText("快捷键");
        state = 3;
    }
}

void Widget::on_mode3_del_clicked()//按键组删除按钮
{
    if(button_choose && button_class == 1){
        if(cfgUnit->get_key_mode(button_choose - 1) == m3_group){
            cfgUnit->del_mode3_key(button_choose - 1);
        }
    }
}

void Widget::on_mode3_clear_clicked()//按键组清空按钮
{
    if(button_choose && button_class == 1){
        if(cfgUnit->get_key_mode(button_choose - 1) == m3_group){
            cfgUnit->clear_key(button_choose - 1);
        }
    }
}

void Widget::on_key_clear_clicked()//按键清除键
{
    if(button_choose && button_class == 1){
        if(cfgUnit->get_key_mode(button_choose - 1) != m3_group){
            cfgUnit->clear_key(button_choose - 1);
        }
    }
}

void Widget::on_key_clear_r_1_clicked()//摇杆按键清除键
{
    if(button_choose && button_class == 2){
        cfgUnit->clear_rk_key(button_choose - 1);
    }
}

void Widget::on_key_clear_r_2_clicked()//摇杆按键清除键
{
    if(button_choose && button_class == 2){
        cfgUnit->clear_rk_key(button_choose - 1);
    }
}

void Widget::on_key_clear_e_1_clicked()//旋钮按键清除键
{
    if(button_choose && button_class == 3){
        cfgUnit->clear_ec_key(button_choose - 1);
    }
}

void Widget::on_key_clear_e_2_clicked()//旋钮按键清除键
{
    if(button_choose && button_class == 3){
        cfgUnit->clear_ec_key(button_choose - 1);
    }
}

void Widget::on_tabWidget_currentChanged(int index)//翻页
{
    State_to_zero();//状态归零
}

void Widget::State_to_zero()//状态归零
{
    ui->lb_key_cs->setText("当前选择:None");
    ui->mode3_input->setText("输入");
    if(button_class == 1){
        cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
    }
    else if(button_class == 2){
        cfgUnit->bt_r[button_choose - 1]->setStyleSheet(style_big_black);
    }
    else if(button_class == 3){
        cfgUnit->bt_e[button_choose - 1]->setStyleSheet(style_big_black);
    }
    button_class = button_choose = 0;
    key_num = 0;
}

void Widget::on_Bt_mouse_L_clicked(){    key_handle(246);   }//鼠标左键
void Widget::on_Bt_mouse_M_clicked(){    key_handle(247);   }//鼠标中键
void Widget::on_Bt_mouse_R_clicked(){    key_handle(248);   }//鼠标右键

void Widget::on_Bt_wheel_up_clicked(){    key_handle(240);   }//鼠标滚轮向上
void Widget::on_Bt_wheel_down_clicked(){    key_handle(241);   }//鼠标滚轮向下

void Widget::on_Bt_vol_up_clicked(){    key_handle(242);   }//媒体音量加
void Widget::on_Bt_vol_down_clicked(){    key_handle(243);   }//媒体音量减
void Widget::on_Bt_vol_mute_clicked(){    key_handle(244);   }//媒体静音
void Widget::on_Bt_vol_stop_clicked(){    key_handle(245);   }//媒体停止
void Widget::on_Bt_vol_next_clicked(){    key_handle(237);   }//媒体下一个
void Widget::on_Bt_vol_prev_clicked(){    key_handle(238);   }//媒体上一个

void Widget::on_Bt_empty_clicked(){    key_handle(0);   }//空按键



void Widget::on_Bt_pos_get_clicked()//位置捕获
{
    //State_to_zero();//状态归零
    setWindowOpacity(0.7);//改为半透明
    ui->Bt_pos_get->setStyleSheet(style_mid_gray);
    ifPos = true;
}

void Widget::on_Bt_pos_input_clicked()//位置填入
{
    uint16_t x = ui->spinBox_x->value();
    uint16_t y = ui->spinBox_y->value();
    uint16_t w = ui->spinBox_w->value();
    uint16_t h = ui->spinBox_h->value();
    if(x >= w || y >= h){
        QMessageBox::information(this,"提示","坐标XY应分别小于屏幕长宽");
        return;
    }
    if(state > 0 && button_class == 1){//键盘
        if(cfgUnit->get_key_mode(button_choose - 1) == m3_group){
            if(!cfgUnit->add_mode3_pos(button_choose - 1,x,y)){
                QMessageBox::critical(this,"提示","此键可用空间不足");
            }
        }
        else if(cfgUnit->get_key_mode(button_choose - 1) == m4_move
                || cfgUnit->get_key_mode(button_choose - 1) == m5_click){
            cfgUnit->set_mode4_key(button_choose - 1,x,y);
            cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
            state = 0;
        }
    }
}

void Widget::on_Bt_delay_clicked()//时间填入
{
    if(state > 0 && button_class == 1){//键盘
        if(cfgUnit->get_key_mode(button_choose - 1) == m3_group){
            if(!cfgUnit->add_mode3_delay(button_choose - 1,ui->spinBox_delay->value())){
                QMessageBox::critical(this,"提示","此键可用空间不足");
            }
        }
        else if(cfgUnit->get_key_mode(button_choose - 1) == m7_fast){
            cfgUnit->set_mode7_time(button_choose - 1,ui->spinBox_delay->value());
            cfgUnit->bt_k[button_choose - 1]->setStyleSheet(style_big_black);
            state = 0;
        }
    }
}













