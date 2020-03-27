#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tokens.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Run_clicked()
{
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();
    print.clear();
    string gstr;
    QString str=ui->textEdit->toPlainText();
    QStringList strList=str.split('\n');
    for(int i=0; i<strList.size(); ++i) {
       string str_1=strList[i].toStdString();
       gstr+=" "+ str_1;
    }
    try {
    InterpreteExperssion(gstr);
    for(int i=0; i<int(print.size()); ++i) {
    ui->textEdit_2->append(QString::number(print[unsigned(i)])+"\n"); }
    for(int i=0; i<int(onScrenn.size()); ++i) {
        if(onScrenn[unsigned(i)].GetOperator()==Operators::not_equal_to)
            ui->textEdit_3->append("!=");
         if(onScrenn[unsigned(i)].GetOperator()==Operators::less_equal)
            ui->textEdit_3->append("<=");
         if(onScrenn[unsigned(i)].GetOperator()==Operators::greater_equal)
            ui->textEdit_3->append(">=");
         if(onScrenn[unsigned(i)].GetOperator()==Operators::equal_to)
             ui->textEdit_3->append("==");
        if(onScrenn[unsigned(i)].GetOperator()!=Operators::not_equal_to &&
           onScrenn[unsigned(i)].GetOperator()!=Operators::less_equal &&
           onScrenn[unsigned(i)].GetOperator()!=Operators::greater_equal &&
           onScrenn[unsigned(i)].GetOperator()!=Operators::equal_to)
        ui->textEdit_3->append(QString::fromStdString(ToString(onScrenn[unsigned(i)]))+" ");
    }
    } catch (const exception &ex) {
    ui->textEdit_2->setText(ex.what()); }
}
