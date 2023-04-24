#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "threadpool.h"
#include "factorial.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<QVector<int>>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    QFuture<void> future = QtConcurrent::run([=] () {
    threadPool = new threadpool(ui->numThreadsBox->value());
    });
    isPoolStarted = true;
    ui->infoTable->setRowCount(0);
}

void MainWindow::on_stopButton_clicked()
{
    if(isPoolStarted)
    {
        threadPool->clearQueue();
        cout << "pressed stop" << endl;
    }
}

void MainWindow::on_addTaskButton_clicked()
{
    if (isPoolStarted)
    {
        QString taskName = "Task: " + QString::number(ui->initValueSpinBox->value()) + "!"
                + " ... " + QString::number(ui->endValueSpinBox->value()) + "!";

        int newRowIndex = ui->infoTable->rowCount();
        ui->infoTable->insertRow(newRowIndex);
        ui->infoTable->setItem(newRowIndex, 0, new QTableWidgetItem(taskName));
        ui->infoTable->setItem(newRowIndex, 1, new QTableWidgetItem("In Queue"));

        threadPool->doJob([=] ()
        {
            ui->infoTable->setItem(newRowIndex, 1, new QTableWidgetItem("In Process"));
            cout << "doing job " << newRowIndex << endl;
            for (int i = ui->initValueSpinBox->value(); i < ui->endValueSpinBox->value(); ++i)
            {
                cout << getFactorial(i) << endl;
                if (i == ui->endValueSpinBox->value() - 1)
                   ui->infoTable->setItem(newRowIndex, 2,
                                          new QTableWidgetItem(QString::number(getFactorial(i), 'e', 4)));
            }
            cout << "job done " << newRowIndex << endl;
            ui->infoTable->setItem(newRowIndex, 1, new QTableWidgetItem("Done"));
        });
    }
}

void MainWindow::on_initValueSpinBox_valueChanged(int newValue)
{
    if (newValue > ui->endValueSpinBox->value()) ui->initValueSpinBox->setValue(ui->endValueSpinBox->value() - 1);
}

void MainWindow::on_endValueSpinBox_valueChanged(int newValue)
{
    if (newValue < ui->initValueSpinBox->value()) ui->endValueSpinBox->setValue(ui->initValueSpinBox->value() + 1);
}
