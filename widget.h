#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include<QDesktopServices>
#include<QTimer>
#include<QUrl>
#include"filecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonSave1_clicked();

    void on_pushButtonSave2_clicked();

    void on_pushButtonSave3_clicked();

    void on_pushButtonLoad1_clicked();

    void on_pushButtonLoad2_clicked();

    void on_pushButtonLoad3_clicked();

    void on_pushButtonOpenFile_clicked();

    void on_pushButtonChangeZPoint_clicked();

    void on_toolButtonGitHub_clicked();

    void on_toolButtonAbout_clicked();

private:
    Ui::Widget *ui;
    FileController *p_FileController;
    QTimer *timer;

    void initUpdateTimer();
    void updateLoadTime();
    void updateSaveTime();
};
#endif // WIDGET_H
