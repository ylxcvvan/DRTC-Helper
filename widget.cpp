#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,p_FileController(new FileController)
{
    ui->setupUi(this);

    ui->lineEditHomePath->setText(p_FileController->getHomePath());
    ui->lineEditZPoint->setText(QString("%0").arg(p_FileController->getZPoint()));

    initUpdateTimer();
}

Widget::~Widget()
{
    delete p_FileController;
    delete timer;
    delete ui;
}

void Widget::on_pushButtonSave1_clicked()
{
    if(!p_FileController->Save(0))
        QMessageBox::information(this,"错误","保存失败,该存档不存在！");
    updateLoadTime();
}


void Widget::on_pushButtonSave2_clicked()
{
    if(!p_FileController->Save(1))
        QMessageBox::information(this,"错误","保存失败,该存档不存在！");
    updateLoadTime();
}


void Widget::on_pushButtonSave3_clicked()
{
    if(!p_FileController->Save(2))
        QMessageBox::information(this,"错误","保存失败,该存档不存在！");
    updateLoadTime();
}


void Widget::on_pushButtonLoad1_clicked()
{
    if(!p_FileController->Load(0))
        QMessageBox::information(this,"错误","读取失败,该存档不存在！");
}


void Widget::on_pushButtonLoad2_clicked()
{
    if(!p_FileController->Load(1))
        QMessageBox::information(this,"错误","读取失败,该存档不存在！");
}


void Widget::on_pushButtonLoad3_clicked()
{
    if(!p_FileController->Load(2))
        QMessageBox::information(this,"错误","读取失败,该存档不存在！");
}


void Widget::on_pushButtonOpenFile_clicked()
{
    QString homePath=p_FileController->getHomePath();
    if (!homePath.isEmpty()) {
        QUrl url = QUrl::fromLocalFile(homePath);
        QDesktopServices::openUrl(url);
    } else {
        qDebug() << "Home path is empty.";
    }
}

void Widget::on_pushButtonChangeZPoint_clicked()
{
    bool isDone=p_FileController->setZPoint(ui->lineEditZPoint->text().toInt());
    if(isDone)
    {
        QMessageBox::information(this,"成功！","僵点子(僵尸点数)修改成功！\n快打开游戏准备大杀四方吧!");
        ui->lineEditZPoint->setText(QString("%0").arg(p_FileController->getZPoint()));
    }
    else
    {
        QMessageBox::warning(this,"错误!","修改失败,\n请检查文件目录是否正确。");
        return;
    }

}

void Widget::initUpdateTimer()
{
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [this](){
        updateLoadTime();
        updateSaveTime();
    });
}

void Widget::updateLoadTime()
{
    ui->pushButtonLoad1->setText(p_FileController->getSaveTime(0));
    ui->pushButtonLoad1->setEnabled(ui->pushButtonLoad1->text().size()>=10);

    ui->pushButtonLoad2->setText(p_FileController->getSaveTime(1));
    ui->pushButtonLoad2->setEnabled(ui->pushButtonLoad2->text().size()>=10);

    ui->pushButtonLoad3->setText(p_FileController->getSaveTime(2));
    ui->pushButtonLoad3->setEnabled(ui->pushButtonLoad3->text().size()>=10);
    QFont font;

    font = ui->pushButtonLoad1->font();
    if (ui->pushButtonLoad1->text().size() >= 10) {
        font.setPointSize(10);
    } else {
        font.setPointSize(12);
    }
    ui->pushButtonLoad1->setFont(font);

    font = ui->pushButtonLoad2->font();
    if (ui->pushButtonLoad2->text().size() >= 10) {
        font.setPointSize(10);
    } else {
        font.setPointSize(12);
    }
    ui->pushButtonLoad2->setFont(font);


    font = ui->pushButtonLoad3->font();
    if (ui->pushButtonLoad3->text().size() >= 10) {
        font.setPointSize(10);
    } else {
        font.setPointSize(12);
    }
    ui->pushButtonLoad3->setFont(font);
}

void Widget::updateSaveTime()
{
    ui->pushButtonSave1->setText(p_FileController->getThisTime(0));
    ui->pushButtonSave1->setEnabled(ui->pushButtonSave1->text().size()>3);

    ui->pushButtonSave2->setText(p_FileController->getThisTime(1));
    ui->pushButtonSave2->setEnabled(ui->pushButtonSave2->text().size()>3);

    ui->pushButtonSave3->setText(p_FileController->getThisTime(2));
    ui->pushButtonSave3->setEnabled(ui->pushButtonSave3->text().size()>3);
}


void Widget::on_toolButtonGitHub_clicked()
{
    QUrl url("https://github.com/ylxcvvan/DRTC-Helper");
    QDesktopServices::openUrl(url);
}


void Widget::on_toolButtonAbout_clicked()
{
    QMessageBox::information(this,"使用说明","存档的三个槽位对应游戏内的三个存档。玩家可在游戏的过程中任意存档。\n读档时，需退出到标题页面或重启游戏才能读档\n若游戏数据目录获取错误，则本软件无法使用！");
}

