#pragma execution_character_set("utf-8")//防止中文乱码
#include "amapmainwindow.h"
#include "ui_amapmainwindow.h"
#include <QNetworkProxyFactory>
#include<QDoubleValidator>
#include <QSize>
#include<QFileDialog>
#include<QTextStream>
#include<QMessageBox>
#include<QThread>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
AMapMainWindow::AMapMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AMapMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MarkTools地图打点小工具");
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
    p_AmapView=new QWebEngineView(p_AmapViewWedget);//将QWebEngineView的父Widget为p_AmapViewWedget
    p_AmapChannel = new QWebChannel(p_AmapView->page());   //创建通道,与JS交互
    p_AmapBridge=new JSWebBridge(p_AmapViewWedget); //创建通道,与JS交互
    p_AmapChannel->registerObject("amap_bridge",(QObject*)p_AmapBridge);// 注册名"amap_bridge"与JS中用到的名称保持相同
    p_AmapView->page()->setWebChannel(p_AmapChannel);//View与chanel建立连接关系
    p_AmapViewWedget->setCentralWidget(p_AmapView);
    p_AmapHLayout=new QHBoxLayout(p_AmapHLayoutWidget);
    p_AmapHLayout->addWidget((QWidget *)p_AmapViewWedget);
    QNetworkProxyFactory::setUseSystemConfiguration(false);//不用代理
    QString htmlFilePath =QApplication::applicationDirPath()+"/amapmarktools.html";
    qDebug()<<htmlFilePath;
    loadMarkFilePath=QApplication::applicationDirPath();
    p_AmapView->page()->load(QUrl(htmlFilePath));
    p_AmapView->page()->setBackgroundColor(Qt::transparent);//背景透明
    setLayout(p_AmapHLayout);
    this->setMinimumWidth(720);
    this->setMinimumHeight(480);
    //baiduMap
    p_BaiduViewWedget = new QMainWindow();//用于放置webView
    p_BaiduView=new QWebEngineView(p_BaiduViewWedget);//将QWebEngineView的父Widget为p_BaiduViewWedget
    p_BaiduChannel = new QWebChannel(p_BaiduView->page());   //创建通道,与JS交互
    p_BaiduBridge=new JSWebBridge(p_BaiduViewWedget); //创建通道,与JS交互
    p_BaiduChannel->registerObject("bmap_bridge",(QObject*)p_BaiduBridge);// 注册名"amap_bridge"与JS中用到的名称保持相同
    p_BaiduView->page()->setWebChannel(p_BaiduChannel);//View与chanel建立连接关系
    p_BaiduViewWedget->setCentralWidget(p_BaiduView);
    p_BaiduHLayoutWidget = new QWidget(this);//用于将webView放置在AMapMainWindow
    p_BaiduHLayoutWidget->setObjectName(QString::fromUtf8("p_BaiduHLayoutWidget"));
    p_BaiduHLayoutWidget->setGeometry(QRect(0, 0, 720, 480));//窗口大小为720*480
    p_BaiduHLayoutWidget->move(0,0);
    p_BaiduView=new QWebEngineView(p_BaiduViewWedget);//将QWebEngineView的父Widget为p_BaiduViewWedget
    p_BaiduChannel = new QWebChannel(p_BaiduView->page());   //创建通道,与JS交互
    p_BaiduBridge=new JSWebBridge(p_BaiduViewWedget); //创建通道,与JS交互
    p_BaiduChannel->registerObject("amap_bridge",(QObject*)p_BaiduBridge);// 注册名"amap_bridge"与JS中用到的名称保持相同
    p_BaiduView->page()->setWebChannel(p_BaiduChannel);//View与chanel建立连接关系
    p_BaiduViewWedget->setCentralWidget(p_BaiduView);
    p_BaiduHLayout=new QHBoxLayout(p_BaiduHLayoutWidget);
    p_BaiduHLayout->addWidget((QWidget *)p_BaiduViewWedget);
    htmlFilePath =QApplication::applicationDirPath()+"/baidu-map.html";
    qDebug()<<htmlFilePath;
    p_BaiduView->page()->load(QUrl(htmlFilePath));
    p_BaiduView->page()->setBackgroundColor(Qt::transparent);//背景透明
    setLayout(p_BaiduHLayout);


    p_operaterLayoutWidget=new QWidget(this);
    p_addOneMark=new QPushButton(p_operaterLayoutWidget);
    p_addOneMark->setFixedSize(QSize(100,30));
    p_addOneMark->setText("增加一个标记");
    p_addMarkFromFile=new QPushButton(p_operaterLayoutWidget);
    p_addMarkFromFile->setFixedSize(QSize(100,30));
    p_addMarkFromFile->setText("文件导入标记");
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
    logLngColumnEdit=new QLineEdit(p_operaterLayoutWidget);//经度所在列
    logLatColumnEdit=new QLineEdit(p_operaterLayoutWidget);//维度所在列
    logStartRowEdit=new QLineEdit(p_operaterLayoutWidget);//开始打点的行
    logNameColumnEdit=new QLineEdit(p_operaterLayoutWidget);//打点名所在列
    selectLogGPSFormat= new QComboBox(p_operaterLayoutWidget);

    latLineEdit->setFixedSize(QSize(100,20));
    latLineEdit->setPlaceholderText("gps维度:23.4");
    lngLineEdit->setFixedSize(QSize(100,20));
    lngLineEdit->setPlaceholderText("gps经度:113.4");
    nameLineEdit->setFixedSize(QSize(100,20));
    nameLineEdit->setPlaceholderText("标记名称(可选)");
    selectLogGPSFormat->setFixedSize(QSize(130,30));
    logLngColumnEdit->setFixedSize(QSize(100,20));
    logLatColumnEdit->setFixedSize(QSize(100,20));
    logStartRowEdit->setFixedSize(QSize(100,20));
    logNameColumnEdit->setFixedSize(QSize(130,20));
    logLngColumnEdit->setPlaceholderText("经度列号:2");
    logLatColumnEdit->setPlaceholderText("维度列号:3");
    logStartRowEdit->setPlaceholderText("开始打点行:2");
    logNameColumnEdit->setPlaceholderText("标记名列号(可选)");
    selectLogGPSFormat->addItem("格式:dd.ddddd");
    selectLogGPSFormat->addItem("格式:ddmm.mmmm");
    QDoubleValidator *validator = new QDoubleValidator(0, 360, 10, this);
    lngLineEdit->setValidator(validator);
    latLineEdit->setValidator(validator);
    p_operaterLayout->addWidget(latLineEdit);
    p_operaterLayout->addWidget(lngLineEdit);
    p_operaterLayout->addWidget(nameLineEdit);
    p_operaterLayout->addWidget(p_addOneMark);
    p_operaterLayout->addWidget(logNameColumnEdit);
    p_operaterLayout->addWidget(logLngColumnEdit);
    p_operaterLayout->addWidget(logLatColumnEdit);
    p_operaterLayout->addWidget(logStartRowEdit);
    p_operaterLayout->addWidget(selectLogGPSFormat);
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
    p_AmapHLayoutWidget->setGeometry(QRect(0, 0,(m_size.width()-150)/2,m_size.height()));//窗口大小为720*480
    p_AmapHLayoutWidget->move(150,0);
    p_BaiduHLayoutWidget->setGeometry(QRect(0, 0, (m_size.width()-150)/2,m_size.height()));//窗口大小为720*480
    p_BaiduHLayoutWidget->move(150+(m_size.width()-150)/2,0);
    p_operaterLayoutWidget->setGeometry(QRect(0, 0, 150, m_size.height()));
    p_operaterLayoutWidget->move(0,0);



}
void AMapMainWindow::addOneMark(double lat,double lng,QString labelname)
{
    QString latStr = QString::number(lat, 12, 8);
    QString lngStr = QString::number(lng, 12, 8);
    QString jsCmd=QString("addOneMarker('%1','%2','%3')").arg(latStr).arg(lngStr).arg(labelname);
    qDebug()<<jsCmd;
    p_AmapView->page()->runJavaScript(jsCmd);
    p_BaiduView->page()->runJavaScript(jsCmd);
    QThread::msleep(35);
}

void AMapMainWindow::addOneMarkBtnClickEvent()
{
      qDebug()<<"addOneMarkBtnClickEvent";
      if((lngLineEdit->text().length()<1)||(latLineEdit->text().length()<1))
          return;

      int curGPSFormat=0;//当前索引,0->dd.ddddd  1->ddmm.mmmmm，格式1需要转换
       curGPSFormat=selectLogGPSFormat->currentIndex();//当前索引

      double longtitude=lngLineEdit->text().toDouble();
      double latitude=latLineEdit->text().toDouble();
//      int tmp=longtitude;
//      longtitude=((tmp%100)+longtitude-tmp)/60+tmp/100;
//      tmp=latitude;
//      latitude=((tmp%100)+latitude-tmp)/60+tmp/100;

      if(GPSFormat_DDMM_MMMMMM==curGPSFormat)//转换后
      {
          // ddmm.mmmmmm
          //dd.dddddd=dd+mm.mmmmmm/60
          double tmpLat=latitude;
          int latInt=((int)tmpLat);
          double  latDouble=(tmpLat-latInt);
          latitude=latInt/100+(latInt%100+latDouble)/60;
          qDebug()<<"latInt:"<<latInt<<"latDouble:"<<latDouble<<"caclLat="<<latitude;
          double tmpLng=longtitude;
          int lngInt=((int)tmpLng);
          double  lngDouble=(tmpLng-lngInt);
          longtitude=lngInt/100+(lngInt%100+lngDouble)/60;
          qDebug()<<"lngInt:"<<lngInt<<"lngDouble:"<<lngDouble<<"caclLng="<<longtitude;
      }

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
        p_BaiduView->page()->runJavaScript(jsCmd);
        setFlg=true;
    }

}
void  AMapMainWindow::removeAllMark()
{
    QString jsCmd=QString("removeAllMark()");
    qDebug()<<jsCmd;
    p_AmapView->page()->runJavaScript(jsCmd);
    p_BaiduView->page()->runJavaScript(jsCmd);
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
    int lngColumn=0;//经度所在列
    int latColumn=0;//纬度所在列
    int startRow=0;//开始打点的行
    int rowCnt=0;//当前行计数
    int curGPSFormat=0;//当前索引,0->dd.ddddd  1->ddmm.mmmmm，格式1需要转换
    int valindPointCnt=0;
    int invalindPointCnt=0;


    int markNameColumn=-1;//默认文件记号列为空
    if(logLngColumnEdit->text().isEmpty())
    {
        lngColumn=2;
    }
    else
    {
         lngColumn=logLngColumnEdit->text().toInt();
    }
    if(logLatColumnEdit->text().isEmpty())
    {
        latColumn=3;
    }
    else
    {
        latColumn=logLatColumnEdit->text().toInt();
    }
    if(logStartRowEdit->text().isEmpty())
    {
         startRow=2;
    }
    else
    {
        startRow=logStartRowEdit->text().toInt();
    }
    if(!(logNameColumnEdit->text().isEmpty()))
    {
        markNameColumn=logNameColumnEdit->text().toInt();
    }


    curGPSFormat=selectLogGPSFormat->currentIndex();//当前索引

    qDebug()<<"lngColumn="<<lngColumn<<",latColumn="<<latColumn<<",startRow="<<startRow<<",curGPSFormat="<<curGPSFormat<<",markNameColumn="<<markNameColumn;
    //fileDialog.setGeometry(200,200,300,300);
    int defaultNameUseCnt=0;
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
                QString oneLineText = readTextStream.readLine();//读取一行
                
                rowCnt++;
                if(rowCnt<startRow)//控制从文本框行开始输入
                {
                    continue;
                }
                QString tmpText=oneLineText;
                int i=0;
                int totalSymbolCnt=0;
                if(tmpText.contains(","))
                {
                    QString caclTotalSymbolCntStr=oneLineText;//计算用
                    caclTotalSymbolCntStr.replace(",","");
                    totalSymbolCnt=oneLineText.length()-caclTotalSymbolCntStr.length();
                    qDebug()<<"totalSymbolCnt:"<<totalSymbolCnt;
                }
                defaultNameUseCnt++;
                do
                {
                    if(i>totalSymbolCnt)
                        break;
                         
                    if((tmpText.contains(",")))
                    {
                        tmpText=oneLineText.section(',',(i)).trimmed();
                        qDebug()<<"i="<<i<<",tmpText:"<<tmpText;
                        bool isOk=false;
                        if(i==lngColumn-1)//匹配经度输入的列
                        {

                            oneLineText.section(',', lngColumn-1,lngColumn-1).trimmed().toDouble(&isOk);
                            if(isOk)
                            {
                                lng=oneLineText.section(',', lngColumn-1,lngColumn-1).trimmed().toDouble();
                                qDebug()<<"输入经度:"<<lng;
                                
                                if(GPSFormat_DDMM_MMMMMM==curGPSFormat)
                                {
                                    // ddmm.mmmmmm
                                    //dd.dddddd=dd+mm.mmmmmm/60
                                    double tmpLng=lng;
                                    int lngInt=((int)tmpLng);
                                    double  lngDouble=(tmpLng-lngInt);
                                    lng=lngInt/100+(lngInt%100+lngDouble)/60;
                                    qDebug()<<"lngInt:"<<lngInt<<"lngDouble:"<<lngDouble<<"caclLng="<<lng;
                                }
                                qDebug()<<"实际打点经度:"<<lng;
                            }
                            goto Get_Kng_Lat_Lable_End;
                        }
                        if(i==latColumn-1)//匹配纬度输入的列
                        {
                            oneLineText.section(',', latColumn-1,latColumn-1).trimmed().toDouble(&isOk);
                            if(isOk)
                            {
                                lat=oneLineText.section(',', latColumn-1,latColumn-1).trimmed().toDouble();
                                qDebug()<<"输入纬度:"<<lat;
                                if(GPSFormat_DDMM_MMMMMM==curGPSFormat)//转换后
                                {
                                    // ddmm.mmmmmm
                                    //dd.dddddd=dd+mm.mmmmmm/60
                                    double tmpLat=lat;
                                    int latInt=((int)tmpLat);
                                    double  latDouble=(tmpLat-latInt);
                                    lat=latInt/100+(latInt%100+latDouble)/60;
                                    qDebug()<<"latInt:"<<latInt<<"latDouble:"<<latDouble<<"caclLat="<<lat;
                                }
                                qDebug()<<"实际打点纬度:"<<lat;
                            }
                            goto Get_Kng_Lat_Lable_End;

                        }

                        if( (markNameColumn>0) && (i==markNameColumn-1) )//匹配标记名所在的列
                        {
                            markName=oneLineText.section(',', markNameColumn-1,markNameColumn-1).trimmed();
                            qDebug()<<"输入标记名字:"<<markName;
                            goto Get_Kng_Lat_Lable_End;
                        }
                        else if(markName.isEmpty())
                        {
                            markName=QString::number(defaultNameUseCnt);
                            qDebug()<<"标记名字:"<<markName;
                            goto Get_Kng_Lat_Lable_End;
                        }

                     }
Get_Kng_Lat_Lable_End:
                    i++;
                }while(tmpText.size()>0);
                if(lng>0&&lat>0)
                {
                    qDebug()<<"addOneMark"<<lat<<","<<lng<<","<<markName;
                    addOneMark(lat,lng,markName);
                    valindPointCnt++;
                }
                else
                {
                    qDebug()<<"============invalid Mark=="<<lat<<","<<lng<<","<<markName;
                    invalindPointCnt++;
                }
            }
            QString infoShow="有效点:";
            infoShow+=QString::number(valindPointCnt);
            infoShow+=",无效点:";
            infoShow+=QString::number(invalindPointCnt);
            QMessageBox message(QMessageBox::NoIcon, "show info", infoShow, QMessageBox::Yes , NULL); 
            if(message.exec() == QMessageBox::Yes) 
            { 
                //
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
