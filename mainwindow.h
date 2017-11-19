#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void trackVideo(QString &fileName);
    void startTracking();

private slots:
    void on_actionOpen_Video_triggered();

    void on_actionExit_triggered();

    void on_toolOpenVideo_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
