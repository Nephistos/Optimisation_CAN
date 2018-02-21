#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::change_it(int it)
{
    nb_it = it;
}

void Widget::apply()
{

    //ouverture du fichier en lecture

    //declaration des variables pour la lecture du fichier
    QString texte, tempo;
    int count=0;
    QFile fichier("../cfg.txt");
    QTextStream flux(&fichier);

    //on ouvre le fichier avec des vérifications
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //on balaye chacune de ses lignes
        while(!flux.atEnd())
        {
            //on récupère les paramètres intéréssant
            count++;

            if(count==44)
            {
                texte += QString::number(nb_it) + '\n';
                tempo = flux.readLine();
            }
            else
                texte += flux.readLine() + '\n';
        }
        //on ferme le fichier
        texte.resize(texte.size()-1); // Élimine le '\n' en trop
        fichier.close();
    }

    //ouverture du fichier en écriture

    fichier.open(QIODevice::WriteOnly | QIODevice::Text);

    flux << texte;
    fichier.close();

}
