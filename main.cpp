#include "mainwindow.h"
#include <typeinfo>
#include <QApplication>


template <typename T>
class CalculatorButton : public QPushButton
{
public:
    static_assert(std::is_same<T, std::string>::value || std::is_same<T, int>::value,
                  "Only strings and integers are allowed as T.");

    T testFunc(T value)
    {
        if constexpr (std::is_same<T, std::string>::value) {
            // Check if the value is a mathematical operation
            if (value == "+" || value == "-" || value == "*" || value == "/") {
                // Return the corresponding character for the operation
                return value;
            } else {
                qDebug() << "Invalid mathematical operation!";
                return T();
            }
        } else if constexpr (std::is_same<T, int>::value) {
            // Return the integer value as is
            return value;
        } else {
            qDebug() << "Invalid type!";
            return T();
        }
    }

    CalculatorButton(T parameter, QWidget *parent = nullptr) : QPushButton(parent)
    {
        setText(QString::number(parameter));

        QObject::connect(this, &QPushButton::released, [this, parameter]() {
            T result = this->testFunc(parameter);  // Pass the parameter as input to testFunc
            qDebug() << "Result: " << result;
        });

        QMargins margins(5, 5, 5, 5); // Set the desired margins
        setContentsMargins(margins);
    }

    ~CalculatorButton() {}
};


template <typename T>
void populateGrid(QGridLayout* grid, std::vector<std::unique_ptr<CalculatorButton<T>>>& calculatorButtons)
{
    int row = 0;
    int col = 0;
    for (const auto& button : calculatorButtons) {
        grid->addWidget(button.get(), row, col, 1, 1);
        col++;
        if (col >= 3) {
            row++;
            if (row == 3)
                col = 1;
            else col = 0;
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication Calculator(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    mainWindow.resize(600, 600);
    mainWindow.setWindowTitle(QApplication::translate("toplevel", "Calculator"));

    QGridLayout* gridLayout = new QGridLayout(&mainWindow);

    std::vector<std::unique_ptr<CalculatorButton<int>>> calculatorButtons;
    for (auto i = 0; i < 10; i++) {
        calculatorButtons.push_back(std::make_unique<CalculatorButton<int>>(i));
    }

    gridLayout->setSpacing(5);

    populateGrid(gridLayout, calculatorButtons);
    QWidget *w = new QWidget();
    w->setLayout(gridLayout);
    mainWindow.setCentralWidget(w);

    return Calculator.exec();
}
