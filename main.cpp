#include <QtWidgets>
#include <QApplication>
QColor color;
QString filePath="";
class Window : public QWidget{
public:
    Window();
    void openButtonClick();
    void openImage (const QString &path=QString());
private:
    QLabel *textLabel;
    QLabel *label;
    QLabel *labelRGB;
    QLabel *labelR;

    void mouseMoveEvent(QMouseEvent *event);
    QLineEdit *pathEdit;
    QPushButton *openButton;
    void paintEvent(QPaintEvent *event);
    QPixmap pixmap1;
};
Window::Window(){
    resize(1500,900);

    setWindowTitle("PixelPicker");
    label = new QLabel(this);
    label->setGeometry(0,0,200,300);
    label->setFont(QFont("Arial", 10));

    textLabel=new QLabel(filePath, this);
    textLabel->setGeometry(0,0,250,20);

    labelR=new QLabel(this);
    labelR->setGeometry(0,425,200,200);
    labelR->setText("R=");
    labelR->setFont(QFont("Arial", 20));

    labelRGB=new QLabel(this);
    labelRGB->setGeometry(40,425,250,200);
    labelRGB->setFont(QFont("Arial", 20));

    setMouseTracking(true);

    pathEdit=new QLineEdit(this);
    pathEdit->setGeometry(0,30,250,20);
    pathEdit->setText(filePath);
    openButton=new QPushButton("Open photo", this);
    openButton->setGeometry(0,60,80,30);
    connect(openButton, &QPushButton::clicked, this, &Window::openButtonClick);

}
void Window::mouseMoveEvent(QMouseEvent *event){
    int x = event -> x();
    int y = event -> y();
    label->setText(QString("Mouse coordinates: [%1;%2]").arg(x).arg(y));
    QWidget::grab(QRect(x, y, 1, 1)).toImage().pixelColor(0, 0);

    qDebug()<<event->globalX()<<event->globalY()<<endl;
    QScreen *screen=QGuiApplication::primaryScreen();
    if (!screen){
        return;
    }
    QPixmap pixmap=screen->grabWindow(0);
    QImage img=pixmap.toImage();
    QRgb rgb=img.pixel(event->globalX(), event->globalY());
    color.setRgb(rgb);
    labelRGB->setText(QString::number(color.red())+" G=" + QString::number(color.green()) + " B=" + QString::number(color.blue()));
}
void Window::openButtonClick(){
filePath=pathEdit->text();
openImage();
update();
}
void Window::openImage(const QString &path){
filePath=path;
if (filePath.isNull()){
    filePath=QFileDialog::getOpenFileName(this, "Open Image", " ", "Image Files(*.png *.jpg *.bmp)");
 }

if (!filePath.isEmpty()){
if (!pixmap1.load(filePath)){
QMessageBox::warning(this, "Open Image", "The Image file could not be loaded", QMessageBox::Cancel);
return;
   }
  }
textLabel->setText(filePath);
pathEdit->setText(filePath);
}
void Window::paintEvent(QPaintEvent *){
QPainter painter1(this);
QPainter painter2(this);
pixmap1.load(filePath);
QPixmap scaledPixmap = pixmap1.scaled(900,1024, Qt::KeepAspectRatio);
painter1.drawPixmap(600,0, scaledPixmap);

painter2.setBrush(color);
painter2.drawRect(50,400,50,50);
update();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.show();
    return a.exec();
}
