#include <QApplication>
#include "PrivateWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PrivateWindow window;
    window.show();
    return app.exec();
}