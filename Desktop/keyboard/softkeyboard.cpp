#include "softkeyboard.h"
#include "ui_softkeyboard.h"
#include <QLineEdit>
#include <QFile>
#include <QPoint>
#include <QMessageBox>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1

#if (DEBUG==0 || DEBUG==1)
#include <stdio.h>
#include <stdlib.h>
#endif
#if (DEBUG==0)
#define debug(fmt,...) fprintf(stderr," "fmt"File: %s Function:%s LineNumber:%d",__VA_ARGS__,__FILE__,__FUNCTION__,__LINE__)
#elif (DEBUG==1)
#define debug(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#elif (DEBUG==2)
#define debug(fmt,...)
#endif

SoftKeyboard *SoftKeyboard::m_instance=NULL;

SoftKeyboard::SoftKeyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SoftKeyboard)
{
    ui->setupUi(this);

    m_capital=false; //字母大小写
    m_shift=false; //辅助功能
    m_language=false;
    m_text=NULL;
    vectorPos=0;
    numbers=0;
    pageNumbers=0;
    curPage=0;
    setWindowFlags(Qt::CustomizeWindowHint);
    createConnection();


    loadQssFile("qss/keyborder.qss");
    QPoint point=cursor().pos();
    move(point);

    ui->label_one->installEventFilter(this);
    ui->label_two->installEventFilter(this);
    ui->label_three->installEventFilter(this);
    ui->label_four->installEventFilter(this);
    ui->label_five->installEventFilter(this);
    ui->label_six->installEventFilter(this);
    ui->label_seven->installEventFilter(this);
    ui->label_eight->installEventFilter(this);

    file.setFileName("pinyin.txt");

    if( !file.open(QIODevice::ReadOnly)){
        qDebug()<<"open file is error";
    }

    while(!file.atEnd()){
        QString line=file.readLine();
        line.replace(line.size()-1,1," ");
        QStringList list=line.split("  ");
        QString key=QString::fromUtf8(list.at(0).toStdString().c_str());
        QString value=QString::fromUtf8(list.at(1).toStdString().c_str());
        qDebug()<<key<<"--------------"<<value;
        sMap.insert(list.at(0),list.at(1));
    }

}
SoftKeyboard::~SoftKeyboard()
{
    delete ui;
}
bool SoftKeyboard::eventFilter(QObject *object, QEvent *event)
{
    if(     object->objectName()=="label_two"||
            object->objectName()=="label_three"||
            object->objectName()=="label_four"||
            object->objectName()=="label_five"||
            object->objectName()=="label_six"||
            object->objectName()=="label_seven"||
            object->objectName()=="label_eight"){
        if(event->type()==QEvent::MouseButtonPress){
            QLabel *label=static_cast<QLabel *>(object);
            debug("=====%s\n",label->text().toStdString().c_str());
            QString line=m_text->text();
            line.append(label->text());
            m_text->setText(line);
            emit clearAll();
            return true;
        }
    }
    return QDialog::eventFilter(object,event);
}
void SoftKeyboard::transPy(QString pyInput)
{

    pinYin.append(pyInput);
    ui->label_one->setText(pinYin);
    getPy(pinYin);

}
void SoftKeyboard::transPy1(QString pyInput)
{
    pinYin.clear();
    pinYin.append(pyInput);
    ui->label_one->setText(pyInput);
    getPy(pyInput);
}
void SoftKeyboard::getPy(QString pyInput)
{
    static bool findNothing=true;
    vector.clear();
    numbers=0;


    //    regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(pyInput));
    //    while(!file.atEnd()){
    //        QByteArray data = file.readLine();
    //        int ret = regExp.indexIn(QString(data.data()),0,QRegExp::CaretAtZero);
    //        if( ret > -1)
    //        {
    //            findNothing=false;
    //            vector.append(regExp.cap(1).toAscii().data());
    //            ++numbers;
    //            if(numbers%7==0){
    //                ++pageNumbers;
    //            }
    //        }
    //    }
    //    file.seek(0);
    if(!sMap.contains(pyInput)){
        findNothing=true;
    }else{
        QMap<QString,QString>::iterator it=sMap.find(pyInput);
        QStringList list=it.value().split(" ");
        for(int i=0;i<list.size();++i){
            findNothing=false;
            vector.append(list.at(i));
            ++numbers;
            if(numbers%7==0){
                ++pageNumbers;
            }
        }
    }



    //    if(list.size()==0){
    //        findNothing=true;
    //    }
    //    for(int i=0;i<list.size();++i){

    //          vector.append(list.at(i));
    //          qDebug()<<list.at(i);
    //    }
    if(findNothing){
        ui->label_two->setText("");
        ui->label_three->setText("");
        ui->label_four->setText("");
        ui->label_five->setText("");
        ui->label_six->setText("");
        ui->label_seven->setText("");
        ui->label_eight->setText("");
    }

    findNothing = true;
    showPage(0);
}
void SoftKeyboard::showPage(int pageIndex)
{
    QString chinese[7]={""};
    vectorPos=pageIndex*7;
    for(int i=0;i<7;++i){
        if(vectorPos>=numbers){
            break;
        }
        chinese[i].append(vector.at(vectorPos));
        vectorPos++;
    }

    ui->label_two->setText(QString::fromUtf8(chinese[0].toAscii()));
    ui->label_three->setText(QString::fromUtf8(chinese[1].toAscii()));
    ui->label_four->setText(QString::fromUtf8(chinese[2].toAscii()));
    ui->label_five->setText(QString::fromUtf8(chinese[3].toAscii()));
    ui->label_six->setText(QString::fromUtf8(chinese[4].toAscii()));
    ui->label_seven->setText(QString::fromUtf8(chinese[5].toAscii()));
    ui->label_eight->setText(QString::fromUtf8(chinese[6].toAscii()));
}

void SoftKeyboard::pushButtonUpPageSlot()
{
    debug("%s ",ui->pushButton_upPage->text().toStdString().c_str());

    if(curPage>0){
        showPage(curPage-1);
        curPage-=1;
    }

}

void SoftKeyboard::pushButtonDownPageSlot()
{
    debug("%s ",ui->pushButton_downPage->text().toStdString().c_str());

    if(curPage<pageNumbers){
        showPage(curPage+1);
        curPage+=1;
    }
}
SoftKeyboard *SoftKeyboard::instance()
{
    if(m_instance==NULL){
        m_instance=new SoftKeyboard;
    }
    return m_instance;
}
void SoftKeyboard::setLineText(QLineEdit *text)
{
    m_text=text;
}

void SoftKeyboard::clearAllThing()
{
    pinYin.clear();
    curPage=0;
    numbers=0;
    ui->label_one->setText("");
    ui->label_two->setText("");
    ui->label_three->setText("");
    ui->label_four->setText("");
    ui->label_five->setText("");
    ui->label_six->setText("");
    ui->label_seven->setText("");
    ui->label_eight->setText("");
}

void SoftKeyboard::pushButtonQuotesSlot()
{
    QString text=ui->pushButton_quotes->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);

}
void SoftKeyboard::pushButtonOneSlot()
{
    QString text=ui->pushButton_one->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);

}
void SoftKeyboard::pushButtonTwoSlot()
{
    QString text=ui->pushButton_two->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);

}
void SoftKeyboard::pushButtonThreeSlot()
{
    QString text=ui->pushButton_three->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);


}
void SoftKeyboard::pushButtonFourSlot()
{
    QString text=ui->pushButton_four->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonFiveSlot()
{
    QString text=ui->pushButton_five->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonSixSlot()
{
    QString text=ui->pushButton_six->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonSevenSlot()
{
    QString text=ui->pushButton_seven->text();
    if(text.startsWith("&")){
        text="&";
    }
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonEightSlot()
{
    QString text=ui->pushButton_eight->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonNineSlot()
{
    QString text=ui->pushButton_nine->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonZeroSlot()
{
    QString text=ui->pushButton_zero->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonEqualSlot()
{
    QString text=ui->pushButton_equal->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonLineSlot()
{
    QString text=ui->pushButton_line->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonBackspaceSlot()
{
    debug("%s ",ui->pushButton_backspace->text().toStdString().c_str());
    if(m_text==NULL){
        return;
    }
    if(m_language){
        ui->label_one->setFocus();
        QString text=ui->label_one->text();
        if(text.isEmpty()){
            m_text->backspace();
        }
        ui->label_one->clear();
        text=text.left(text.length() - 1);
        transPy1(text);
    }else{
        m_text->backspace();
    }
}
void SoftKeyboard::pushButtonTabSlot()
{
    QString text=ui->pushButton_tab->text();
    debug("%s ",text.toStdString().c_str());
    QString str="    ";
    setLineEditText(str);
}
void SoftKeyboard::pushButtonQSlot()
{
    QString text=ui->pushButton_q->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}

void SoftKeyboard::pushButtonWSlot()
{
    QString text=ui->pushButton_w->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonESlot()
{
    QString text=ui->pushButton_e->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonRSlot()
{
    QString text=ui->pushButton_r->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonTSlot()
{
    QString text=ui->pushButton_t->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonYSlot()
{
    QString text=ui->pushButton_y->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonUSlot()
{
    QString text=ui->pushButton_u->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonISlot()
{
    QString text=ui->pushButton_i->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}

void SoftKeyboard::pushButtonOSlot()
{
    QString text=ui->pushButton_o->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}

void SoftKeyboard::pushButtonPSlot()
{
    QString text=ui->pushButton_p->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonLeftBracketSlot()
{
    QString text=ui->pushButton_left->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonRightBracketSlot()
{
    QString text=ui->pushButton_right->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonBackslashSlot()
{
    QString text=ui->pushButton_backslash->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonCapslockSlot()
{
    debug("%s ",ui->pushButton_capslock->text().toStdString().c_str());
    if(m_capital){
        setSmallLetter();
        m_capital=false;
    }else{
        setCapital();
        m_capital=true;
    }
}
void SoftKeyboard::pushButtonASlot()
{
    QString text=ui->pushButton_a->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonSSlot()
{
    QString text=ui->pushButton_s->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonDSlot()
{
    QString text=ui->pushButton_d->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonFSlot()
{
    QString text=ui->pushButton_f->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonGSlot()
{
    QString text=ui->pushButton_g->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonHSlot()
{
    QString text=ui->pushButton_h->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonJSlot()
{
    QString text=ui->pushButton_j->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonKSlot()
{
    QString text=ui->pushButton_k->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonLSlot()
{
    QString text=ui->pushButton_l->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonSemicolonSlot()
{
    QString text=ui->pushButton_semicolon->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonYinhaoSlot()
{
    QString text=ui->pushButton_yinhao->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonEnterSlot()
{
    debug("%s ",ui->pushButton_enter->text().toStdString().c_str());
    this->hide();
}

void SoftKeyboard::pushButtonShiftSlot()
{
    debug("%s ",ui->pushButton_shift->text().toStdString().c_str());
    if(m_shift){
        setNoShift();
        m_shift=false;
    }else{
        setShift();
        m_shift=true;
    }
}
void SoftKeyboard::pushButtonZSlot()
{
    QString text=ui->pushButton_z->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonXSlot()
{
    QString text=ui->pushButton_x->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonCSlot()
{
    QString text=ui->pushButton_c->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonVSlot()
{
    QString text=ui->pushButton_v->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonBSlot()
{
    QString text=ui->pushButton_b->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonNSlot()
{
    QString text=ui->pushButton_n->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonMSlot()
{
    QString text=ui->pushButton_m->text();
    debug("%s ",text.toStdString().c_str());
    if(m_language && !m_capital){
        transPy(text);
    }else{
        setLineEditText(text);
    }
}
void SoftKeyboard::pushButtonDouhaoSlot()
{
    QString text=ui->pushButton_douhao->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonJuhaoSlot()
{
    QString text=ui->pushButton_juhao->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonXiexianSlot()
{
    QString text=ui->pushButton_xiexian->text();
    debug("%s ",text.toStdString().c_str());
    setLineEditText(text);
}
void SoftKeyboard::pushButtonDelSlot()
{
    debug("%s ",ui->pushButton_del->text().toStdString().c_str());
    if(m_text==NULL){
        return;
    }
    m_text->del();
}

void SoftKeyboard::pushButtonSpaceSlot()
{
    debug("%s ",ui->pushButton_space->text().toStdString().c_str());
    QString str=" ";
    setLineEditText(str);
}
void SoftKeyboard::setCapital()
{
    ui->pushButton_a->setText("A");
    ui->pushButton_b->setText("B");
    ui->pushButton_c->setText("C");
    ui->pushButton_d->setText("D");
    ui->pushButton_e->setText("E");
    ui->pushButton_f->setText("F");
    ui->pushButton_g->setText("G");
    ui->pushButton_h->setText("H");
    ui->pushButton_i->setText("I");
    ui->pushButton_j->setText("J");
    ui->pushButton_k->setText("K");
    ui->pushButton_l->setText("L");
    ui->pushButton_m->setText("M");
    ui->pushButton_n->setText("N");
    ui->pushButton_o->setText("O");
    ui->pushButton_p->setText("P");
    ui->pushButton_q->setText("Q");
    ui->pushButton_r->setText("R");
    ui->pushButton_s->setText("S");
    ui->pushButton_t->setText("T");
    ui->pushButton_u->setText("U");
    ui->pushButton_v->setText("V");
    ui->pushButton_w->setText("W");
    ui->pushButton_x->setText("X");
    ui->pushButton_y->setText("Y");
    ui->pushButton_z->setText("Z");
}
void SoftKeyboard::setSmallLetter()
{
    ui->pushButton_a->setText("a");
    ui->pushButton_b->setText("b");
    ui->pushButton_c->setText("c");
    ui->pushButton_d->setText("d");
    ui->pushButton_e->setText("e");
    ui->pushButton_f->setText("f");
    ui->pushButton_g->setText("g");
    ui->pushButton_h->setText("h");
    ui->pushButton_i->setText("i");
    ui->pushButton_j->setText("j");
    ui->pushButton_k->setText("k");
    ui->pushButton_l->setText("l");
    ui->pushButton_m->setText("m");
    ui->pushButton_n->setText("n");
    ui->pushButton_o->setText("o");
    ui->pushButton_p->setText("p");
    ui->pushButton_q->setText("q");
    ui->pushButton_r->setText("r");
    ui->pushButton_s->setText("s");
    ui->pushButton_t->setText("t");
    ui->pushButton_u->setText("u");
    ui->pushButton_v->setText("v");
    ui->pushButton_w->setText("w");
    ui->pushButton_x->setText("x");
    ui->pushButton_y->setText("y");
    ui->pushButton_z->setText("z");
}

void SoftKeyboard::createConnection()
{
    QObject::connect(this,SIGNAL(clearAll()),this,SLOT(clearAllThing()));

    QObject::connect(ui->pushButton_language,SIGNAL(clicked()),this,SLOT(pushButtonLanguageSlot()));
    QObject::connect(ui->pushButton_upPage,SIGNAL(clicked()),this,SLOT(pushButtonUpPageSlot()));
    QObject::connect(ui->pushButton_downPage,SIGNAL(clicked()),this,SLOT(pushButtonDownPageSlot()));

    QObject::connect(ui->pushButton_quotes,SIGNAL(clicked()),this,SLOT(pushButtonQuotesSlot()));
    QObject::connect(ui->pushButton_one,SIGNAL(clicked()),this,SLOT(pushButtonOneSlot()));
    QObject::connect(ui->pushButton_two,SIGNAL(clicked()),this,SLOT(pushButtonTwoSlot()));
    QObject::connect(ui->pushButton_three,SIGNAL(clicked()),this,SLOT(pushButtonThreeSlot()));
    QObject::connect(ui->pushButton_four,SIGNAL(clicked()),this,SLOT(pushButtonFourSlot()));
    QObject::connect(ui->pushButton_five,SIGNAL(clicked()),this,SLOT(pushButtonFiveSlot()));
    QObject::connect(ui->pushButton_six,SIGNAL(clicked()),this,SLOT(pushButtonSixSlot()));
    QObject::connect(ui->pushButton_seven,SIGNAL(clicked()),this,SLOT(pushButtonSevenSlot()));
    QObject::connect(ui->pushButton_eight,SIGNAL(clicked()),this,SLOT(pushButtonEightSlot()));
    QObject::connect(ui->pushButton_nine,SIGNAL(clicked()),this,SLOT(pushButtonNineSlot()));
    QObject::connect(ui->pushButton_zero,SIGNAL(clicked()),this,SLOT(pushButtonZeroSlot()));
    QObject::connect(ui->pushButton_line,SIGNAL(clicked()),this,SLOT(pushButtonLineSlot()));
    QObject::connect(ui->pushButton_equal,SIGNAL(clicked()),this,SLOT(pushButtonEqualSlot()));
    QObject::connect(ui->pushButton_backspace,SIGNAL(clicked()),this,SLOT(pushButtonBackspaceSlot()));

    QObject::connect(ui->pushButton_tab,SIGNAL(clicked()),this,SLOT(pushButtonTabSlot()));
    QObject::connect(ui->pushButton_q,SIGNAL(clicked()),this,SLOT(pushButtonQSlot()));
    QObject::connect(ui->pushButton_w,SIGNAL(clicked()),this,SLOT(pushButtonWSlot()));
    QObject::connect(ui->pushButton_e,SIGNAL(clicked()),this,SLOT(pushButtonESlot()));
    QObject::connect(ui->pushButton_r,SIGNAL(clicked()),this,SLOT(pushButtonRSlot()));
    QObject::connect(ui->pushButton_t,SIGNAL(clicked()),this,SLOT(pushButtonTSlot()));
    QObject::connect(ui->pushButton_y,SIGNAL(clicked()),this,SLOT(pushButtonYSlot()));
    QObject::connect(ui->pushButton_u,SIGNAL(clicked()),this,SLOT(pushButtonUSlot()));
    QObject::connect(ui->pushButton_i,SIGNAL(clicked()),this,SLOT(pushButtonISlot()));
    QObject::connect(ui->pushButton_o,SIGNAL(clicked()),this,SLOT(pushButtonOSlot()));
    QObject::connect(ui->pushButton_p,SIGNAL(clicked()),this,SLOT(pushButtonPSlot()));
    QObject::connect(ui->pushButton_left,SIGNAL(clicked()),this,SLOT(pushButtonLeftBracketSlot()));
    QObject::connect(ui->pushButton_right,SIGNAL(clicked()),this,SLOT(pushButtonRightBracketSlot()));
    QObject::connect(ui->pushButton_backslash,SIGNAL(clicked()),this,SLOT(pushButtonBackslashSlot()));

    QObject::connect(ui->pushButton_capslock,SIGNAL(clicked()),this,SLOT(pushButtonCapslockSlot()));
    QObject::connect(ui->pushButton_a,SIGNAL(clicked()),this,SLOT(pushButtonASlot()));
    QObject::connect(ui->pushButton_s,SIGNAL(clicked()),this,SLOT(pushButtonSSlot()));
    QObject::connect(ui->pushButton_d,SIGNAL(clicked()),this,SLOT(pushButtonDSlot()));
    QObject::connect(ui->pushButton_f,SIGNAL(clicked()),this,SLOT(pushButtonFSlot()));
    QObject::connect(ui->pushButton_g,SIGNAL(clicked()),this,SLOT(pushButtonGSlot()));
    QObject::connect(ui->pushButton_h,SIGNAL(clicked()),this,SLOT(pushButtonHSlot()));
    QObject::connect(ui->pushButton_j,SIGNAL(clicked()),this,SLOT(pushButtonJSlot()));
    QObject::connect(ui->pushButton_k,SIGNAL(clicked()),this,SLOT(pushButtonKSlot()));
    QObject::connect(ui->pushButton_l,SIGNAL(clicked()),this,SLOT(pushButtonLSlot()));
    QObject::connect(ui->pushButton_semicolon,SIGNAL(clicked()),this,SLOT(pushButtonSemicolonSlot()));
    QObject::connect(ui->pushButton_yinhao,SIGNAL(clicked()),this,SLOT(pushButtonYinhaoSlot()));
    QObject::connect(ui->pushButton_enter,SIGNAL(clicked()),this,SLOT(pushButtonEnterSlot()));

    QObject::connect(ui->pushButton_shift,SIGNAL(clicked()),this,SLOT(pushButtonShiftSlot()));
    QObject::connect(ui->pushButton_z,SIGNAL(clicked()),this,SLOT(pushButtonZSlot()));
    QObject::connect(ui->pushButton_x,SIGNAL(clicked()),this,SLOT(pushButtonXSlot()));
    QObject::connect(ui->pushButton_c,SIGNAL(clicked()),this,SLOT(pushButtonCSlot()));
    QObject::connect(ui->pushButton_v,SIGNAL(clicked()),this,SLOT(pushButtonVSlot()));
    QObject::connect(ui->pushButton_b,SIGNAL(clicked()),this,SLOT(pushButtonBSlot()));
    QObject::connect(ui->pushButton_n,SIGNAL(clicked()),this,SLOT(pushButtonNSlot()));
    QObject::connect(ui->pushButton_m,SIGNAL(clicked()),this,SLOT(pushButtonMSlot()));
    QObject::connect(ui->pushButton_douhao,SIGNAL(clicked()),this,SLOT(pushButtonDouhaoSlot()));
    QObject::connect(ui->pushButton_juhao,SIGNAL(clicked()),this,SLOT(pushButtonJuhaoSlot()));
    QObject::connect(ui->pushButton_xiexian,SIGNAL(clicked()),this,SLOT(pushButtonXiexianSlot()));
    QObject::connect(ui->pushButton_del,SIGNAL(clicked()),this,SLOT(pushButtonDelSlot()));
    QObject::connect(ui->pushButton_space,SIGNAL(clicked()),this,SLOT(pushButtonSpaceSlot()));
}
void SoftKeyboard::setShift()
{
    ui->pushButton_quotes->setText("~");
    ui->pushButton_one->setText("!");
    ui->pushButton_two->setText("@");
    ui->pushButton_three->setText("#");
    ui->pushButton_four->setText("$");
    ui->pushButton_five->setText("%");
    ui->pushButton_six->setText("^");
    ui->pushButton_seven->setText("&&");
    ui->pushButton_eight->setText("*");
    ui->pushButton_nine->setText("(");
    ui->pushButton_zero->setText(")");
    ui->pushButton_line->setText("_");
    ui->pushButton_equal->setText("+");

    ui->pushButton_left->setText("{");
    ui->pushButton_right->setText("}");
    ui->pushButton_backslash->setText("|");

    ui->pushButton_semicolon->setText(":");
    ui->pushButton_yinhao->setText("\"");

    ui->pushButton_douhao->setText("<");
    ui->pushButton_juhao->setText(">");
    ui->pushButton_xiexian->setText("?");
}
void SoftKeyboard::setNoShift()
{
    ui->pushButton_quotes->setText("`");
    ui->pushButton_one->setText("1");
    ui->pushButton_two->setText("2");
    ui->pushButton_three->setText("3");
    ui->pushButton_four->setText("4");
    ui->pushButton_five->setText("5");
    ui->pushButton_six->setText("6");
    ui->pushButton_seven->setText("7");
    ui->pushButton_eight->setText("8");
    ui->pushButton_nine->setText("9");
    ui->pushButton_zero->setText("0");
    ui->pushButton_line->setText("-");
    ui->pushButton_equal->setText("=");

    ui->pushButton_left->setText("[");
    ui->pushButton_right->setText("]");
    ui->pushButton_backslash->setText("\\");

    ui->pushButton_semicolon->setText(";");
    ui->pushButton_yinhao->setText("\'");

    ui->pushButton_douhao->setText(",");
    ui->pushButton_juhao->setText(".");
    ui->pushButton_xiexian->setText("/");
}
void SoftKeyboard::setLineEditText(QString text)
{
    if(m_text==NULL){
        return;
    }
    QString editContent=m_text->text();
    editContent+=text;
    m_text->setText(editContent);
}
void SoftKeyboard::loadQssFile(QString fileName)
{
    QFile qssFile(fileName);
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()){
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }
}
void SoftKeyboard::pushButtonLanguageSlot()
{
    debug("%s ",ui->pushButton_language->text().toStdString().c_str());

    if(m_language){
        m_language=false;
    }else{
        m_language=true;
    }
}
