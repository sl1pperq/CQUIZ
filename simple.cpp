#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
using namespace std;
using JSON = nlohmann::json;

int simple() {
    int value = 0;
    int maxValue = 0;
    string answer = "";
    string userAnswer = "";
    int quizValue = 0;
    ifstream f("../file.json");
    JSON data = JSON::parse(f);
    string a = data["title"].get<string>();
    cout << "Начинаем " << a << "! Вот вопросы:" << endl << endl;
    for (auto d : data["test"]) {
        cout << d["question"].get<string>() << endl;
        quizValue = d["value"].get<int>();
        maxValue = maxValue + quizValue;
        if (d["type"] == "Choose") {
            int num = 1;
            for (auto i : d["answers"]) {
                cout << num << ". " << i["text"] << endl;
                if (i["how"] == true) {
                    answer = to_string(num);
                }
                num++;
            }
            cout << "Введите номер правильного ответа: ";
            cin >> userAnswer;
            if (answer == userAnswer) {
                value = value + quizValue;
            }
            cout << "Ваш ответ принят!" << endl << endl;
        } else if (d["type"] == "Text") {
            cout << "Введите текстовый (или числовой) ответ: ";
            quizValue = d["value"].get<int>();
            answer = d["answer"];
            cin >> userAnswer;
            if (userAnswer == answer) {
                value = value + quizValue;
            }
            cout << "Ваш ответ принят!" << endl << endl;
        }
    }
    int percent = round(value / maxValue * 100);
    cout << "Вы получили " << value << " баллов из " << maxValue << ". Это " << percent << "%";
    return 0;
}