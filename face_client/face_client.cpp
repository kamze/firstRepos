#include "face_client.h"
//#include "ui_face_client.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QTime>

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , getFortuneButton(new QPushButton(tr("Get Fortune")))
    , tcpSocket(new QTcpSocket(this))
    , networkSession(Q_NULLPTR)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    QLabel *portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(portLineEdit);
    timeServer_Label = new QLabel();
    timeClient_Label = new QLabel();
    pression_Label = new QLabel();
    temperature_Label = new QLabel();
    altitude_Label = new QLabel();

    statusLabel = new QLabel(tr("This examples requires that you run the "
                                "Fortune Server example as well."));
    statusLabel1 = new QLabel();

    imageLbl = new QLabel();


    getFortuneButton->setDefault(true);
    getFortuneButton->setEnabled(false);

    QPushButton *quitButton = new QPushButton(tr("Quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetFortuneButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetFortuneButton);

    connect(getFortuneButton, &QAbstractButton::clicked,
           this, &Client::requestNewFortune);

    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::decode);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &Client::displayError);

    QGridLayout *mainLayout = Q_NULLPTR;
    if (QGuiApplication::styleHints()->showIsFullScreen()) {
        QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(buttonBox, 2,  0, 1, 2);
    mainLayout->addWidget(imageLbl, 2, 2);
    mainLayout->addWidget(timeServer_Label, 3, 1);
    mainLayout->addWidget(timeClient_Label, 4, 1);
    mainLayout->addWidget(pression_Label, 5, 1);
    mainLayout->addWidget(temperature_Label, 6, 1);
    mainLayout->addWidget(altitude_Label, 7, 1);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        getFortuneButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
blocksize=0;

}

void Client::requestNewFortune()
{
    qDebug() << "requestNewFortune : ";

    getFortuneButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
}

// QTHread QObject QCoreApplication 'moeToThread
void Client::decode(){

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);


    qDebug() << "before: bytesAvailable : " << tcpSocket->bytesAvailable();
    std::vector<uchar> compressed_data;
    quint8 b;
    quint32 imgSize;
// si on veut que utiliser ip et non pas via localhost faut decommenter
    //  --------------------sensor-----------------
    double pression;
    double temperature;
    double altitude;

    in >> pression;
    in >> altitude;
    in >> temperature;

    QString phrase_pression= "pression : ";
    phrase_pression.append(QString::number(pression));
    QString phrase_altitude= "height above sea: ";
    phrase_altitude.append(QString::number(altitude));
    QString phrase_temperature= "temperature : " ;
    phrase_temperature.append(QString::number(temperature));

    pression_Label->setText(phrase_pression);
    temperature_Label->setText(phrase_temperature);
    altitude_Label->setText(phrase_altitude);

    qDebug() << "pressure : " << pression;
    qDebug() << "height above sea: " <<altitude;
    qDebug() << "temperature : " << temperature;


   // if(tcpSocket->waitForReadyRead()){

//  --------------------time-----------------
    QTime time = QTime::currentTime();
    QString format= "hh:mm:ss.zzz";
    QString timeClient_display = "Client: " ;
    QString timeServer;
    QString timeServer_display;

    timeClient_display.append(time.toString(format));
    in >> timeServer;
    timeServer_display= "Server: ";
    timeServer_display.append(timeServer);

    qDebug() << timeClient_display;
    qDebug() << timeServer_display;

    timeServer_Label->setText(timeServer_display);
    timeClient_Label->setText(timeClient_display);

//--------------size & img---------------
        in >> imgSize;
        qDebug() << "imgSize : " << imgSize;

        for(int i=0;i < imgSize;i++){
            in >> b;
            compressed_data.push_back((uchar)b);
}
   // }
    cv::Mat image = cv::imdecode(compressed_data,-1);


    imageLbl->setPixmap(QPixmap::fromImage(MatToQimage(image)));




    qDebug() << "----------------------------";
}

void Client::decompressImage()
{

}

QImage Client::MatToQimage(cv::Mat inMat){
    QImage image( inMat.data,
                              inMat.cols, inMat.rows,
                              static_cast<int>(inMat.step),
                              QImage::Format_RGB888 );

    return image;

}



void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    getFortuneButton->setEnabled(true);
}

void Client::enableGetFortuneButton()
{
    getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    statusLabel->setText(tr("This examples requires that you run the "
                            "Fortune Server example as well."));

    enableGetFortuneButton();
}
