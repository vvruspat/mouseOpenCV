#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <QFileDialog>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pointsTable->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Video_triggered()
{
    MainWindow::startTracking();
}

void MainWindow::startTracking() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Video"), "",
            tr("Video (*.mp4, *.mov);;"));

    if (fileName != "") {
        MainWindow::trackVideo(fileName);
    }
}

void MainWindow::trackVideo(QString &fileName) {

//    graphicsView
    QGraphicsScene *graphScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(graphScene);

    string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
    string trackerType = trackerTypes[4];
    Ptr<Tracker> tracker;

    if (trackerType == "BOOSTING")
        tracker = TrackerBoosting::create();
    if (trackerType == "MIL")
        tracker = TrackerMIL::create();
    if (trackerType == "KCF")
        tracker = TrackerKCF::create();
    if (trackerType == "TLD")
        tracker = TrackerTLD::create();
    if (trackerType == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    if (trackerType == "GOTURN")
        tracker = TrackerGOTURN::create();

    const string file = fileName.toStdString();

    VideoCapture video(file);

    // Read first frame
    Mat frame;
    bool ok = video.read(frame);
    // Define initial boundibg box
    Rect2d bbox(287, 23, 86, 320);
    Rect2d prevBbox;

    // Uncomment the line below to select a different bounding box
    bbox = selectROI(frame, false);

    // Display bounding box.
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);

    tracker->init(frame, bbox);

    prevBbox = bbox;

    while(video.read(frame))
    {
        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        int rowNumber = ui->pointsTable->rowCount();

        ui->pointsTable->setRowCount(rowNumber + 1);

        ui->pointsTable->setItem(rowNumber, 0, new QTableWidgetItem(QString::number(video.get(cv::CAP_PROP_POS_MSEC))));
        ui->pointsTable->setItem(rowNumber, 1, new QTableWidgetItem(QString::number(bbox.x)));
        ui->pointsTable->setItem(rowNumber, 2, new QTableWidgetItem(QString::number(bbox.y)));

        graphScene->addLine(prevBbox.x, prevBbox.y, bbox.x, bbox.y, QPen(QBrush(Qt::black),1));
        prevBbox = bbox;
        ui->graphicsView->show();

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,100), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }

        // Display tracker type on frame
        putText(frame, "Press ESC to stop tracking", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,60), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);

        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_toolOpenVideo_clicked()
{
    MainWindow::startTracking();
}
