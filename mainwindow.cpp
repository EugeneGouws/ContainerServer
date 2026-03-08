#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlreader.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QStandardItemModel>
#include <QNetworkInterface>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new Server(this);

    QWidget *centralWidget = new QWidget(this);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topLine = new QHBoxLayout();

    // Widgets
    QLabel *label = new QLabel("Listening to port: ");
    QLabel *portLabl = new QLabel(QString::number(server->getPort()));

    // Get local IP
    QString ipAddress;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (QHostAddress addr : list) {
        if (addr != QHostAddress::LocalHost && addr.toIPv4Address()) {
            ipAddress = addr.toString();
            break;
        }
    }
    QLabel *ipLabel = new QLabel("Server IP: " + ipAddress);


    QTableView *table = new QTableView();

    // Table model and initial setup
    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({
        "Pallet", "Container","Code", "Height", "Length/Diameter", "Breadth", "Weight"
    });
    table->setModel(model);

    // Assemble
    topLine->addWidget(label);
    topLine->addWidget(portLabl);
    topLine->addWidget(ipLabel);

    mainLayout->addLayout(topLine);
    mainLayout->addWidget(table);


    // Finalize
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Container Server");
    setWindowIcon(QIcon(":/images/server_icon.ico"));

    //slots
    connect(server,SIGNAL(dataReceived()),this,SLOT(handleDataReceived()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleDataReceived()
{
    qInfo() << "handeling data received";
    XmlReader reader;
    reader.readXml(server->getData(),model);
}
