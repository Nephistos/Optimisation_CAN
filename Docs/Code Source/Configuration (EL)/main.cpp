#include "widget.h"
#include <QApplication>
#include <QTextEdit>   // afficher le contenu du fichier
#include <QFile>       // Pour utiliser le fichier
#include <QString>     // Stocke le contenu du fichier
#include <QTextStream> // Flux sortant du fichier


int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        Widget w;

        w.show();

    return a.exec();
}
