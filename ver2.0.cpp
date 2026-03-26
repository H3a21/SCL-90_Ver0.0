#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

// 分类
enum Category {
    SOM, OC, INT, DEP, ANX, HOS, PHOB, PAR, PSY, OTH
};

// 字符串转分类
Category strToCategory(const string& s) {
    if (s == "SOM") return SOM;
    if (s == "OC") return OC;
    if (s == "INT") return INT;
    if (s == "DEP") return DEP;
    if (s == "ANX") return ANX;
    if (s == "HOS") return HOS;
    if (s == "PHOB") return PHOB;
    if (s == "PAR") return PAR;
    if (s == "PSY") return PSY;
    return OTH;
}

string getCategoryName(Category c) {
    switch (c) {
        case SOM: return "躯体化";
        case OC: return "强迫";
        case INT: return "人际敏感";
        case DEP: return "抑郁";
        case ANX: return "焦虑";
        case HOS: return "敌对";
        case PHOB: return "恐怖";
        case PAR: return "偏执";
        case PSY: return "精神病性";
        default: return "其他";
    }
}

// 题目结构
struct Question {
    int id;
    string text;
    Category cat;
};

// 读取文件
vector<Question> loadQuestions(const string& filename) {
    vector<Question> qs;
    ifstream fin(filename);

    if (!fin) {
        cout << "Cannot open file!\n";
        return qs;
    }

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, text, cat;

        getline(ss, id, '|');
        getline(ss, text, '|');
        getline(ss, cat, '|');

        qs.push_back({stoi(id), text, strToCategory(cat)});
    }

    return qs;
}

// 输入
int getInput() {
    int x;
    while (true) {
        cin >> x;
        if (cin.fail() || x < 1 || x > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid(1-5): ";
        } else return x;
    }
}

// 平均
double avg(const vector<int>& v) {
    int sum = 0;
    for (int x : v) sum += x;
    return (double)sum / v.size();
}

int main() {

    //从文件加载
    vector<Question> questions = loadQuestions("questions.txt");
    cout << "有" << questions.size() << "个问题被加载\n";
    if (questions.empty()) {
        cout << "没有任何问题被加载\n";
        return 0;
    }

    vector<int> scores;
    map<Category, vector<int>> catScores;

    cout << "SCL-90 测试(GBK)\n";

    cout << "点击回车以开始测试\n";

    cin.get();

    for (auto& q : questions) {
        cout << "问题" << q.id << " (" << q.text << "): ";
        int s = getInput();

        scores.push_back(s);
        catScores[q.cat].push_back(s);
    }

    double totalAvg = avg(scores);

    map<Category, double> catAvg;
    for (auto& p : catScores) {
        catAvg[p.first] = avg(p.second);
    }

    // 输出文件
    ofstream fout("结果.txt");

    fout << "SCL-90测评结果\n";
    fout << "总平均分: " << totalAvg << "\n\n";

    for (auto& p : catAvg) {
        fout << getCategoryName(p.first)
             << " 平均分: " << p.second << "\n";
    }

    //if ()

    //fout << "您的综合结果为+\n";

    fout << "建议寻找专业心理咨询师寻求进一步说明与帮助";

    fout.close();

    cout << "结果已保存到对应文件夹，命名为结果.txt\n";

    return 0;
}