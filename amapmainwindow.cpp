#pragma execution_character_set("utf-8")//防止中文乱码
#include "amapmainwindow.h"
#include "ui_amapmainwindow.h"
#include <QNetworkProxyFactory>
#include<QDoubleValidator>
#include <QSize>
#include<QFileDialog>
#include<QTextStream>
AMapMainWindow::AMapMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AMapMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AMapMarkTools地图打点小工具");
    p_AmapViewWedget = new QMainWindow();//用于放置webView
    p_AmapView=new QWebEngineView(p_AmapViewWedget);//将QWebEngineView的父Widget为p_AmapViewWedget
    p_AmapChannel = new QWebChannel(p_AmapView->page());   //创建通道,与JS交互
    p_AmapBridge=new JSWebBridge(p_AmapViewWedget); //创建通道,与JS交互
    p_AmapChannel->registerObject("amap_bridge",(QObject*)p_AmapBridge);// 注册名"amap_bridge"与JS中用到的名称保持相同
    p_AmapView->page()->setWebChannel(p_AmapChannel);//View与chanel建立连接关系
    p_AmapViewWedget->setCentralWidget(p_AmapView);
    p_AmapHLayoutWidget = new QWidget(this);//用于将webView放置在AMapMainWindow
    p_AmapHLayoutWidget->setObjectName(QString::fromUtf8("p_AmapHLayoutWidget"));
    p_AmapHLayoutWidget->setGeometry(QRect(0, 0, 720, 480));//窗口大小为720*480
    p_AmapHLayoutWidget->move(150,0);
    p_AmapHLayout=new QHBoxLayout(p_AmapHLayoutWidget);
    p_AmapHLayout->addWidget((QWidget *)p_AmapViewWedget);
    setLayout(p_AmapHLayout);
    QNetworkProxyFactory::setUseSystemConfiguration(false);//不用代理
    QString htmlFilePath =QApplication::applicationDirPath()+"/amapmarktools.html";
    qDebug()<<htmlFilePath;
    loadMarkFilePath=QApplication::applicationDirPath();
    p_AmapView->page()->load(QUrl(htmlFilePath));
    p_AmapView->page()->setBackgroundColor(Qt::transparent);//背景透明
    this->setMinimumWidth(720);
    this->setMinimumHeight(480);
    p_operaterLayoutWidget=new QWidget(this);
    p_addOneMark=new QPushButton(p_operaterLayoutWidget);
    p_addOneMark->setFixedSize(QSize(100,30));
    p_addOneMark->setText("增加标记");
    p_addMarkFromFile=new QPushButton(p_operaterLayoutWidget);
    p_addMarkFromFile->setFixedSize(QSize(100,30));
    p_addMarkFromFile->setText("从文件导入标记");
    p_removeAllMark=new QPushButton(p_operaterLayoutWidget);
    p_removeAllMark->setFixedSize(QSize(100,30));
    p_removeAllMark->setText("删除所有标记");
    p_saveAllMarkToFile=new QPushButton(p_operaterLayoutWidget);
    p_saveAllMarkToFile->setFixedSize(QSize(100,30));
    p_saveAllMarkToFile->setText("导出所有标记");
    p_operaterLayout=new QVBoxLayout(p_operaterLayoutWidget);
    p_operaterLayoutWidget->setGeometry(QRect(0, 0, 200, 480));
    p_operaterLayoutWidget->move(0,0);
    latLineEdit=new QLineEdit(p_operaterLayoutWidget);
    lngLineEdit=new QLineEdit(p_operaterLayoutWidget);
    nameLineEdit=new QLineEdit(p_operaterLayoutWidget);
    latLineEdit->setFixedSize(QSize(100,20));
    latLineEdit->setPlaceholderText("gps维度:23.4");
    lngLineEdit->setFixedSize(QSize(100,20));
    lngLineEdit->setPlaceholderText("gps经度:113.4");
    nameLineEdit->setFixedSize(QSize(100,20));
    nameLineEdit->setPlaceholderText("标记名称(可选)");
    QDoubleValidator *validator = new QDoubleValidator(0, 360, 10, this);
    lngLineEdit->setValidator(validator);
    latLineEdit->setValidator(validator);
    p_operaterLayout->addWidget(latLineEdit);
    p_operaterLayout->addWidget(lngLineEdit);
    p_operaterLayout->addWidget(nameLineEdit);
    p_operaterLayout->addWidget(p_addOneMark);
    p_operaterLayout->addWidget(p_addMarkFromFile);
    p_operaterLayout->addWidget(p_saveAllMarkToFile);
    p_operaterLayout->addWidget(p_removeAllMark);
    setLayout(p_operaterLayout);
    p_operaterLayout->setAlignment(Qt::AlignJustify);
    connect(p_addOneMark, SIGNAL(clicked()),this, SLOT(addOneMarkBtnClickEvent()));
    connect(p_removeAllMark, SIGNAL(clicked()),this, SLOT(removeAllMarkBtnClickEvent()));
    connect(p_addMarkFromFile, SIGNAL(clicked()),this, SLOT(addMarkFromFileBtnClickEvent()));
    connect(p_saveAllMarkToFile, SIGNAL(clicked()),this, SLOT(saveAllMarkToFileBtnClickEvent()));
}

AMapMainWindow::~AMapMainWindow()
{
    delete ui;
}

void AMapMainWindow::resizeEvent(QResizeEvent* size)
{
    QSize m_size= this->size();
    p_AmapHLayoutWidget->setGeometry(QRect(0, 0, m_size.width()-150,m_size.height()));//窗口大小为720*480
    p_AmapHLayoutWidget->move(150,0);
    p_operaterLayoutWidget->setGeometry(QRect(0, 0, 150, m_size.height()));
    p_operaterLayoutWidget->move(0,0);
}
void AMapMainWindow::addOneMark(double lat,double lng,QString labelname)
{
    QString latStr = QString::number(lat, 10, 6);
    QString lngStr = QString::number(lng, 10, 6);
    QString jsCmd=QString("addOneMarker('%1','%2','%3')").arg(latStr).arg(lngStr).arg(labelname);
    qDebug()<<jsCmd;
    p_AmapView->page()->runJavaScript(jsCmd);
}

void AMapMainWindow::addOneMarkBtnClickEvent()
{
      qDebug()<<"addOneMarkBtnClickEvent";
      if((lngLineEdit->text().length()<1)||(latLineEdit->text().length()<1))
          return;

      double longtitude=lngLineEdit->text().toDouble();
      double latitude=latLineEdit->text().toDouble();
//      int tmp=longtitude;
//      longtitude=((tmp%100)+longtitude-tmp)/60+tmp/100;
//      tmp=latitude;
//      latitude=((tmp%100)+latitude-tmp)/60+tmp/100;
      if(nameLineEdit->text().length()>=1)
      {
          addOneMark(latitude,longtitude,nameLineEdit->text());
      }
      else
      {
          addOneMark(latitude,longtitude,"");
      }

      getMarkerIconpath((QApplication::applicationDirPath()+"/markerIcon.png"));
}
void AMapMainWindow::removeAllMarkBtnClickEvent()
{
    removeAllMark();
}
void AMapMainWindow::getMarkerIconpath(QString path)
{
    static bool setFlg=false;

    if(setFlg==false)
    {
        QString jsCmd=QString("getMarkerIconpath('%1')").arg(path);
        qDebug()<<jsCmd;
        p_AmapView->page()->runJavaScript(jsCmd);
        setFlg=true;
    }

}
void  AMapMainWindow::removeAllMark()
{
    QString jsCmd=QString("removeAllMark()");
    qDebug()<<jsCmd;
    p_AmapView->page()->runJavaScript(jsCmd);
}

void AMapMainWindow::addMarkFromFileBtnClickEvent()
{
     getMarkerIconpath((QApplication::applicationDirPath()+"/markerIcon.png"));
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("打开文件"));
    fileDialog.setDirectory(loadMarkFilePath);
    fileDialog.setNameFilter(tr("*.txt *.log *.*"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setViewMode(QFileDialog::Detail);
    //fileDialog.setGeometry(200,200,300,300);
    if(fileDialog.exec() == QDialog::Accepted)
    {
        QString path = fileDialog.selectedFiles()[0];//select first filename
        fileDialog.close();
        QFile rfile(path);
        if(rfile.open(QIODevice::ReadOnly)== true)
        {

            QTextStream readTextStream(&rfile); //创建输出流
            while(!readTextStream.atEnd())
            {
                double lng=-1;
                double lat=-1;
                QString markName;
                QString oneLineText = readTextStream.readLine();  //读取一行
                QString tmpText=oneLineText;
                int i=0;
                do
                {
                    if(i>2)
                        break;
                    if((tmpText.contains(",")))
                    {

                        tmpText=oneLineText.section(',', (i)).trimmed();
                        qDebug()<<"i="<<i<<",tmpText"<<tmpText;
                        bool isOk=false;
                            switch (i)
                            {
                            case 0:

                                oneLineText.section(',', i,i).trimmed().toDouble(&isOk);
                                if(isOk)
                                {
                                 lng=oneLineText.section(',', i,i).trimmed().toDouble();
                                 qDebug()<<"经度:"<<lng;
                                }
                                break;
                            case 1:
                                 oneLineText.section(',', i,i).trimmed().toDouble(&isOk);
                                 if(isOk)
                                 {
                                     lat=oneLineText.section(',', i,i).trimmed().toDouble();
                                     qDebug()<<"维度:"<<lat;
                                 }
                                break;
                            case 2:
                                markName=oneLineText.section(',', i,i).trimmed();
                                qDebug()<<"名字:"<<markName;
                                break;
                            default:
                                break;

                            }
                     }
                    i++;
                }while(tmpText.size()>0);
                if(lng>0&&lat>0)
                {
                    qDebug()<<"addOneMark"<<lat<<","<<lng<<","<<markName;
                    addOneMark(lat,lng,markName);
                }
            }

        }
    }

}
void AMapMainWindow::saveAllMarkToFile()
{

    QString jsCmd=QString("saveMarkToFile()");
    qDebug()<<jsCmd;
    p_AmapView->page()->runJavaScript(jsCmd);
}
void AMapMainWindow::saveAllMarkToFileBtnClickEvent()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("选择保存路径"),
        "",
        tr("*.txt;; *.log;; *.csv;; *.*")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        p_AmapBridge->saveAllMarkPath=filename;
        qDebug()<<(p_AmapBridge->saveAllMarkPath);
    }
    saveAllMarkToFile();
}
