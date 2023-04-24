#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "threadpool.h"
#include <iostream>
#include <QMainWindow>
#include <QtConcurrent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_addTaskButton_clicked();

    void on_initValueSpinBox_valueChanged(int newValue);

    void on_endValueSpinBox_valueChanged(int newValue);

private:
    Ui::MainWindow *ui;
    threadpool *threadPool;
    bool isPoolStarted = false;
};

#endif // MAINWINDOW_H
