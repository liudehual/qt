#ifndef SOFTKEYBOARD_H
#define SOFTKEYBOARD_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QRegExp>
#include <QVector>
#include <QMap>
namespace Ui {
  class SoftKeyboard;
}
class QLineEdit;

class SoftKeyboard : public QDialog
{
  Q_OBJECT

public:
  void setLineText(QLineEdit *text);

  static SoftKeyboard *instance();
protected:
  bool eventFilter(QObject *, QEvent *);
private:
  explicit SoftKeyboard(QWidget *parent = 0);
  ~SoftKeyboard();
signals:
  void clearAll();

public slots:
  void clearAllThing();
public slots:
  //键盘第一行
  void pushButtonLanguageSlot();
  void pushButtonDelSlot();
  void pushButtonUpPageSlot();
  void pushButtonDownPageSlot();
  //键盘第二行
  void pushButtonQuotesSlot();
  void pushButtonOneSlot();
  void pushButtonTwoSlot();
  void pushButtonThreeSlot();
  void pushButtonFourSlot();
  void pushButtonFiveSlot();
  void pushButtonSixSlot();
  void pushButtonSevenSlot();
  void pushButtonEightSlot();
  void pushButtonNineSlot();
  void pushButtonZeroSlot();
  void pushButtonEqualSlot();
  void pushButtonLineSlot();
  void pushButtonBackspaceSlot();

  //键盘第三行
  void pushButtonTabSlot();
  void pushButtonQSlot();
  void pushButtonWSlot();
  void pushButtonESlot();
  void pushButtonRSlot();
  void pushButtonTSlot();
  void pushButtonYSlot();
  void pushButtonUSlot();
  void pushButtonISlot();
  void pushButtonOSlot();
  void pushButtonPSlot();
  void pushButtonLeftBracketSlot();
  void pushButtonRightBracketSlot();
  void pushButtonBackslashSlot();

  //键盘第四行
  void pushButtonCapslockSlot();
  void pushButtonASlot();
  void pushButtonSSlot();
  void pushButtonDSlot();
  void pushButtonFSlot();
  void pushButtonGSlot();
  void pushButtonHSlot();
  void pushButtonJSlot();
  void pushButtonKSlot();
  void pushButtonLSlot();
  void pushButtonSemicolonSlot();
  void pushButtonYinhaoSlot();
  void pushButtonEnterSlot();

  //键盘第五行
  void pushButtonShiftSlot();
  void pushButtonZSlot();
  void pushButtonXSlot();
  void pushButtonCSlot();
  void pushButtonVSlot();
  void pushButtonBSlot();
  void pushButtonNSlot();
  void pushButtonMSlot();
  void pushButtonDouhaoSlot();
  void pushButtonJuhaoSlot();
  void pushButtonXiexianSlot();
  void pushButtonSpaceSlot();
private:
  void setCapital(); //大写
  void setSmallLetter();//小写
  void createConnection(); //创建连接关系
  void setShift();//shift键有效设置
  void setNoShift(); //shift键无效设置
  void setLineEditText(QString);
  void loadQssFile(QString);
  void transPy(QString pyInput);
  void transPy1(QString pyInput);
  void getPy(QString pyInput);
  void showPage(int pageIndex);

private:
  bool m_language;
  bool m_capital;
  bool m_shift;

  QLineEdit *m_text;
  QFile file;
  QFile out;
  QRegExp regExp;
  QString pinYin;

  QVector<QString> vector;
  QMap<QString,QString> sMap;
  int vectorPos;
  int numbers;
  int pageNumbers;
  int curPage;


  static SoftKeyboard *m_instance;

private:
  Ui::SoftKeyboard *ui;

};

#endif // SOFTKEYBOARD_H
