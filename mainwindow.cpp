#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this);
    imageCapture = new QCameraImageCapture(camera);
    imageLabel = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(viewfinder);
    layout->addWidget(imageLabel);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->setViewfinder(viewfinder);
    camera->start();

    QPushButton *captureButton = new QPushButton("拍照", this);
    connect(captureButton, &QPushButton::clicked, this, &MainWindow::onCaptureButtonClicked);
    //captureButton->move(10, 10);
    //captureButton->move(width() / 2, height() - 20);
    captureButton->move(width() / 2 - captureButton->width() / 2, height() - 20);

    connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::onImageCaptured);
}

MainWindow::~MainWindow()
{
    camera->stop();
    delete camera;
}

void MainWindow::onCaptureButtonClicked()
{
    camera->searchAndLock();
    imageCapture->capture();
    camera->unlock();
}

void MainWindow::onImageCaptured(int id, const QImage& previewImage)
{
    capturedImage = previewImage;
    viewfinder->show();
    viewfinder->repaint();

    QPixmap pixmap = QPixmap::fromImage(capturedImage);
    QPixmap scaledPixmap = pixmap.scaled(viewfinder->size(), Qt::KeepAspectRatio);
    imageLabel->setPixmap(scaledPixmap);

    // 儲存影像到檔案
    //QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG Image (.jpg);;PNG Image (.png)");
    //if (!fileName.isEmpty())
    //capturedImage.save(fileName);
}
