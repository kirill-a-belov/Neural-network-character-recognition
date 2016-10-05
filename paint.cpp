#include "paint.h"
#include "ui_paint.h"

Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    scene = new paintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену

    timer = new QTimer();       // Инициализируем таймер
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);          // Запускаем таймер

    bitMatrix.resize(4096); //Задаём полный размер битовой карты
   // for(int i=0; i<count; ++i )    new( &foos[ i ] ) Foo( a, b );

    for (int i=0; i<36; i++) //Инициализируем нейроны
    {
        neurons[i].init((char) ui->comboBox->itemText(i+1).toLatin1().data()[0]);
    }
}

Paint::~Paint()
{
    delete ui;
}

void Paint::slotTimer()
{
    // Переопределяем размеры графической сцены в зависимости от размеров окна

    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Paint::on_pushButton_clicked() //Очистить
{

    ui->label_2->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->progressBar->setValue(0);
    bitMatrix.fill(false);
    ui->tableWidget->clear();
    scene->clear();
}

void Paint::on_pushButton_2_clicked() //Обучить
{
    ui->tableWidget->clear();
    ui->label_2->clear();
    QPixmap pixmap = QPixmap::grabWidget(ui->graphicsView);
    QImage image = pixmap.toImage();
    // Чистка массива
    bitMatrix.fill(false);
    //Перебираем исходный рисунок и оцифровываем (составляем битовую карту) в массив

    for (short int i = 0; i<64; ++i)
    {
      for (short int j = 0; j<64; ++j)
      {
          short int final = 0;
          for (short int x = (i*8); x<(8+i*8); ++x)
          {
            for (short int y = (j*8); y<(8+j*8); ++y)
            {
                if (QColor(image.pixel(y,x)) != QColor(Qt::white))
                {
                    final++;
                }
             }
          }
          if (final > 31)
          {
              bitMatrix.setBit((i*64 +j), true);
          }

      }
    }

        //Отправка матрицы битов нейрону на обучение
    if (ui->comboBox->currentIndex() >0)
    {
        neurons[(ui->comboBox->currentIndex()-1)].train(bitMatrix);
      //  ui->comboBox->setCurrentIndex(0);
        scene->clear();
    }
}

void Paint::on_pushButton_3_clicked() //Распознать
{

      ui->lineEdit->clear();
      ui->lineEdit_2->clear();
      ui->lineEdit_3->clear();
      ui->lineEdit_4->clear();
      ui->progressBar->setValue(0);


      QPixmap pixmap = QPixmap::grabWidget(ui->graphicsView);
      QImage image = pixmap.toImage();
      int bitCount = 0;

      // Чистка массива
      bitMatrix.fill(false);

      //Перебираем исходный рисунок и оцифровываем (составляем битовую карту) в массив

      for (short int i = 0; i<64; ++i)
      {
        for (short int j = 0; j<64; ++j)
        {
            short int final = 0;
            for (short int x = (i*8); x<(8+i*8); ++x)
            {
              for (short int y = (j*8); y<(8+j*8); ++y)
              {
                  if (QColor(image.pixel(y,x)) != QColor(Qt::white))
                  {
                      final++;
                  }
               }
            }
            if (final > 31)
            {
                bitMatrix.setBit((i*64 +j), true);
                bitCount++;
            }

        }
      }
      // Визуализация битовой карты (чтобы наглядно видеть оцифровку и память нейрона)
      for (short int i = 0; i<64; ++i)
      {
        for (short int j = 0; j<64; ++j)
        {
           if (bitMatrix.testBit(i*64 +j)== true)
           {
               ui->tableWidget->setItem(i,j,new QTableWidgetItem());
               ui->tableWidget->item( i, j)->setBackground( Qt::green);
           }

        }
      }


      // Распознование символа!!!!!
      int result =0;
      int resultPrev =0;
      int resultId = -1;
      for (int i=0; i<36; i++) // Прогон по всем нейронам, выявление результирующих весов
      {
          if (neurons[i].analyze(bitMatrix) > result)
          {
              resultPrev = result;
              result = neurons[i].analyze(bitMatrix);
              resultId = i;
          }
      }
      /* Условие принятия решения:
       * Если нейрон найден и количество веса больше порога (подбирается эксперементально)
       * программа выдаёт решение
       */
      if ((resultId >=0)&& (result))
      {
       ui->label_2->setText(QString::fromLatin1(&neurons[resultId].name));
       ui->comboBox_2->setCurrentIndex(resultId+1);
       ui->pushButton_5->click();
      } else
      {
         ui->label_2->setText("-");
      }

      ui->lineEdit->setText(QString::number(bitCount));

      ui->lineEdit_2->setText(QString::number(result));

        //расчёт вероятности распознования
      ui->lineEdit_3->setText(QString::number((double)result/bitCount*10,'f',1).append("%"));

      ui->lineEdit_4->setText(QString::number(result-resultPrev));

      if (result > 0)
      {
      ui->progressBar->setValue((result-resultPrev)*100/result);
        }
}

void Paint::on_pushButton_5_clicked() //Визуализировать память нейрона
{
     ui->tableWidget_2->clear();

        if (ui->comboBox_2->currentIndex() >0)
        {

          for (short int i = 0; i<64; ++i)
            {
              for (short int j = 0; j<64; ++j)
              {

                if (neurons[(ui->comboBox_2->currentIndex()-1)].memory[i*64 +j] > 0)
                 {


                    QColor color = Qt::green;
                    color.setAlpha((25*neurons[(ui->comboBox_2->currentIndex()-1)].memory[i*64 +j]));
                    ui->tableWidget_2->setItem(i,j,new QTableWidgetItem());
                    ui->tableWidget_2->item( i, j)->setBackgroundColor(color);

                 }

              }
            }

        }
}

