#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>

using namespace std;
using JSON = nlohmann::json;


class Question {
public:
    JSON question;
    string text;
    string answer;
    string type;
    string trueAnswer;

    Question(JSON t) {
        question = t;
        if (t["type"] == "Choose") {
            type = "Choose";
        } else if (t["type"] == "Text") {
            type = "Text";
        }
    }

    void print() {
        if (question["type"] == "Text") {
            cout << question["question"] << endl;
            cout << "Введите текстовый ответ: ";
            trueAnswer = question["answer"];
        } else if (type == "Choose") {
            int counter = 1;
            cout << question["question"] << endl;
            for (auto a: question["answers"]) {
                cout << counter << ") " << a["text"].get<string>() << endl;
                if (a["how"] == true) {
                    trueAnswer = to_string(counter);
                }
                counter++;
            }
            cout << "В ответ запишите цифру: ";
        }
    }

    int checkCorrect(string userAnswer) {
        if (userAnswer == trueAnswer) {
            return question["value"].get<int>();
        } else {
            return 0;
        }
    }
};

class Quiz {
public:
    JSON data;
    string quizTitle;
    int maxPoints;
    int quizLen;
    int userPoints = 0;

    Quiz(string path) {
        ifstream f(path);
        data = JSON::parse(f);
        quizTitle = getQuizTitle();
        quizLen = getQuizLen();
        maxPoints = getMaxPoints();
    }

    void intro() {
        cout << "Квиз - " << quizTitle << endl;
        cout << "Всего " << quizLen << " заданий" << endl;
        cout << "Максимальный балл - " << maxPoints << endl << endl;
    }

    string getQuizTitle() {
        return data["title"];
    }

    int getQuizLen() {
        return data["test"].size();
    }

    int getMaxPoints() {
        int points = 0;
        for (auto d: data["test"]) {
            points = points + d["value"].get<int>();
        }
        return points;
    }

    void start() {
        for (auto t: data["test"]) {
            string userAnswer;
            Question question(t);
            question.print();
            cin >> userAnswer;
            int newPoints = question.checkCorrect(userAnswer);
            userPoints = userPoints + newPoints;
            cout << "Ответ засчитан" << endl << endl;
        }
    }

    void result() {
        cout << "Ваш результат: " << userPoints << " из " << maxPoints;
    }
};

int main() {
    Quiz quiz("../file.json");
    quiz.intro();
    quiz.start();
    quiz.result();
    return 0;
}