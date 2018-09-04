#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QStringListModel>
#include <QStringList>
#include <QHeaderView>
#include <QStandardItem>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QObject::connect(ui->getFinleInfo_button,SIGNAL(clicked()),this,SLOT(getFileInfo()));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::getFileInfo()
{
    QString    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("ALL Files (*.*)"));
    if(fileName.isEmpty()){
        return;
    }
    QFileInfo info(fileName);

    QString myFileName=info.fileName();
    qint64 size=info.size();
    QDateTime createTime=info.created();
    QDateTime lastModifyTime=info.lastModified();
    QDateTime lastReadTime=info.lastRead();

    QString filename="File";
    QString fileSize="Size";
    QString tSize=QString::number(size,10);
    QString fileCreateTime="Created Time";
    QString fileLastModifyTime="Last Modify Time";
    QString fileLastReadTime="Last Read Time";


    bool isDir=info.isDir();
    bool isFile=info.isFile();
    //bool isSymLink = info.isSymLink();
    bool isHidden = info.isHidden();
    bool isReadable = info.isReadable();
    bool isWritable = info.isWritable();
    bool isExecutable =info.isExecutable();

    QString fType="File Type";
    QString fileType;

    QString iHide="Hide";
    QString hid;

    QString iRead="Readable";
    QString readAble;

    QString iWrite="Writeable";
    QString writeAble;

    QString iExecuable="Executable";
    QString execAble;

    if(isDir) fileType="Directory";
    if(isFile) fileType="File";

    if(isHidden)hid="Yes";
    if(!isHidden)hid="No";

    if(isReadable) readAble="Yes";
    if(!isReadable) readAble="No";

    if(isWritable) writeAble="Yes";
    if(!isWritable) writeAble="No";

    if(isExecutable) execAble="Yes";
    if(!isExecutable) execAble="No";

    QString kOwner="Owner";
    QString kGroup="Group";
    QString kPath="Path";
    QString vOwner;
    QString vGroup;
    QString vPath;
    vOwner=info.owner();
    vGroup=info.group();
    vPath=info.path();

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Property")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("Value")));

    model->setItem(0, 0, new QStandardItem(filename));
    model->setItem(0, 1, new QStandardItem(myFileName));
    model->setItem(1, 0, new QStandardItem(fileSize));
    model->setItem(1, 1, new QStandardItem(tSize));
    model->setItem(2, 0, new QStandardItem(fileCreateTime));
    model->setItem(2, 1, new QStandardItem(createTime.toString()));
    model->setItem(3, 0, new QStandardItem(fileLastModifyTime));
    model->setItem(3, 1, new QStandardItem(lastModifyTime.toString()));
    model->setItem(4, 0, new QStandardItem(fileLastReadTime));
    model->setItem(4, 1, new QStandardItem(lastReadTime.toString()));

    model->setItem(5, 0, new QStandardItem(fType));
    model->setItem(5, 1, new QStandardItem(fileType));
    model->setItem(6, 0, new QStandardItem(iHide));
    model->setItem(6, 1, new QStandardItem(hid));
    model->setItem(7, 0, new QStandardItem(iRead));
    model->setItem(7, 1, new QStandardItem(readAble));
    model->setItem(8, 0, new QStandardItem(iWrite));
    model->setItem(8, 1, new QStandardItem(writeAble));
    model->setItem(9, 0, new QStandardItem(iExecuable));
    model->setItem(9, 1, new QStandardItem(execAble));
    model->setItem(10, 0, new QStandardItem(kOwner));
    model->setItem(10, 1, new QStandardItem(vOwner));
    model->setItem(11, 0, new QStandardItem(kGroup));
    model->setItem(11, 1, new QStandardItem(vGroup));
    model->setItem(12, 0, new QStandardItem(kPath));
    model->setItem(12, 1, new QStandardItem(vPath));
    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,600);

}
